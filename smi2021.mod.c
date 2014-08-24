#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0x654eb549, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xf4344bd9, __VMLINUX_SYMBOL_STR(vb2_ioctl_reqbufs) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0xc7cb5bc4, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xbeb12825, __VMLINUX_SYMBOL_STR(v4l2_event_unsubscribe) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xbc623ac4, __VMLINUX_SYMBOL_STR(video_device_release_empty) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xb6b46a7c, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0xa266732a, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x2f4d0102, __VMLINUX_SYMBOL_STR(v4l2_ctrl_log_status) },
	{ 0xfc20334f, __VMLINUX_SYMBOL_STR(snd_pcm_period_elapsed) },
	{ 0x869a6b8d, __VMLINUX_SYMBOL_STR(snd_card_create) },
	{ 0x8a26fbee, __VMLINUX_SYMBOL_STR(v4l2_device_unregister) },
	{ 0x3ab5cda, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_free) },
	{ 0x982816d7, __VMLINUX_SYMBOL_STR(vb2_fop_poll) },
	{ 0xf7802486, __VMLINUX_SYMBOL_STR(__aeabi_uidivmod) },
	{ 0x4d603702, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamon) },
	{ 0x706dd6d, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xa9f7f400, __VMLINUX_SYMBOL_STR(__video_register_device) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x8e59b175, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_integer) },
	{ 0x5340563b, __VMLINUX_SYMBOL_STR(i2c_add_adapter) },
	{ 0x9744d0ad, __VMLINUX_SYMBOL_STR(v4l2_device_register) },
	{ 0xe9803d90, __VMLINUX_SYMBOL_STR(vb2_fop_read) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xdbbfb0d5, __VMLINUX_SYMBOL_STR(v4l2_device_disconnect) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x9cae52b5, __VMLINUX_SYMBOL_STR(vb2_vmalloc_memops) },
	{ 0x53bda2f9, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x970f2205, __VMLINUX_SYMBOL_STR(vb2_fop_mmap) },
	{ 0xe1c784fd, __VMLINUX_SYMBOL_STR(vb2_ioctl_qbuf) },
	{ 0x7c8869a1, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xdc798d37, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa6a79ff9, __VMLINUX_SYMBOL_STR(video_unregister_device) },
	{ 0xf98f5dfa, __VMLINUX_SYMBOL_STR(snd_pcm_set_ops) },
	{ 0x5cd75500, __VMLINUX_SYMBOL_STR(usb_set_interface) },
	{ 0x64be0625, __VMLINUX_SYMBOL_STR(v4l2_ctrl_subscribe_event) },
	{ 0xc20e7d77, __VMLINUX_SYMBOL_STR(vb2_plane_vaddr) },
	{ 0x87991fdd, __VMLINUX_SYMBOL_STR(vb2_buffer_done) },
	{ 0xea81ef4d, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_pow2) },
	{ 0x9df51c76, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x73e20c1c, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0xe16b893b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xf2027d40, __VMLINUX_SYMBOL_STR(vb2_ioctl_prepare_buf) },
	{ 0x3686da02, __VMLINUX_SYMBOL_STR(snd_pcm_lib_ioctl) },
	{ 0x6345a0f4, __VMLINUX_SYMBOL_STR(vb2_ioctl_create_bufs) },
	{ 0x43b0c9c3, __VMLINUX_SYMBOL_STR(preempt_schedule) },
	{ 0x1ad5cfcb, __VMLINUX_SYMBOL_STR(vb2_ioctl_dqbuf) },
	{ 0xc182049c, __VMLINUX_SYMBOL_STR(i2c_del_adapter) },
	{ 0x3b3e0f03, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xe458cba, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x2f3e0028, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xbe6212cd, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x6fa8570f, __VMLINUX_SYMBOL_STR(vb2_fop_release) },
	{ 0xc2a6fc4b, __VMLINUX_SYMBOL_STR(video_devdata) },
	{ 0xd67889cf, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x2fd85baf, __VMLINUX_SYMBOL_STR(___ratelimit) },
	{ 0xc63c9e31, __VMLINUX_SYMBOL_STR(v4l2_fh_open) },
	{ 0xb9e52429, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x68a24153, __VMLINUX_SYMBOL_STR(snd_pcm_format_physical_width) },
	{ 0x4dd1c2f8, __VMLINUX_SYMBOL_STR(vb2_ioctl_querybuf) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x39861d6b, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_init_class) },
	{ 0xfdcf15a8, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xd1eaa97a, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0x5dc81108, __VMLINUX_SYMBOL_STR(snd_card_free) },
	{ 0xc28f59a8, __VMLINUX_SYMBOL_STR(v4l2_i2c_new_subdev_board) },
	{ 0xb311fa46, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0xe8672195, __VMLINUX_SYMBOL_STR(snd_card_register) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xcc998844, __VMLINUX_SYMBOL_STR(snd_pcm_new) },
	{ 0xa9649df5, __VMLINUX_SYMBOL_STR(vmalloc_to_page) },
	{ 0xa0da52bc, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamoff) },
	{ 0xbc6b134e, __VMLINUX_SYMBOL_STR(vb2_queue_release) },
	{ 0x45455146, __VMLINUX_SYMBOL_STR(v4l2_device_put) },
	{ 0x4e44697a, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0x7b69d4ce, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x1c483a9, __VMLINUX_SYMBOL_STR(v4l2_get_timestamp) },
	{ 0x541f0b23, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0x89de020a, __VMLINUX_SYMBOL_STR(video_ioctl2) },
	{ 0x9326a504, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0xf6acaaf8, __VMLINUX_SYMBOL_STR(vb2_queue_init) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=videobuf2-core,videodev,snd-pcm,snd,videobuf2-vmalloc,v4l2-common";

MODULE_ALIAS("usb:v1C88p0007d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Ed*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Fd*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "C4ABE95EE82D28AB7FBE0D9");
