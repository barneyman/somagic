/************************************************************************
 * smi2021_main.c							*
 *									*
 * USB Driver for SMI2021 - EasyCAP					*
 * **********************************************************************
 *
 * Copyright 2011-2013 Jon Arne Jørgensen
 * <jonjon.arnearne--a.t--gmail.com>
 *
 * Copyright 2011, 2012 Tony Brown, Michal Demin, Jeffry Johnston
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * This driver is heavily influensed by the STK1160 driver.
 * Copyright (C) 2012 Ezequiel Garcia
 * <elezegarcia--a.t--gmail.com>
 *
 */

#include "smi2021.h"

#include <linux/delay.h>

#define VENDOR_ID		0x1c88
#define BOOTLOADER_ID	0x0007

#define SMI2021_MODE_CTRL_HEAD		0x01
#define SMI2021_MODE_CAPTURE		0x05
#define SMI2021_MODE_STANDBY		0x03
#define SMI2021_REG_CTRL_HEAD		0x0b

// get off the URB callback ASAP by putting the returned URB into a workqueue
#define _USE_WORK_QUEUE

#ifdef _USE_WORK_QUEUE
#include <linux/workqueue.h>
#endif

static int smi2021_set_mode(struct smi2021 *smi2021, u8 mode)
{
	int pipe, rc;
	struct mode_ctrl_transfer {
		u8 head;
		u8 mode;
	} *transfer_buf = kzalloc(sizeof(*transfer_buf), GFP_KERNEL);

	dev_info(smi2021->dev, "smi2021_set_mode\n");

	if (transfer_buf == NULL)
		return -ENOMEM;

	transfer_buf->head = SMI2021_MODE_CTRL_HEAD;
	transfer_buf->mode = mode;

	pipe = usb_sndctrlpipe(smi2021->udev, SMI2021_USB_SNDPIPE);
	rc = usb_control_msg(smi2021->udev, pipe, SMI2021_USB_REQUEST,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			transfer_buf->head, SMI2021_USB_INDEX,
			transfer_buf, sizeof(*transfer_buf), 1000);

	kfree(transfer_buf);

	return rc;
}

/*
 * The smi2021 chip will handle two different types of register settings.
 * Settings for the gm7113c chip via i2c or settings for the smi2021 chip.
 * All settings are passed with the following struct.
 * Some bits in data_offset and data_cntl parameters tells the device what
 * kind of setting it's receiving and if it's a read or write request.
 */
struct smi2021_reg_ctrl_transfer {
	u8 head;
	u8 i2c_addr;
	u8 data_cntl;
	u8 data_offset;
	u8 data_size;
	union data {
		u8 val;
		struct i2c_data {
			u8 reg;
			u8 val;
		} __packed i2c_data;
		struct smi_data {
			u8 reg_hi;
			u8 reg_lo;
			u8 val;
		} __packed smi_data;
		u8 reserved[8];
	} __packed data;
} __packed;

static int smi2021_set_reg(struct smi2021 *smi2021, u8 i2c_addr,  u16 reg, u8 val)
{
	int rc, pipe;
	struct smi2021_reg_ctrl_transfer *transfer_buf;

	static const struct smi2021_reg_ctrl_transfer smi_data = {
		.head = SMI2021_REG_CTRL_HEAD,
		.i2c_addr = 0x00,
		.data_cntl = 0x00,
		.data_offset = 0x82,
		.data_size = sizeof(u8),
	};

	static const struct smi2021_reg_ctrl_transfer i2c_data = {
		.head = SMI2021_REG_CTRL_HEAD,
		.i2c_addr = 0x00,
		.data_cntl = 0xc0,
		.data_offset = 0x01,
		.data_size = sizeof(u8)
	};

	//dev_info(smi2021->dev, "smi2021_set_reg\n");

	if (smi2021->udev == NULL) {
		rc = -ENODEV;
		goto out;
	}

	transfer_buf = kzalloc(sizeof(*transfer_buf), GFP_KERNEL);
	if (transfer_buf == NULL) {
		rc = -ENOMEM;
		goto out;
	}

	if (i2c_addr) {
		memcpy(transfer_buf, &i2c_data, sizeof(*transfer_buf));
		transfer_buf->i2c_addr = i2c_addr;
		transfer_buf->data.i2c_data.reg = reg;
		transfer_buf->data.i2c_data.val = val;
	} else {
		memcpy(transfer_buf, &smi_data, sizeof(*transfer_buf));
		transfer_buf->data.smi_data.reg_lo = __cpu_to_le16(reg) & 0xff;
		transfer_buf->data.smi_data.reg_hi = __cpu_to_le16(reg)>> 8 ;
		transfer_buf->data.smi_data.val = val;
	}

	pipe = usb_sndctrlpipe(smi2021->udev, SMI2021_USB_SNDPIPE);
	rc = usb_control_msg(smi2021->udev, pipe, SMI2021_USB_REQUEST,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			transfer_buf->head, SMI2021_USB_INDEX,
			transfer_buf, sizeof(*transfer_buf), 1000);

	kfree(transfer_buf);
out:
	return rc;
}

