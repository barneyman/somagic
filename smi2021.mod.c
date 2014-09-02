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
	{ 0x648e9c5f, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe8496742, __VMLINUX_SYMBOL_STR(vb2_ioctl_reqbufs) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x9e1e4cf9, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd1b344d1, __VMLINUX_SYMBOL_STR(v4l2_event_unsubscribe) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x816b89d1, __VMLINUX_SYMBOL_STR(video_device_release_empty) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xb6b46a7c, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xde231f4a, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x43a53735, __VMLINUX_SYMBOL_STR(__alloc_workqueue_key) },
	{ 0xda87da52, __VMLINUX_SYMBOL_STR(v4l2_ctrl_log_status) },
	{ 0x64359b35, __VMLINUX_SYMBOL_STR(snd_pcm_period_elapsed) },
	{ 0x39b085d1, __VMLINUX_SYMBOL_STR(snd_card_create) },
	{ 0x8cd6632c, __VMLINUX_SYMBOL_STR(v4l2_device_unregister) },
	{ 0xa9fbf92f, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_free) },
	{ 0xc01cf848, __VMLINUX_SYMBOL_STR(_raw_read_lock) },
	{ 0x73cbf30d, __VMLINUX_SYMBOL_STR(vb2_fop_poll) },
	{ 0x2210eef5, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamon) },
	{ 0x644a445, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xf43ae3d6, __VMLINUX_SYMBOL_STR(__video_register_device) },
	{ 0xe00fad59, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x9dad562d, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_integer) },
	{ 0xa9adc229, __VMLINUX_SYMBOL_STR(snd_pcm_link_rwlock) },
	{ 0x64e9e8f7, __VMLINUX_SYMBOL_STR(i2c_add_adapter) },
	{ 0xda67b8bd, __VMLINUX_SYMBOL_STR(v4l2_device_register) },
	{ 0xd1666034, __VMLINUX_SYMBOL_STR(vb2_fop_read) },
	{ 0xa2eab52a, __VMLINUX_SYMBOL_STR(v4l2_device_disconnect) },
	{ 0xebe82cbc, __VMLINUX_SYMBOL_STR(vb2_vmalloc_memops) },
	{ 0x8f9c2c04, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xf97456ea, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0xf03da735, __VMLINUX_SYMBOL_STR(vb2_fop_mmap) },
	{ 0x1dfecbb, __VMLINUX_SYMBOL_STR(vb2_ioctl_qbuf) },
	{ 0xdf05c681, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xebad96a2, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1b1fc8ed, __VMLINUX_SYMBOL_STR(video_unregister_device) },
	{ 0x2999d569, __VMLINUX_SYMBOL_STR(snd_pcm_set_ops) },
	{ 0x48f19f5b, __VMLINUX_SYMBOL_STR(usb_set_interface) },
	{ 0xb383c290, __VMLINUX_SYMBOL_STR(v4l2_ctrl_subscribe_event) },
	{ 0xe2b576b2, __VMLINUX_SYMBOL_STR(vb2_plane_vaddr) },
	{ 0xfb0deb89, __VMLINUX_SYMBOL_STR(vb2_buffer_done) },
	{ 0xbb1c8fb9, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_pow2) },
	{ 0xbbbb094e, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x73e20c1c, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0x2d8666d0, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x8c03d20c, __VMLINUX_SYMBOL_STR(destroy_workqueue) },
	{ 0xda542108, __VMLINUX_SYMBOL_STR(vb2_ioctl_prepare_buf) },
	{ 0x5bffd6aa, __VMLINUX_SYMBOL_STR(snd_pcm_lib_ioctl) },
	{ 0x4b689b7e, __VMLINUX_SYMBOL_STR(vb2_ioctl_create_bufs) },
	{ 0x37b5dff5, __VMLINUX_SYMBOL_STR(vb2_ioctl_dqbuf) },
	{ 0x42160169, __VMLINUX_SYMBOL_STR(flush_workqueue) },
	{ 0x4101ea9a, __VMLINUX_SYMBOL_STR(i2c_del_adapter) },
	{ 0xf638d884, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xc44618f3, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x3a84118, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0xd800adee, __VMLINUX_SYMBOL_STR(vb2_fop_release) },
	{ 0xdc0739aa, __VMLINUX_SYMBOL_STR(video_devdata) },
	{ 0xc41eede7, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0xb0207ecf, __VMLINUX_SYMBOL_STR(___ratelimit) },
	{ 0x6da84623, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x67f7403e, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x21fb443e, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0xedcad9f7, __VMLINUX_SYMBOL_STR(v4l2_fh_open) },
	{ 0xe45f60d8, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x68a24153, __VMLINUX_SYMBOL_STR(snd_pcm_format_physical_width) },
	{ 0xaebf8aae, __VMLINUX_SYMBOL_STR(vb2_ioctl_querybuf) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9ee6c4b7, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_init_class) },
	{ 0x15c1f0b0, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xda7827aa, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0x8315fee7, __VMLINUX_SYMBOL_STR(snd_card_free) },
	{ 0x65b21f49, __VMLINUX_SYMBOL_STR(v4l2_i2c_new_subdev_board) },
	{ 0x79a1deb6, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0x6f23d0b8, __VMLINUX_SYMBOL_STR(snd_card_register) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x6aabcf89, __VMLINUX_SYMBOL_STR(snd_pcm_new) },
	{ 0x45da9cdb, __VMLINUX_SYMBOL_STR(vmalloc_to_page) },
	{ 0x194fbd7a, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamoff) },
	{ 0xc07afaea, __VMLINUX_SYMBOL_STR(vb2_queue_release) },
	{ 0xce9f2e78, __VMLINUX_SYMBOL_STR(v4l2_device_put) },
	{ 0xc5b8b0b2, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0xc6812b7b, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x1c483a9, __VMLINUX_SYMBOL_STR(v4l2_get_timestamp) },
	{ 0xa5424890, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0xf4f93ca1, __VMLINUX_SYMBOL_STR(video_ioctl2) },
	{ 0xefaf6a47, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
	{ 0x4dea7196, __VMLINUX_SYMBOL_STR(vb2_queue_init) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=videobuf2-core,videodev,snd-pcm,snd,usbcore,i2c-core,videobuf2-vmalloc,v4l2-common";

MODULE_ALIAS("usb:v1C88p0007d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Ed*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Fd*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "6DF17C059F41B3877A37BDD");
