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

#define VENDOR_ID 0x1c88
#define BOOTLOADER_ID 0x0007

#define SMI2021_MODE_CTRL_HEAD		0x01
#define SMI2021_MODE_CAPTURE		0x05
#define SMI2021_MODE_STANDBY		0x03
#define SMI2021_REG_CTRL_HEAD		0x0b

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

static int smi2021_set_reg(struct smi2021 *smi2021, u8 i2c_addr,
			   u16 reg, u8 val)
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
		transfer_buf->data.smi_data.reg_hi = __cpu_to_le16(reg) >> 8;
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
		transfer_buf->data.smi_data.reg_lo = __cpu_to_le16(reg) & 0xff;
		transfer_buf->data.smi_data.reg_hi = __cpu_to_le16(reg) >> 8;
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

#define _EVERY_X_FRAMES	20
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
		//dev_warn(smi2021->dev,"no buffers available\n");
		return NULL;
	}
	buf = list_first_entry(&smi2021->bufs, struct smi2021_buf, list);
	list_del(&buf->list);
	spin_unlock_irqrestore(&smi2021->buf_lock, flags);

	buf->pos=0;

	return buf;
}

static void smi2021_buf_done(struct smi2021 *smi2021)
{
	struct smi2021_buf *buf = smi2021->cur_buf;

	v4l2_get_timestamp(&buf->vb.v4l2_buf.timestamp);
	buf->vb.v4l2_buf.sequence = smi2021->sequence++;
	buf->vb.v4l2_buf.field = V4L2_FIELD_INTERLACED;

	//if (buf->pos < (SMI2021_BYTES_PER_LINE * smi2021->cur_height)) 
	//{
	//	vb2_set_plane_payload(&buf->vb, 0, 0);
	//	vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);

	//	dev_warn(smi2021->dev,"VB2_BUF_STATE_ERROR\n");
	//} 
	//else 
	{
		// hack
		buf->pos=SMI2021_BYTES_PER_LINE* smi2021->cur_height;

		vb2_set_plane_payload(&buf->vb, 0, buf->pos);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_DONE);
	}

	smi2021->cur_buf = NULL;
}



#define BYTES_OF_UYUV_DATA_EXPECTED	1440

static int blitVideoToBuffer(struct smi2021 *smi2021, u8 *sourceBuffer, u8 *endBuffer)
{
	int toWrite=MIN(endBuffer - sourceBuffer, smi2021->parseVideoStateMachine.bytes_remaining_to_fetch);
	struct smi2021_buf *buf = smi2021->cur_buf;
	unsigned offset, lineOfOutput;
	// bounds check buf here?
	if(buf)
	{
		// work out where to start writing, each line alternates between fields
		// f0 l0 - 0
		// f1 l0 - 1
		// f0 l1 - 2
		// f1 l1 - 3

		// work out what line to squirt into
		if(!smi2021->parseVideoStateMachine.fieldNumber)
		{
			lineOfOutput=smi2021->parseVideoStateMachine.active_line_count.field0*2;
		}
		else
		{
			lineOfOutput=(smi2021->parseVideoStateMachine.active_line_count.field1*2)+1;
		}


		// work that into a line's buffer address
		offset=lineOfOutput*BYTES_OF_UYUV_DATA_EXPECTED;
		// and cater for what we've already blit'd down
		offset+=(BYTES_OF_UYUV_DATA_EXPECTED-smi2021->parseVideoStateMachine.bytes_remaining_to_fetch);

		buf->pos+=toWrite;

		// check we're not off the end of the buffer
		if(lineOfOutput < smi2021->cur_height)
		{
			memcpy(buf->mem+offset,sourceBuffer,toWrite);
		}
	}

	return toWrite;
}