static int smi2021_get_reg(struct smi2021 *smi2021, u8 i2c_addr,
			   u16 reg, u8 *val)
{
	int rc, pipe;
	struct smi2021_reg_ctrl_transfer *transfer_buf;

	static const struct smi2021_reg_ctrl_transfer i2c_prepare_read = {
		.head = SMI2021_REG_CTRL_HEAD,
		.i2c_addr = 0x00,
		.data_cntl = 0x84,
		.data_offset = 0x00,
		.data_size = sizeof(u8)
	};

	static const struct smi2021_reg_ctrl_transfer smi_read = {
		.head = SMI2021_REG_CTRL_HEAD,
		.i2c_addr = 0x00,
		.data_cntl = 0x20,
		.data_offset = 0x82,
		.data_size = sizeof(u8)
	};

	//dev_info(smi2021->dev, "smi2021_get_reg\n");

	*val = 0;

	if (smi2021->udev == NULL) {
		rc = -ENODEV;
		goto out;
	}

	transfer_buf = kzalloc(sizeof(*transfer_buf), GFP_KERNEL);
	if (transfer_buf == NULL) {
		rc = -ENOMEM;
		goto out;
	}

	pipe = usb_sndctrlpipe(smi2021->udev, SMI2021_USB_SNDPIPE);

	if (i2c_addr) {
		memcpy(transfer_buf, &i2c_prepare_read, sizeof(*transfer_buf));
		transfer_buf->i2c_addr = i2c_addr;
		transfer_buf->data.i2c_data.reg = reg;

		rc = usb_control_msg(smi2021->udev, pipe, SMI2021_USB_REQUEST,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			transfer_buf->head, SMI2021_USB_INDEX,
			transfer_buf, sizeof(*transfer_buf), 1000);
		if (rc < 0)
			goto free_out;

		transfer_buf->data_cntl = 0xa0;
	} else {
		memcpy(transfer_buf, &smi_read, sizeof(*transfer_buf));
		transfer_buf->data.smi_data.reg_lo = __cpu_to_le16(reg)& 0xff ;
		transfer_buf->data.smi_data.reg_hi = __cpu_to_le16(reg )>> 8;
	}

	rc = usb_control_msg(smi2021->udev, pipe, SMI2021_USB_REQUEST,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			transfer_buf->head, SMI2021_USB_INDEX,
			transfer_buf, sizeof(*transfer_buf), 1000);
	if (rc < 0)
		goto free_out;

	pipe = usb_rcvctrlpipe(smi2021->udev, SMI2021_USB_RCVPIPE);
	rc = usb_control_msg(smi2021->udev, pipe, SMI2021_USB_REQUEST,
			USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			transfer_buf->head, SMI2021_USB_INDEX,
			transfer_buf, sizeof(*transfer_buf), 1000);
	if (rc < 0)
		goto free_out;

	*val = transfer_buf->data.val;

free_out:
	kfree(transfer_buf);
out:
	return rc;
}

static int smi2021_i2c_xfer(struct i2c_adapter *i2c_adap,
				struct i2c_msg msgs[], int num)
{
	struct smi2021 *smi2021 = i2c_adap->algo_data;

	switch (num) {
	case 2:  /* Read reg */
		if (msgs[0].len != 1 || msgs[1].len != 1)
			goto err_out;

		if ((msgs[1].flags & I2C_M_RD) != I2C_M_RD)
			goto err_out;
		smi2021_get_reg(smi2021, msgs[0].addr, msgs[0].buf[0],
							msgs[1].buf);
		break;
	case 1: /* Write reg */
		if (msgs[0].len == 0)
			break;
		else if (msgs[0].len != 2)
			goto err_out;
		if (msgs[0].buf[0] == 0)
			break;
		smi2021_set_reg(smi2021, msgs[0].addr, msgs[0].buf[0],
							msgs[0].buf[1]);
		break;
	default:
		goto err_out;
	}
	return num;

err_out:
	return -EOPNOTSUPP;
}

static u32 smi2021_i2c_functionality(struct i2c_adapter *adap)
{
	return I2C_FUNC_SMBUS_EMUL;
}

static struct i2c_algorithm smi2021_algo = {
	.master_xfer = smi2021_i2c_xfer,
	.functionality = smi2021_i2c_functionality,
};

/* gm7113c_init table overrides */
static enum saa7113_r10_ofts r10_ofts = SAA7113_OFTS_VFLAG_BY_VREF;
static bool r10_vrln = true;
static bool r13_adlsb = true;

static struct saa7115_platform_data gm7113c_data = {
	.saa7113_r10_ofts = &r10_ofts,
	.saa7113_r10_vrln = &r10_vrln,
	.saa7113_r13_adlsb = &r13_adlsb,
};

static struct i2c_board_info gm7113c_info = {
	.type = "gm7113c",
	.addr = 0x4a,
	.platform_data = &gm7113c_data,
};

static int smi2021_initialize(struct smi2021 *smi2021)
{
	int i, rc;

	/*
	 * These registers initializes the smi2021 chip,
	 * but I have not been able to figure out exactly what they do.
	 * My guess is that they toggle the reset pins of the
	 * cs5350 and gm7113c chips.
	 */
	static const u16 init[][2] = {
		{ 0x3a, 0x80 },
		{ 0x3b, 0x00 },
		{ 0x34, 0x01 },
		{ 0x35, 0x00 },
		{ 0x34, 0x11 },
		{ 0x35, 0x11 },
		{ 0x3b, 0x80 },
		{ 0x3b, 0x00 },
	};

	dev_info(smi2021->dev, "smi2021_initialize\n");


	for (i = 0; i < ARRAY_SIZE(init); i++) {
		rc = smi2021_set_reg(smi2021, 0x00, init[i][0], init[i][1]);
		if (rc < 0)
			return rc;
	}

	return 0;
}

//try to fake 8mb/s out (via h264)
//#define _EVERY_X_FRAMES	5
#ifdef _EVERY_X_FRAMES
static unsigned debugCount=_EVERY_X_FRAMES;
#endif

static struct smi2021_buf *smi2021_get_buf(struct smi2021 *smi2021)
{
	unsigned long flags;
	struct smi2021_buf *buf = NULL;

#ifdef _EVERY_X_FRAMES
	// debug! catch a frame every _EVERY_X_FRAMES
	if(debugCount--)
	{
		return NULL;
	}
	
	debugCount=_EVERY_X_FRAMES;
#endif

	spin_lock_irqsave(&smi2021->buf_lock, flags);
	if (list_empty(&smi2021->bufs)) 
	{
		/* No free buffers, userspace likely too slow! */
		spin_unlock_irqrestore(&smi2021->buf_lock, flags);
		smi2021->runtimeStats.missedV4lBuffers++;
		return NULL;
	}

