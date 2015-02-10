#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x31003be0, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc858c48c, __VMLINUX_SYMBOL_STR(vb2_ioctl_reqbufs) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0xbf1b3ce5, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xdae75d88, __VMLINUX_SYMBOL_STR(v4l2_event_unsubscribe) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x49675779, __VMLINUX_SYMBOL_STR(video_device_release_empty) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0x51eafc8e, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x43a53735, __VMLINUX_SYMBOL_STR(__alloc_workqueue_key) },
	{ 0x793fae5a, __VMLINUX_SYMBOL_STR(v4l2_ctrl_log_status) },
	{ 0x7dd8e65e, __VMLINUX_SYMBOL_STR(snd_pcm_period_elapsed) },
	{ 0x4e351535, __VMLINUX_SYMBOL_STR(v4l2_device_unregister) },
	{ 0x99188e56, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_free) },
	{ 0x8e248ccd, __VMLINUX_SYMBOL_STR(vb2_fop_poll) },
	{ 0xf7802486, __VMLINUX_SYMBOL_STR(__aeabi_uidivmod) },
	{ 0x6dc44569, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamon) },
	{ 0x4c6b5877, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
	{ 0x4e900af9, __VMLINUX_SYMBOL_STR(__video_register_device) },
	{ 0x2606a4f1, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x5bb0b415, __VMLINUX_SYMBOL_STR(snd_pcm_stream_lock) },
	{ 0x50728c68, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_integer) },
	{ 0xe9775718, __VMLINUX_SYMBOL_STR(i2c_add_adapter) },
	{ 0x6e354ccb, __VMLINUX_SYMBOL_STR(v4l2_device_register) },
	{ 0x919bf250, __VMLINUX_SYMBOL_STR(vb2_fop_read) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x529b5046, __VMLINUX_SYMBOL_STR(v4l2_device_disconnect) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xfab4a5a6, __VMLINUX_SYMBOL_STR(vb2_vmalloc_memops) },
	{ 0xdfe16b5e, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x51d559d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0x9edc4943, __VMLINUX_SYMBOL_STR(vb2_fop_mmap) },
	{ 0xdbf866ec, __VMLINUX_SYMBOL_STR(vb2_ioctl_qbuf) },
	{ 0x650b7f7b, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xf21c974f, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x2fef3a3f, __VMLINUX_SYMBOL_STR(video_unregister_device) },
	{ 0xf7ccbf41, __VMLINUX_SYMBOL_STR(snd_pcm_set_ops) },
	{ 0x814794a, __VMLINUX_SYMBOL_STR(usb_set_interface) },
	{ 0x602568ba, __VMLINUX_SYMBOL_STR(v4l2_ctrl_subscribe_event) },
	{ 0xbee82085, __VMLINUX_SYMBOL_STR(vb2_plane_vaddr) },
	{ 0x8990df40, __VMLINUX_SYMBOL_STR(vb2_buffer_done) },
	{ 0x6bb3b7e2, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_pow2) },
	{ 0x2400e55c, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x73e20c1c, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0x2b73ae40, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x8c03d20c, __VMLINUX_SYMBOL_STR(destroy_workqueue) },
	{ 0x29079667, __VMLINUX_SYMBOL_STR(vb2_ioctl_prepare_buf) },
	{ 0xdf57d4b2, __VMLINUX_SYMBOL_STR(snd_pcm_lib_ioctl) },
	{ 0xf5c5ae9c, __VMLINUX_SYMBOL_STR(vb2_ioctl_create_bufs) },
	{ 0xe8bb3daf, __VMLINUX_SYMBOL_STR(vb2_ioctl_dqbuf) },
	{ 0x42160169, __VMLINUX_SYMBOL_STR(flush_workqueue) },
	{ 0x57229da3, __VMLINUX_SYMBOL_STR(snd_card_new) },
	{ 0x96f4ae89, __VMLINUX_SYMBOL_STR(i2c_del_adapter) },
	{ 0x4c24b586, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x828be532, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x423826c0, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0xdb9e7f96, __VMLINUX_SYMBOL_STR(vb2_fop_release) },
	{ 0x8c976931, __VMLINUX_SYMBOL_STR(video_devdata) },
	{ 0xb6ddab3a, __VMLINUX_SYMBOL_STR(snd_pcm_stream_unlock) },
	{ 0xc4449513, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x72350130, __VMLINUX_SYMBOL_STR(___ratelimit) },
	{ 0x7d34cb70, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x598542b2, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0x8764e0a2, __VMLINUX_SYMBOL_STR(v4l2_fh_open) },
	{ 0xd85cd67e, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x68a24153, __VMLINUX_SYMBOL_STR(snd_pcm_format_physical_width) },
	{ 0x927ae2d3, __VMLINUX_SYMBOL_STR(vb2_ioctl_querybuf) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x37e8a3cf, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_init_class) },
	{ 0xcaf63d1c, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xed5d7db0, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0x30f02e36, __VMLINUX_SYMBOL_STR(snd_card_free) },
	{ 0x38b59f0e, __VMLINUX_SYMBOL_STR(v4l2_i2c_new_subdev_board) },
	{ 0x4a8ae4b4, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0x4de75ef5, __VMLINUX_SYMBOL_STR(snd_card_register) },
	{ 0x676bbc0f, __VMLINUX_SYMBOL_STR(_set_bit) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x3f7410cf, __VMLINUX_SYMBOL_STR(snd_pcm_new) },
	{ 0x3d21a54f, __VMLINUX_SYMBOL_STR(vmalloc_to_page) },
	{ 0x3c1d5afb, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamoff) },
	{ 0x879479ac, __VMLINUX_SYMBOL_STR(vb2_queue_release) },
	{ 0xdee25d6d, __VMLINUX_SYMBOL_STR(v4l2_device_put) },
	{ 0xa8740e2c, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x1c483a9, __VMLINUX_SYMBOL_STR(v4l2_get_timestamp) },
	{ 0x9e5627b6, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0x294bee86, __VMLINUX_SYMBOL_STR(video_ioctl2) },
	{ 0x953512e5, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0x7dced8af, __VMLINUX_SYMBOL_STR(vb2_queue_init) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=videobuf2-core,videodev,snd-pcm,videobuf2-vmalloc,snd,v4l2-common";

MODULE_ALIAS("usb:v1C88p0007d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Ed*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Fd*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "DBC5010BBE7DEDD3A02F5BD");