static void finishFrameBlit(struct smi2021 *smi2021)
{
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


#define TRC_IS_EAV_MARKER(a)	(a&0x10)
#define TRC_FIELD_THIS_LINE(a)	((a&0x40) ? 1 : 0)
#define TRC_VERTICAL_BLANK(a)	(a&0x20)

// this is the state-machine-handler
static void parse_video(struct smi2021 *smi2021, u8 *p, int size)
{

	u8 *next = p;
	u8 *end = p + size;

	int skip, wrote;
	u8 currentByteVal;

#ifdef DEBUG
	if(!(smi2021->debug.totalFrames%500) && false)
	{
		dev_info(smi2021->dev,"%d:(%d:%d):(%d:%d)\n",	smi2021->debug.totalFrames,
														smi2021->debug.SAV_found_field0, smi2021->debug.SAV_found_field1,
														smi2021->debug.vblank_found_field0, smi2021->debug.vblank_found_field1
				);
	}
#endif


	// sync_state is handling a state machine - we start at HSYNC
	// 
	do
	{
		// get the current byte in the buffer
		
		currentByteVal=*next;

		/*
		 * Timing reference code (TRC):
		 *     [ff 00 00 SAV] [ff 00 00 EAV]
		 * Where SAV is 80 or c7, and EAV is 9d or da.
		 * A line of video will look like (1448 bytes total):
		 *     [ff 00 00 EAV] [ff 00 00 SAV] [1440 bytes of UYVY video] (repeat on next line)
		 */
		switch (smi2021->parseVideoStateMachine.sync_state) 
		{
			//case VHSYNC_BLANK1:
			//	// we're expecting 0x80 or ff
			//	if(currentByteVal==0x80)
			//	{
			//		smi2021->parseVideoStateMachine.sync_state = VHSYNC_BLANK2;
			//	}
			//	else
			//	{
			//		// unexpected, fall back to HSYNC
			//		smi2021->parseVideoStateMachine.sync_state = HSYNC;
			//	}
			//	next++;
			//	break;
			case VHSYNC_BLANK2:
				// we're expecting 0x10
				if(currentByteVal==0x10)
				{
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
				if(currentByteVal == 0x80)
				{
					// another blank
					// round again
					smi2021->parseVideoStateMachine.sync_state = VHSYNC_BLANK2;
					next++;
					break;
				}
				// otherwise fall thru

			case HSYNC:
				// we expect ff
				if (currentByteVal == 0xff) 
				{
					// move up the state tree
					smi2021->parseVideoStateMachine.sync_state = SYNCZ1;
				} 
				next++;
				break;

			case SYNCZ1:
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
				}
				next++;
				break;

			case SYNCZ2:
				// we expect 00
				if (!currentByteVal) 
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
				}
				next++;
				break;

			case SYNCAV:
				/*
				 * Found 0xff 0x00 0x00, now expecting SAV or EAV. Might
				 * also be the SDID (sliced data ID), 0x00.
				 */
				if (!currentByteVal) 
				{
					/*
					 * SDID detected, so we still haven't found the
					 * active YUV data.
					 */
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
					// now we have 80-10 ... until the horizblank is done
					smi2021->parseVideoStateMachine.sync_state = VHSYNC_BLANK1_OR_HSYNCH;
					next++;
					break;
				} 
				else
				{
					/* SAV (start of active data) */


					// is this a vertblank - i.e. end of frame
					if (TRC_VERTICAL_BLANK(currentByteVal))
					{
						smi2021->parseVideoStateMachine.sync_state = VBLANK;
						// if it's field1 then we've finished this frame, both times


#ifdef _USE_VBLANK_DODGY_TO_DETECT_FRAME_DONE

						// get the field number - 0 or 1 
						smi2021->parseVideoStateMachine.fieldNumber = TRC_FIELD_THIS_LINE(currentByteVal);

#ifdef DEBUG

						if(smi2021->parseVideoStateMachine.fieldNumber)
							smi2021->debug.SAV_found_field0++;
						else
							smi2021->debug.SAV_found_field1++;
#endif

						// sanity check the row we're on .. this is uber dodgy
						if( (smi2021->parseVideoStateMachine.fieldNumber?smi2021->parseVideoStateMachine.active_line_count.field1:smi2021->parseVideoStateMachine.active_line_count.field1 ) > 250)
						{

							if(smi2021->parseVideoStateMachine.fieldNumber) 
							{
								/* flag as done, dusted */
								smi2021->parseVideoStateMachine.endOfFrameDetected=true;

								// if we're grabbing this frame, commit to the v4l
								if(!smi2021->parseVideoStateMachine.frameBeingIgnored)
								{
									finishFrameBlit(smi2021);
								}

#ifdef DEBUG
								smi2021->debug.totalFrames++;
#endif

							}
						}
#endif

					}
					else 
					{

#ifndef _USE_VBLANK_DODGY_TO_DETECT_FRAME_DONE


#ifdef DEBUG

						if(smi2021->parseVideoStateMachine.fieldNumber)
							smi2021->debug.SAV_found_field0++;
						else
							smi2021->debug.SAV_found_field1++;
#endif

						// IIF we are throwing away vblank frames, and ignoing THEIR field number
						// if the last field we saw was 1, and THIS one is 0, we've ended frame
						if(smi2021->parseVideoStateMachine.fieldNumber && !TRC_FIELD_THIS_LINE(currentByteVal))
						{
							/* flag as done, dusted */
							smi2021->parseVideoStateMachine.endOfFrameDetected=true;

							// if we're grabbing this frame, commit to the v4l
							if(!smi2021->parseVideoStateMachine.frameBeingIgnored)
							{
								finishFrameBlit(smi2021);
							}

#ifdef DEBUG
							smi2021->debug.totalFrames++;
#endif
						}

						// get the field number NOW - 0 or 1 
						smi2021->parseVideoStateMachine.fieldNumber = TRC_FIELD_THIS_LINE(currentByteVal);

#endif



						/* Line is active video */
						smi2021->parseVideoStateMachine.sync_state = VACTIVE;

						// if the last thing we saw was a blank, this is the start of a new frame
						if(smi2021->parseVideoStateMachine.endOfFrameDetected)
						{
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
					}


					// if vblank or vactive, there's still a full row expected
					smi2021->parseVideoStateMachine.bytes_remaining_to_fetch = BYTES_OF_UYUV_DATA_EXPECTED;
				}
				next++;
				break;


				// in 625/50 and 525/60, the blanking lines are the 'extra lines' beyond 576/483 (respectively)
			case VBLANK:
					// skip to the end of this ????, which is either to the end of the buffer, or the end of remaining bytes
					skip = MIN(smi2021->parseVideoStateMachine.bytes_remaining_to_fetch, (end - next));
					smi2021->parseVideoStateMachine.bytes_remaining_to_fetch -= skip;
					next += skip ;

#ifdef DEBUG
					if(smi2021->parseVideoStateMachine.fieldNumber)
						smi2021->debug.vblank_found_field0++;
					else
						smi2021->debug.vblank_found_field1++;
#endif

					// if we've consumed this line start again
					if(smi2021->parseVideoStateMachine.bytes_remaining_to_fetch<=0)
					{
						smi2021->parseVideoStateMachine.sync_state = HSYNC;
					}

				break;

			case VACTIVE:


				// we are actively consuming row data
				wrote=blitVideoToBuffer(smi2021, next, end);

				smi2021->parseVideoStateMachine.bytes_remaining_to_fetch -= wrote;
				next += wrote;

				if (smi2021->parseVideoStateMachine.bytes_remaining_to_fetch <= 0) 
				{
					smi2021->parseVideoStateMachine.sync_state = HSYNC;

					// and up the numbers
					if(!smi2021->parseVideoStateMachine.fieldNumber)
						smi2021->parseVideoStateMachine.active_line_count.field0++;
					else
						smi2021->parseVideoStateMachine.active_line_count.field1++;
				}

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
static void process_packet(struct smi2021 *smi2021, u8 *p, int size)
{
	int i, sizeOfthisChunk;
	u32 *header;

	if (size % 0x400 != 0) 
	{
		printk_ratelimited(KERN_INFO "smi2021::%s: size: %d\n",	__func__, size);
		return;
	}

	for (i = 0; i < size; i += 0x400) 
	{
		sizeOfthisChunk = MIN(0x400,size-i);
		header = (u32 *)(p + i);
		switch (*header) 
		{
			case cpu_to_be32(0xaaaa0000):
				//dev_info(smi2021->dev, "process_packet VIDEO (%x)\n",size);
				parse_video(smi2021, p+i+4, sizeOfthisChunk-4);
				break;
			case cpu_to_be32(0xaaaa0001):
				//dev_info(smi2021->dev, "process_packet AUDIO (%x)\n",size);
				//smi2021_audio(smi2021, p+i+4, sizeOfthisChunk-4);
				break;
			default:
				//dev_warn(smi2021->dev, "process_packet UNKNOWN (%d)\n",size);
				break;

		}
	}
}

static void smi2021_iso_cb(struct urb *ip)
{
	static int seenCount=0;

	int i, rc;
	struct smi2021 *smi2021 = ip?ip->context:NULL;

	if(!smi2021)
	{
		dev_err(smi2021->dev, "null pointer in smi2021_iso_cb\n");
		return;
	}

	if(!((++seenCount)%500))
		dev_info(smi2021->dev, "seen %d complete\n", seenCount);


	switch (ip->status) {
		case 0:
		/* All fine */
			break;
		/* Device disconnected or capture stopped? */
		case -ENODEV:
		case -ENOENT:
		case -ECONNRESET:
		case -ESHUTDOWN:
			return;
		/* Unknown error, retry */
		default:
			dev_warn(smi2021->dev, "urb error! status %d\n", ip->status);
			goto resubmit;
	}


	for (i = 0; i < ip->number_of_packets; i++) 
	{
		int size = ip->iso_frame_desc[i].actual_length;
		unsigned char *data = ip->transfer_buffer + ip->iso_frame_desc[i].offset;
		process_packet(smi2021, data, size);
	}


resubmit:
	if(ip)
	{
		rc = usb_submit_urb(ip, GFP_ATOMIC);
		if (rc)
			dev_warn(smi2021->dev, "urb re-submit failed (%d)\n", rc);
	}
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
	dev_info(smi2021->dev, "smi2021_toggle_audio\n");

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

	dev_info(smi2021->dev, "smi2021_start\n");

	memset(&smi2021->parseVideoStateMachine,0,sizeof(smi2021->parseVideoStateMachine));
	smi2021->parseVideoStateMachine.sync_state = HSYNC;
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




	/*
	 * Reset RTSO0 6 Times (Bit 7)
	 * The Windows driver does this, not sure if it's really needed.
	 */
	smi2021_get_reg(smi2021, 0x4a, 0x0e, &reg);

	dev_info(smi2021->dev,"register 0x0E : %02x\n", reg);

	reg |= 0x80;
	for (i = 0; i < 6; i++)
		smi2021_set_reg(smi2021, 0x4a, 0x0e, reg);

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

	smi2021_toggle_audio(smi2021, false);


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

	// i'm ASSUMING that setting AUFD (above) will make the chip work out what's going on, and a subsequent query will
	// reflect what's actually happneing ... so let's do the subsequent query ...
	smi2021_get_reg(smi2021, 0x4a, 0x08, &reg);

	// this is an important flag for handling vertical line sync in the parse_video state machine
	smi2021->fieldSelection=(reg&0x40)?itu556_525ln60hz:itu556_625ln50hz;

	dev_info(smi2021->dev,"smi2021->fieldSelection %d\n", smi2021->fieldSelection);

	/* I have no idea about what this register does with this value. */
	smi2021_set_reg(smi2021, 0, 0x1800, 0x0d);

	return 0;

start_fail:
	smi2021_stop(smi2021);

	return rc;

}

void smi2021_stop(struct smi2021 *smi2021)
{
	int i;
	unsigned long flags;

	dev_info(smi2021->dev, "smi2021_stop\n");


	/* Cancel running transfers */
	for (i = 0; i < SMI2021_ISOC_TRANSFERS; i++) {
		struct urb *ip = smi2021->isoc_urbs[i];
		if (ip == NULL)
			continue;
		usb_kill_urb(ip);
		kfree(ip->transfer_buffer);
		usb_free_urb(ip);
		smi2021->isoc_urbs[i] = NULL;
	}

	usb_set_interface(smi2021->udev, 0, 0);
	smi2021_set_mode(smi2021, SMI2021_MODE_STANDBY);

	smi2021_stop_audio(smi2021);

	/* Return buffers to userspace */
	spin_lock_irqsave(&smi2021->buf_lock, flags);
	while (!list_empty(&smi2021->bufs)) 
	{
		struct smi2021_buf *buf = list_first_entry(&smi2021->bufs,
						struct smi2021_buf, list);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
		list_del(&buf->list);
	}
	spin_unlock_irqrestore(&smi2021->buf_lock, flags);

	return;
}

static void smi2021_release(struct v4l2_device *v4l2_dev)
{
	struct smi2021 *smi2021 = container_of(v4l2_dev, struct smi2021,
								v4l2_dev);
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
	smi2021->cur_norm = V4L2_STD_NTSC;
	smi2021->cur_height = SMI2021_NTSC_LINES;

	smi2021->cur_norm = V4L2_STD_PAL;
	smi2021->cur_height = SMI2021_PAL_LINES;


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


	v4l2_device_call_all(&smi2021->v4l2_dev, 0, video, s_routing,
			smi2021->vid_inputs[smi2021->cur_input].type, 0, 0);
	v4l2_device_call_all(&smi2021->v4l2_dev, 0, core, s_std,
			smi2021->cur_norm);

	usb_set_intfdata(intf, smi2021);
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