	buf = list_first_entry(&smi2021->bufs, struct smi2021_buf, list);
	list_del(&buf->list);
	spin_unlock_irqrestore(&smi2021->buf_lock, flags);

	return buf;
}

static void smi2021_buf_done(struct smi2021 *smi2021)
{
	struct smi2021_buf *buf = smi2021->cur_buf;

	if (buf)
	{

		v4l2_get_timestamp(&buf->vb.v4l2_buf.timestamp);
		buf->vb.v4l2_buf.sequence = smi2021->sequence++;
		buf->vb.v4l2_buf.field = V4L2_FIELD_INTERLACED;

		vb2_set_plane_payload(&buf->vb, 0, SMI2021_BYTES_PER_LINE* smi2021->currentFrameHeight);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_DONE);

		smi2021->cur_buf = NULL;
	}
}


// 1440
#define BYTES_OF_UYUV_DATA_EXPECTED	SMI2021_BYTES_PER_LINE


static int blitVideoToBuffer(struct smi2021 *smi2021, u8 *sourceBuffer, u8 *endBuffer)
{
	int toWrite=MIN(endBuffer - sourceBuffer, smi2021->parseVideoStateMachine.bytes_remaining_to_fetch);
	int totalBytesToWrite=toWrite;
	struct smi2021_buf *buf = smi2021->cur_buf;
	unsigned offset, lineOfOutput;


	// bounds check buf here?
	if(buf)
	{
		// work out where to start writing, dependent on field number
		// work out what line to squirt into
		if(!smi2021->parseVideoStateMachine.fieldNumber)
		{
			// f0 is top/even
			lineOfOutput=smi2021->parseVideoStateMachine.active_line_count.field0*2;
		}
		else
		{
			// f1 is bottom/odd
			lineOfOutput=(smi2021->parseVideoStateMachine.active_line_count.field1*2)+1;
		}

		// work that into a line's buffer address
		offset=lineOfOutput*BYTES_OF_UYUV_DATA_EXPECTED;
		// and cater for what we've already blit'd down
		offset+=(BYTES_OF_UYUV_DATA_EXPECTED-smi2021->parseVideoStateMachine.bytes_remaining_to_fetch);

		// check we're not off the end of the buffer
		if(lineOfOutput < smi2021->currentFrameHeight)
		{
			// copy in
			if(toWrite)
			{
				// it's not word aligned, do it the hard way
				memcpy(buf->mem+offset,sourceBuffer,toWrite);
			}

		}
		else
		{
			smi2021->runtimeStats.tooManyScanlines++;
		}
	}

	// remember we've consumed some
	smi2021->parseVideoStateMachine.bytes_remaining_to_fetch -= totalBytesToWrite;

	return totalBytesToWrite;
}

static void finishFrameBlit(struct smi2021 *smi2021)
{
	// if we HAVE a buffer, offload it
	if(smi2021->cur_buf)
		smi2021_buf_done(smi2021);
}


/*
 * Scan the saa7113 Active video data.
 * This data is:
 *	4 bytes header (0xff 0x00 0x00 [TRC/SAV])
 *	1440 bytes of UYUV Video data
 *	4 bytes footer (0xff 0x00 0x00 [TRC/EAV])
 *
 * TRC = Time Reference Code.
 * SAV = Start Active Video.
 * EAV = End Active Video.
 * This is described in the saa7113 datasheet.
 */


#define TRC_IS_EAV_MARKER(a)	((a)&0x10)
#define TRC_FIELD_THIS_LINE(a)	(((a)&0x40) ? 1 : 0)
#define TRC_VERTICAL_BLANK(a)	((a)&0x20)


