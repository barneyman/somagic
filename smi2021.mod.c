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
	{ 0xb8057589, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe410bc64, __VMLINUX_SYMBOL_STR(vb2_ioctl_reqbufs) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0xdb68a066, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x3dbe63fa, __VMLINUX_SYMBOL_STR(v4l2_event_unsubscribe) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x7f9f260b, __VMLINUX_SYMBOL_STR(video_device_release_empty) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0x51eafc8e, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x43a53735, __VMLINUX_SYMBOL_STR(__alloc_workqueue_key) },
	{ 0xa2b3b816, __VMLINUX_SYMBOL_STR(v4l2_ctrl_log_status) },
	{ 0x478c5124, __VMLINUX_SYMBOL_STR(snd_pcm_period_elapsed) },
	{ 0x5e628952, __VMLINUX_SYMBOL_STR(v4l2_device_unregister) },
	{ 0x8cc1e73b, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_free) },
	{ 0xc775d730, __VMLINUX_SYMBOL_STR(vb2_fop_poll) },
	{ 0xf7802486, __VMLINUX_SYMBOL_STR(__aeabi_uidivmod) },
	{ 0x852a8935, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamon) },
	{ 0xf28262c1, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
	{ 0x8f081595, __VMLINUX_SYMBOL_STR(__video_register_device) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0xe77acb3f, __VMLINUX_SYMBOL_STR(snd_pcm_stream_lock) },
	{ 0x284b9d76, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_integer) },
	{ 0xd697e69a, __VMLINUX_SYMBOL_STR(trace_hardirqs_on) },
	{ 0xba27d79d, __VMLINUX_SYMBOL_STR(i2c_add_adapter) },
	{ 0xd62b6af2, __VMLINUX_SYMBOL_STR(v4l2_device_register) },
	{ 0x5b719ac9, __VMLINUX_SYMBOL_STR(vb2_fop_read) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x38226f39, __VMLINUX_SYMBOL_STR(v4l2_device_disconnect) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xc97f0150, __VMLINUX_SYMBOL_STR(vb2_vmalloc_memops) },
	{ 0xd079fcd1, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x50b1a148, __VMLINUX_SYMBOL_STR(vb2_fop_mmap) },
	{ 0x2086688b, __VMLINUX_SYMBOL_STR(vb2_ioctl_qbuf) },
	{ 0x42cf001a, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xdc798d37, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xf5ab153e, __VMLINUX_SYMBOL_STR(video_unregister_device) },
	{ 0xbdf2e1bd, __VMLINUX_SYMBOL_STR(snd_pcm_set_ops) },
	{ 0x1129c1ab, __VMLINUX_SYMBOL_STR(usb_set_interface) },
	{ 0x5e571d1a, __VMLINUX_SYMBOL_STR(v4l2_ctrl_subscribe_event) },
	{ 0x52ffa5e8, __VMLINUX_SYMBOL_STR(vb2_plane_vaddr) },
	{ 0x32b46d96, __VMLINUX_SYMBOL_STR(vb2_buffer_done) },
	{ 0x8ec51d8a, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_pow2) },
	{ 0xbf20cc77, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x73e20c1c, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0xe16b893b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x8c03d20c, __VMLINUX_SYMBOL_STR(destroy_workqueue) },
	{ 0x14a20f7c, __VMLINUX_SYMBOL_STR(vb2_ioctl_prepare_buf) },
	{ 0xeabf48c2, __VMLINUX_SYMBOL_STR(snd_pcm_lib_ioctl) },
	{ 0xf7936bf9, __VMLINUX_SYMBOL_STR(vb2_ioctl_create_bufs) },
	{ 0x43b0c9c3, __VMLINUX_SYMBOL_STR(preempt_schedule) },
	{ 0x3f9cb425, __VMLINUX_SYMBOL_STR(vb2_ioctl_dqbuf) },
	{ 0x42160169, __VMLINUX_SYMBOL_STR(flush_workqueue) },
	{ 0x3fbef055, __VMLINUX_SYMBOL_STR(snd_card_new) },
	{ 0x2ce2a227, __VMLINUX_SYMBOL_STR(i2c_del_adapter) },
	{ 0x97f220d6, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x63d9701d, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x9e3013c1, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x258c276f, __VMLINUX_SYMBOL_STR(vb2_fop_release) },
	{ 0x9d6800b2, __VMLINUX_SYMBOL_STR(video_devdata) },
	{ 0x1dfd75a9, __VMLINUX_SYMBOL_STR(snd_pcm_stream_unlock) },
	{ 0xbbbd519c, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x2fd85baf, __VMLINUX_SYMBOL_STR(___ratelimit) },
	{ 0xa771e3c4, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x1b880659, __VMLINUX_SYMBOL_STR(v4l2_fh_open) },
	{ 0xb9e52429, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x68a24153, __VMLINUX_SYMBOL_STR(snd_pcm_format_physical_width) },
	{ 0x701cceda, __VMLINUX_SYMBOL_STR(vb2_ioctl_querybuf) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x649dace1, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_init_class) },
	{ 0x97f0e014, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xb6ecab4b, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0x185ac498, __VMLINUX_SYMBOL_STR(snd_card_free) },
	{ 0xe819d637, __VMLINUX_SYMBOL_STR(v4l2_i2c_new_subdev_board) },
	{ 0x3aacee5b, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0x93062e73, __VMLINUX_SYMBOL_STR(snd_card_register) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xdbef45cf, __VMLINUX_SYMBOL_STR(snd_pcm_new) },
	{ 0xfd91c1b9, __VMLINUX_SYMBOL_STR(vmalloc_to_page) },
	{ 0xd20314bb, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamoff) },
	{ 0xfa5e73ca, __VMLINUX_SYMBOL_STR(vb2_queue_release) },
	{ 0xec3d2e1b, __VMLINUX_SYMBOL_STR(trace_hardirqs_off) },
	{ 0xa0f8288f, __VMLINUX_SYMBOL_STR(v4l2_device_put) },
	{ 0xcd6e398f, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x1c483a9, __VMLINUX_SYMBOL_STR(v4l2_get_timestamp) },
	{ 0x1d0b51ee, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0x621370f9, __VMLINUX_SYMBOL_STR(video_ioctl2) },
	{ 0xcca02b4d, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0xcf02cd1a, __VMLINUX_SYMBOL_STR(vb2_queue_init) },
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

MODULE_INFO(srcversion, "34A0E6BC80E16B58792AA30");