// this is the state-machine-handler
static void parse_video(struct smi2021 *smi2021, u8 *p, int size)
{

	u8 *next = p;
	u8 *end = p + size;

	int skip, wrote;
	u8 currentByteVal;

	// sync_state is handling a state machine - we start at HSYNC
	// 
	do
	{
		// get the current byte in the buffer
		
		currentByteVal=*next;

		/*
		 * Timing reference code (TRC):
		 *     [ff 00 00 SAV] [ff 00 00 EAV]
		 * A line of video will look like (1448 bytes total):
		 *     [ff 00 00 EAV] [ff 00 00 SAV] [1440 bytes of UYVY video] (repeat on next line)
		 */
		switch (smi2021->parseVideoStateMachine.sync_state) 
		{

			case VHSYNC_BLANK2:
				smi2021->runtimeStats.blank2++;
				// we're expecting 0x10
				if(currentByteVal==(u8)0x10)
				{
					smi2021->runtimeStats.horizBlanks++;
					// next is either another blank or start of data
					smi2021->parseVideoStateMachine.sync_state = VHSYNC_BLANK1_OR_HSYNCH;
				}
				else
				{
					// unexpected, fall back to HSYNC
					smi2021->parseVideoStateMachine.sync_state = HSYNC;
				}
				next++;
				break;


			case VHSYNC_BLANK1_OR_HSYNCH:
				// we're expecting 80 for another blank, or ff for another hsync
				if(currentByteVal == (u8)0x80)
				{
					smi2021->runtimeStats.blank1++;
					// another blank
					// round again
					smi2021->parseVideoStateMachine.sync_state = VHSYNC_BLANK2;
					next++;
					break;
				}
				// otherwise fall thru - bad form for switch ... so sue me

			case HSYNC:
				smi2021->runtimeStats.hsync++;
				// we expect ff
				if (currentByteVal == (u8)0xff) 
				{
					// move up the state tree
					smi2021->parseVideoStateMachine.sync_state = SYNCZ1;
				} 
				else
				{
					smi2021->runtimeStats.missedHSync++;
				}
				next++;
				break;

			case SYNCZ1:
				smi2021->runtimeStats.synchz1++;
				// we expect 00
				if (!currentByteVal) 
				{
					// up the state tree
					smi2021->parseVideoStateMachine.sync_state = SYNCZ2;
				} 
				else 
				{
					/*
					 * The 2nd byte in the TRC must be 0x00. It
					 * wasn't, so sync was lost.
					 */
					smi2021->parseVideoStateMachine.sync_state = HSYNC;
					dev_warn(smi2021->dev,"HSYNC fallback from SYNCZ1 (%02x)\n", currentByteVal);

				}
				next++;
				break;

			case SYNCZ2:
				smi2021->runtimeStats.synchz2++;
				// we expect 00
				if (currentByteVal==0) 
				{
					smi2021->parseVideoStateMachine.sync_state = SYNCAV;
				} 
				else 
				{
					/*
					 * The 3rd byte in the TRC must be 0x00. It
					 * wasn't, so sync was lost.
					 */
					smi2021->parseVideoStateMachine.sync_state = HSYNC;
					dev_warn(smi2021->dev,"HSYNC fallback from SYNCZ2 (%02x)\n", currentByteVal);
				}
				next++;
				break;

			case SYNCAV:
				smi2021->runtimeStats.synchav++;
				/*
				 * Found 0xff 0x00 0x00, now expecting SAV or EAV. Might
				 * also be the SDID (sliced data ID), 0x00.
				 */
				if (!(currentByteVal&0x80))
				{
					/*
					 * SDID detected, so we still haven't found the
					 * active YUV data.
					 */
					dev_info(smi2021->dev,"Slice Data seen\n");

					smi2021->parseVideoStateMachine.sync_state = HSYNC;
					next++;
					break;
				}

				/*
				 * H = Bit 4 (mask 0x10).
				 * 0: in SAV, 1: in EAV.
				 */

				if (TRC_IS_EAV_MARKER(currentByteVal))
				{
					/* EAV (end of active data) */
					// now we have n of 80-10, where n>=0  ... until the horizblank is done
					// tho' it's documented, i've never seen it
					smi2021->parseVideoStateMachine.sync_state = VHSYNC_BLANK1_OR_HSYNCH;
				} 
				else
				{
					// is this a vertblank just ignore it
					if (TRC_VERTICAL_BLANK(currentByteVal))
					{
						smi2021->parseVideoStateMachine.sync_state = VBLANK;
						smi2021->runtimeStats.vertBlanks++;
					}
					else 
					{

						if(smi2021->parseVideoStateMachine.fieldNumber)
							smi2021->runtimeStats.SAV_found_field0++;
						else
							smi2021->runtimeStats.SAV_found_field1++;

						// IFF we are throwing away vblank frames (and ignoring THEIR field number) then
						// if the last field we saw was 1, and THIS one is 0, we've ended the frame
						if(smi2021->parseVideoStateMachine.fieldNumber>0 && TRC_FIELD_THIS_LINE(currentByteVal)==0)
						{
							// if we're grabbing this frame, commit to the v4l
							if(!smi2021->parseVideoStateMachine.frameBeingIgnored)
							{
								smi2021->runtimeStats.caughtFrames++;
								finishFrameBlit(smi2021);
							}
							else
							{
								smi2021->runtimeStats.ignoredFrames++;
							}


							// reset counters (endOfFrameDetected, bytes_remaining_to_fetch, active_line_count, field, vblank_found)
							memset(&smi2021->parseVideoStateMachine,0,sizeof(smi2021->parseVideoStateMachine));

							// get a buffer
							smi2021->cur_buf=smi2021_get_buf(smi2021);

							// if we havent got a buffer then we should throw this whole frame away ...
							if(!smi2021->cur_buf)
							{
								smi2021->parseVideoStateMachine.frameBeingIgnored=true;
							}

						}

						// get the field number NOW - 0 or 1 
						smi2021->parseVideoStateMachine.fieldNumber = TRC_FIELD_THIS_LINE(currentByteVal);

						/* Line is active video */
						smi2021->parseVideoStateMachine.sync_state = VACTIVE;

					}
					// if vblank or vactive, there's still a full row expected
					smi2021->parseVideoStateMachine.bytes_remaining_to_fetch = BYTES_OF_UYUV_DATA_EXPECTED;

				}
				next++;
				break;

				// in 625/50 and 525/60, the blanking lines are the 'extra lines' beyond 576/483 (respectively)
			case VBLANK:
				smi2021->runtimeStats.blank++;

				// skip to the end of this packet, which may or may not contain all expected bytes, if it doesn't we'll be back here soon
				skip = MIN(smi2021->parseVideoStateMachine.bytes_remaining_to_fetch, (end - next));
				smi2021->parseVideoStateMachine.bytes_remaining_to_fetch -= skip;
				next += skip ;

				// if we've consumed this line start the state machine again
				if(!smi2021->parseVideoStateMachine.bytes_remaining_to_fetch)
				{
					smi2021->parseVideoStateMachine.sync_state = HSYNC;
				}
				break;

			case VACTIVE:
				smi2021->runtimeStats.active++;
				// we are actively consuming row data
				wrote=blitVideoToBuffer(smi2021, next, end);
				next += wrote;

				// if we've consumed all expected bytes, flag a state machine restart
				if (!smi2021->parseVideoStateMachine.bytes_remaining_to_fetch) 
				{
					smi2021->parseVideoStateMachine.sync_state = HSYNC;

					// and up the numbers
					if(!smi2021->parseVideoStateMachine.fieldNumber)
						smi2021->parseVideoStateMachine.active_line_count.field0++;
					else
						smi2021->parseVideoStateMachine.active_line_count.field1++;
				}

				break;
			default:
				dev_info(smi2021->dev,"impossible condition\n");
				break;
		} /* end switch */

	} while (next < end);

}


/*
 * The device delivers data in chunks of 0x400 bytes.
 * The four first bytes is a magic header to identify the chunks.
 *	0xaa 0xaa 0x00 0x00 = saa7113 Active Video Data
 *	0xaa 0xaa 0x00 0x01 = PCM - 24Bit 2 Channel audio data
 */
static void process_packet(struct smi2021 *smi2021, u8 *p, int size, int intendedLength)
{
	int i, sizeOfthisChunk;
	u32 *header;

	if (size % 0x400 != 0) 
	{
		printk_ratelimited(KERN_INFO "smi2021::%s: size: %d\n",	__func__, size);
		return;
	}

	if(!size)
	{
		//dev_info(smi2021->dev,"zero len URB\n");
		smi2021->runtimeStats.zeroLenURBs++;
		return;
	}

	for (i = 0; i < size; i += 0x400) 
	{
		sizeOfthisChunk = 0x400;//MIN(0x400,size-i);
		header = (u32 *)(p + i);
		switch (*header) 
		{
			case cpu_to_be32(0xaaaa0000):
				smi2021->runtimeStats.videoPackets++;
				parse_video(smi2021, p+i+4, sizeOfthisChunk-4);
				break;
			case cpu_to_be32(0xaaaa0001):
				smi2021->runtimeStats.audioPackets++;
				//smi2021_audio(smi2021, p+i+4, sizeOfthisChunk-4);
				break;
			default:
				smi2021->runtimeStats.unknownPackets++;
				break;

		}
	}
}


#ifdef _USE_WORK_QUEUE

static struct workqueue_struct *processUSBworkQueue;
	
typedef struct {
  struct work_struct my_work;
  struct urb *ip;
} my_work_t;




static void processUSBworkFn(struct work_struct *work)
{
	int rc,i;
	my_work_t *my_work = (my_work_t *)work;
	struct urb *ip=my_work->ip;

	struct smi2021 *smi2021 = ip?ip->context:NULL;

	for (i = 0; i < ip->number_of_packets; i++) 
	{
		// for legibility
		struct usb_iso_packet_descriptor *descriptor=&ip->iso_frame_desc[i];

		int size = descriptor->actual_length;
		unsigned char *data = ip->transfer_buffer + descriptor->offset;
		int frameStatus=descriptor->status;

		if(frameStatus)
		{
			dev_warn(smi2021->dev, "frame status %x\n",frameStatus);
		}
		else
		{
			process_packet(smi2021, data, size, descriptor->length);
		}

		// reset 
		descriptor->status=descriptor->actual_length=0;

	}

	rc = usb_submit_urb(ip, GFP_ATOMIC);
	if (rc)
		dev_warn(smi2021->dev, "urb re-submit failed (%d)\n", rc);

	// work was malloc'd for me, so free it
	kfree(work);
}

#endif


static void smi2021_iso_cb(struct urb *ip)
{
#ifdef _USE_WORK_QUEUE
	my_work_t* work = NULL;
#else
	int i;
#endif

	int rc;
	struct smi2021 *smi2021 = ip?ip->context:NULL;

	if(!smi2021)
	{
		dev_err(smi2021->dev, "null pointer in smi2021_iso_cb\n");
		return;
	}

	smi2021->runtimeStats.URBcount++;

	switch (ip->status) {
		case 0:
		/* All fine */
			break;
		/* Device disconnected or capture stopped? */
		case -ENODEV:
		case -ENOENT:
		case -ECONNRESET:
		case -ESHUTDOWN:
			//dev_warn(smi2021->dev, "urb handled error - not resubmitted status %d\n", ip->status);
			return;
		/* Unknown error, retry */
		default:
			dev_warn(smi2021->dev, "urb error! status %d\n", ip->status);
			rc = usb_submit_urb(ip, GFP_ATOMIC);
			if (rc)
				dev_warn(smi2021->dev, "urb re-submit failed (%d)\n", rc);
			break;
	}

	if(ip->error_count)
	{
		dev_err(smi2021->dev, "URB error count %d\n", ip->error_count);
	}
#ifdef _USE_WORK_QUEUE

	// alloc new work - i'm assuming kmalloc is more efficient than me keeping a stack of work packets
	work = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
	if(work)
	{
		INIT_WORK((struct work_struct *)work, &processUSBworkFn);

		// point this work at the URB
		work->ip=ip;

		queue_work(processUSBworkQueue, (struct work_struct *)work);
	}

#else

	for (i = 0; i < ip->number_of_packets; i++) 
	{
		struct usb_iso_packet_descriptor *frame=&ip->iso_frame_desc[i];
		
		u8 *data = ip->transfer_buffer + frame->offset;

		if(frame->status)
		{
			dev_warn(smi2021->dev, "frame status %x\n",frame->status);
		}
		else
		{
			process_packet(smi2021, data, frame->actual_length, frame->length);
		}

		// and reset
		frame->status=frame->actual_length=0;
	}

	if(ip)
	{
		ip->error_count=0;
		rc = usb_submit_urb(ip, GFP_ATOMIC);
		if (rc)
			dev_warn(smi2021->dev, "urb re-submit failed (%d)\n", rc);
	}
#endif


}

static struct urb *smi2021_setup_iso_transfer(struct smi2021 *smi2021)
{
	struct urb *ip;
	int i, size = smi2021->iso_size;

	ip = usb_alloc_urb(SMI2021_ISOC_PACKETS, GFP_KERNEL);
	if (ip == NULL)
		return NULL;

	ip->dev = smi2021->udev;
	ip->context = smi2021;

	ip->pipe = usb_rcvisocpipe(smi2021->udev, SMI2021_ISOC_EP);
	ip->interval = 1;
	ip->start_frame = 0;

	ip->transfer_flags = URB_ISO_ASAP;
	ip->transfer_buffer = kzalloc(SMI2021_ISOC_PACKETS * size, GFP_KERNEL);

	ip->complete = smi2021_iso_cb;

	ip->number_of_packets = SMI2021_ISOC_PACKETS;
	ip->transfer_buffer_length = SMI2021_ISOC_PACKETS * size;
	
	for (i = 0; i < SMI2021_ISOC_PACKETS; i++) 
	{
		ip->iso_frame_desc[i].offset = size * i;
		ip->iso_frame_desc[i].length = size;
	}

	return ip;
}

void smi2021_toggle_audio(struct smi2021 *smi2021, bool enable)
{
	dev_info(smi2021->dev, "smi2021_toggle_audio %s\n",enable?"ON":"off");

	/*
	 * I know that setting this register enables and disables
	 * the transfer of audio data over usb.
	 * I have no idea about what the number 0x1d really represents.
	 * */
	if (enable)
		smi2021_set_reg(smi2021, 0, 0x1740, 0x1d);
	else
		smi2021_set_reg(smi2021, 0, 0x1740, 0x00);
}

int smi2021_start(struct smi2021 *smi2021)
{
	int i, rc;
	u8 reg;

	dev_info(smi2021->dev, "smi2021_start - streaming started\n");

	// clean the per frame data
	memset(&smi2021->parseVideoStateMachine,0,sizeof(smi2021->parseVideoStateMachine));
	memset(&smi2021->runtimeStats,0,sizeof(smi2021->runtimeStats));
	// cue up the state machine
	smi2021->parseVideoStateMachine.sync_state = VHSYNC_BLANK1_OR_HSYNCH;
	// and don't even bother to get the first (partial) frame 
	smi2021->parseVideoStateMachine.frameBeingIgnored=true;


	v4l2_device_call_all(&smi2021->v4l2_dev, 0, video, s_stream, 1);

	/*
	 * Enble automatic field detection on gm7113c (Bit 7)
	 * It seems the device needs this to not fail when receiving bad video
	 * i.e. from an old VHS tape.
	 */
	smi2021_get_reg(smi2021, 0x4a, 0x08, &reg);
	smi2021_set_reg(smi2021, 0x4a, 0x08, reg | 0x80);
	dev_info(smi2021->dev,"register 0x08 : %02x\n", reg);

#ifdef _USE_WORK_QUEUE
	// create the work queue
	processUSBworkQueue=create_workqueue("URB Queue");
	dev_info(smi2021->dev,"using work queues\n");
#endif

	// TODO - lift the 'full set' of sets from the somagic-capture user code


	/*
	 * Reset RTSO0 6 Times (Bit 7)
	 * The Windows driver does this, not sure if it's really needed.
	 */
	smi2021_get_reg(smi2021, 0x4a, 0x0e, &reg);

	dev_info(smi2021->dev,"register 0x0E : %02x\n", reg);

	reg |= 0x80;
	for (i = 0; i < 6; i++)
		smi2021_set_reg(smi2021, 0x4a, 0x0e, reg);

	// set vblank in accordance with itu656
	smi2021_get_reg(smi2021, 0x4a, 0x10, &reg);
	dev_info(smi2021->dev,"register 0x10 : %02x\n", reg);
	reg&=0x3f;
	smi2021_set_reg(smi2021, 0x4a, 0x10, reg);


	rc = smi2021_set_mode(smi2021, SMI2021_MODE_CAPTURE);
	if (rc < 0)
		goto start_fail;

	rc = usb_set_interface(smi2021->udev, 0, 2);
	if (rc < 0)
		goto start_fail;
	// i'm ASSUMING that setting AUFD (above) will make the chip work out what's going on, and a subsequent query will
	// reflect what's actually happneing ... so let's do the subsequent query ...
	smi2021_get_reg(smi2021, 0x4a, 0x08, &reg);

	// this is an important flag for handling vertical line sync in the parse_video state machine
	smi2021->fieldSelection=(reg&0x40)?itu556_525ln60hz:itu556_625ln50hz;

	dev_info(smi2021->dev,"smi2021->fieldSelection %d\n", smi2021->fieldSelection);

	/* I have no idea about what this register does with this value. */
	smi2021_set_reg(smi2021, 0, 0x1800, 0x0d);

	smi2021_toggle_audio(smi2021, false);

	// sleep an ikkle bit to let it settle?
	msleep(300);

	for (i = 0; i < SMI2021_ISOC_TRANSFERS; i++) 
	{
		struct urb *ip = smi2021_setup_iso_transfer(smi2021);

		if (ip == NULL) 
		{
			rc = -ENOMEM;
			goto start_fail;
		}

		smi2021->isoc_urbs[i] = ip;

		rc = usb_submit_urb(ip, GFP_KERNEL);

		if (rc < 0)
			goto start_fail;
	}


	return 0;

start_fail:
	smi2021_stop(smi2021);

	return rc;

}

void smi2021_stop(struct smi2021 *smi2021)
{
	int i;
	unsigned long flags;

	dev_info(smi2021->dev, "smi2021_stop: caught %u, ignored %u, dropped %u, HSYNC misses %u\n%x 0len URBs\n", 
		smi2021->runtimeStats.caughtFrames,smi2021->runtimeStats.ignoredFrames,
		smi2021->runtimeStats.missedV4lBuffers,
		smi2021->runtimeStats.missedHSync,

		smi2021->runtimeStats.zeroLenURBs
		);

	dev_info(smi2021->dev,"Packets: V %x A %x ? %x\nHBLANKS %x VERT %x PokeBeyond %x f0 %x f1 %x\n",
		smi2021->runtimeStats.videoPackets, 
		smi2021->runtimeStats.audioPackets, 
		smi2021->runtimeStats.unknownPackets,

		smi2021->runtimeStats.horizBlanks,
		smi2021->runtimeStats.vertBlanks,

		smi2021->runtimeStats.tooManyScanlines,

		smi2021->runtimeStats.SAV_found_field0, 
		smi2021->runtimeStats.SAV_found_field1
		);

	dev_info(smi2021->dev,"states h %x,b2 %x,b1 %x,z1 %x,z2 %x,av %x,b %x,a %x\nURBS %x",
		smi2021->runtimeStats.hsync, 
		smi2021->runtimeStats.blank2, 
		smi2021->runtimeStats.blank1, 
		smi2021->runtimeStats.synchz1, 
		smi2021->runtimeStats.synchz2, 
		smi2021->runtimeStats.synchav, 
		smi2021->runtimeStats.blank, 
		smi2021->runtimeStats.active,

		smi2021->runtimeStats.URBcount
		);

	// tell the dongle to stop
	usb_set_interface(smi2021->udev, 0, 0);
	smi2021_set_mode(smi2021, SMI2021_MODE_STANDBY);


	/* Cancel running transfers */
	for (i = 0; i < SMI2021_ISOC_TRANSFERS; i++) {
		struct urb *ip = smi2021->isoc_urbs[i];
		if (ip == NULL)
			continue;

		// kill it
		usb_kill_urb(ip);

#ifndef _USE_WORK_QUEUE
		kfree(ip->transfer_buffer);
		usb_free_urb(ip);
		smi2021->isoc_urbs[i] = NULL;
#endif
	}

#ifdef _USE_WORK_QUEUE

	// wait a 'bit' for the URBs above to be cancelled, and flow thru to work queue
	msleep(250);

	// then flush those
	flush_workqueue(processUSBworkQueue);
	// and clean up
	destroy_workqueue(processUSBworkQueue);

	for (i = 0; i < SMI2021_ISOC_TRANSFERS; i++) {
		struct urb *ip = smi2021->isoc_urbs[i];
		if (ip == NULL)
			continue;
		kfree(ip->transfer_buffer);
		usb_free_urb(ip);
		smi2021->isoc_urbs[i] = NULL;
	}

#endif

	// kill the buffer we may have in play
	smi2021_buf_done(smi2021);


	smi2021_stop_audio(smi2021);

	/* Return buffers to userspace */
	spin_lock_irqsave(&smi2021->buf_lock, flags);
	while (!list_empty(&smi2021->bufs)) 
	{
		struct smi2021_buf *buf = list_first_entry(&smi2021->bufs,struct smi2021_buf, list);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
		list_del(&buf->list);
	}
	spin_unlock_irqrestore(&smi2021->buf_lock, flags);

	return;
}

static void smi2021_release(struct v4l2_device *v4l2_dev)
{
	struct smi2021 *smi2021 = container_of(v4l2_dev, struct smi2021, v4l2_dev);
	i2c_del_adapter(&smi2021->i2c_adap);

	v4l2_ctrl_handler_free(&smi2021->ctrl_handler);
	v4l2_device_unregister(&smi2021->v4l2_dev);
	vb2_queue_release(&smi2021->vb2q);
	kfree(smi2021);
}

/************************************************************************
 *									*
 *          DEVICE  -  PROBE   &   DISCONNECT				*
 *									*
 ***********************************************************************/

static const struct usb_device_id smi2021_usb_device_id_table[] = {
	{ USB_DEVICE(VENDOR_ID, BOOTLOADER_ID)	},
	{ USB_DEVICE(VENDOR_ID, 0x003c)		},
	{ USB_DEVICE(VENDOR_ID, 0x003d)		},
	{ USB_DEVICE(VENDOR_ID, 0x003e)		},
	{ USB_DEVICE(VENDOR_ID, 0x003f)		},
	{ }
};
MODULE_DEVICE_TABLE(usb, smi2021_usb_device_id_table);

static const struct smi2021_vid_input dual_input[] = {
	{
		.name = "Composite",
		.type = SAA7115_COMPOSITE0,
	},
	{
		.name = "S-Video",
		.type = SAA7115_SVIDEO1,
	}
};

static const struct smi2021_vid_input quad_input[] = {
	{
		.name = "Composite 0",
		.type = SAA7115_COMPOSITE0,
	},
	{
		.name = "Composite 1",
		.type = SAA7115_COMPOSITE1,
	},
	{
		.name = "Composite 2",
		.type = SAA7115_COMPOSITE2,
	},
	{
		.name = "Composite 3",
		.type = SAA7115_COMPOSITE3,
	},
};

// set by 'me' or by v4l thru a set_std call
bool smi2021_setSourceSignalFormat(struct smi2021 *thisSMI2021, v4l2_std_id standardId)
{
	if(thisSMI2021)
	{
		thisSMI2021->cur_norm=standardId;

		if (standardId & V4L2_STD_525_60)
			thisSMI2021->currentFrameHeight = SMI2021_NTSC_LINES;
		else if (standardId & V4L2_STD_625_50)
			thisSMI2021->currentFrameHeight = SMI2021_PAL_LINES;
		else
			return false;

		return true;
	}

	return false;
}

static int smi2021_usb_probe(struct usb_interface *intf, const struct usb_device_id *devid)
{
	int rc, size, input_count;
	const struct smi2021_vid_input *vid_inputs;
	struct device *dev = &intf->dev;
	struct usb_device *udev = interface_to_usbdev(intf);
	struct smi2021 *smi2021;

	if (udev->descriptor.idProduct == BOOTLOADER_ID)
		return smi2021_bootloader_probe(intf, devid);

	if (intf->num_altsetting != 3)
		return -ENODEV;
	if (intf->altsetting[2].desc.bNumEndpoints != 1)
		return -ENODEV;

	size = usb_endpoint_maxp(&intf->altsetting[2].endpoint[0].desc);

	dev_info(dev, "max packet size %d bytes - additional frames %d",size & 0x07ff, (((size & 0x1800) >> 11)) );
	// usb spec says & 0x7fff dictates max packet size
	// & (0x1800 ROR 11) outlines how many EXTRA sets are available in Hi-Speed+ micro-frames
	size = (size & 0x07ff) * (((size & 0x1800) >> 11) + 1);

	// primary difference is the late firmwares can handle the quad input variant
	switch (udev->descriptor.idProduct) {
	case 0x3e:
	case 0x3f:
		input_count = ARRAY_SIZE(quad_input);
		vid_inputs = quad_input;
		break;
	case 0x3c:
	case 0x3d:
	default:
		input_count = ARRAY_SIZE(dual_input);
		vid_inputs = dual_input;
	}

	smi2021 = kzalloc(sizeof(struct smi2021), GFP_KERNEL);
	if (dev == NULL)
		return -ENOMEM;

	smi2021->dev = dev;
	smi2021->udev = usb_get_dev(udev);

	smi2021->vid_input_count = input_count;
	smi2021->vid_inputs = vid_inputs;
	smi2021->iso_size = size;

	/* videobuf2 struct and locks */
	smi2021_setSourceSignalFormat(smi2021,V4L2_STD_NTSC);
	smi2021_setSourceSignalFormat(smi2021,V4L2_STD_PAL);


	spin_lock_init(&smi2021->buf_lock);
	mutex_init(&smi2021->v4l2_lock);
	mutex_init(&smi2021->vb2q_lock);

	INIT_LIST_HEAD(&smi2021->bufs);

	rc = smi2021_vb2_setup(smi2021);
	if (rc < 0) 
	{
		dev_warn(dev, "Could not initialize videobuf2 queue\n");
		goto smi2021_fail;
	}

	rc = v4l2_ctrl_handler_init(&smi2021->ctrl_handler, 0);
	if (rc < 0) {
		dev_warn(dev, "Could not initialize v4l2 ctrl handler\n");
		goto ctrl_fail;
	}

	/* v4l2 struct */
	smi2021->v4l2_dev.release = smi2021_release;
	smi2021->v4l2_dev.ctrl_handler = &smi2021->ctrl_handler;
	rc = v4l2_device_register(dev, &smi2021->v4l2_dev);
	if (rc < 0) {
		dev_warn(dev, "Could not register v4l2 device\n");
		goto v4l2_fail;
	}

	smi2021_initialize(smi2021);

	/* i2c adapter */
	strlcpy(smi2021->i2c_adap.name, "smi2021",sizeof(smi2021->i2c_adap.name));

	smi2021->i2c_adap.dev.parent = smi2021->dev;
	smi2021->i2c_adap.owner = THIS_MODULE;
	smi2021->i2c_adap.algo = &smi2021_algo;
	smi2021->i2c_adap.algo_data = smi2021;
	i2c_set_adapdata(&smi2021->i2c_adap, &smi2021->v4l2_dev);
	rc = i2c_add_adapter(&smi2021->i2c_adap);
	if (rc < 0) {
		dev_warn(dev, "Could not add i2c adapter\n");
		goto i2c_fail;
	}

	/* i2c client */
	strlcpy(smi2021->i2c_client.name, "smi2021 internal",
				sizeof(smi2021->i2c_client.name));
	smi2021->i2c_client.adapter = &smi2021->i2c_adap;
	smi2021->gm7113c_subdev = v4l2_i2c_new_subdev_board(&smi2021->v4l2_dev,
							&smi2021->i2c_adap,
							&gm7113c_info, NULL);


	v4l2_device_call_all(&smi2021->v4l2_dev, 0, video, s_routing,smi2021->vid_inputs[smi2021->cur_input].type, 0, 0);
	v4l2_device_call_all(&smi2021->v4l2_dev, 0, video, s_std, smi2021->cur_norm);

	usb_set_intfdata(intf, smi2021);

	// enable audio queue
	smi2021_snd_register(smi2021);


	/* video structure */
	rc = smi2021_video_register(smi2021);
	if (rc < 0) 
	{
		dev_warn(dev, "Could not register video device\n");
		goto vdev_fail;
	}

	dev_info(dev, "Somagic Easy-Cap Video Grabber (BJF build)\n");
	return 0;

vdev_fail:
	i2c_del_adapter(&smi2021->i2c_adap);
i2c_fail:
	v4l2_device_unregister(&smi2021->v4l2_dev);
v4l2_fail:
	v4l2_ctrl_handler_free(&smi2021->ctrl_handler);
ctrl_fail:
	vb2_queue_release(&smi2021->vb2q);
smi2021_fail:
	kfree(smi2021);

	return rc;

}

static void smi2021_usb_disconnect(struct usb_interface *intf)
{
	struct smi2021 *smi2021;
	struct usb_device *udev = interface_to_usbdev(intf);

	if (udev->descriptor.idProduct == BOOTLOADER_ID)
		return smi2021_bootloader_disconnect(intf);

	smi2021 = usb_get_intfdata(intf);
	smi2021_snd_unregister(smi2021);

	mutex_lock(&smi2021->vb2q_lock);
	mutex_lock(&smi2021->v4l2_lock);

	usb_set_intfdata(intf, NULL);
	video_unregister_device(&smi2021->vdev);
	v4l2_device_disconnect(&smi2021->v4l2_dev);
	usb_put_dev(smi2021->udev);
	smi2021->udev = NULL;

	mutex_unlock(&smi2021->v4l2_lock);
	mutex_unlock(&smi2021->vb2q_lock);

	v4l2_device_put(&smi2021->v4l2_dev);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jon Arne Jørgensen <jonjon.arnearne--a.t--gmail.com>");
MODULE_DESCRIPTION("SMI2021 - EasyCap");
MODULE_VERSION(SMI2021_DRIVER_VERSION);

struct usb_driver smi2021_usb_driver = {
	.name = "smi2021",
	.id_table = smi2021_usb_device_id_table,
	.probe = smi2021_usb_probe,
	.disconnect = smi2021_usb_disconnect
};

module_usb_driver(smi2021_usb_driver);
