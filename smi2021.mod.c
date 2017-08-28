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

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xf3252e61, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xda521667, __VMLINUX_SYMBOL_STR(vb2_ioctl_reqbufs) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x802df6e4, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x89d3065b, __VMLINUX_SYMBOL_STR(v4l2_event_unsubscribe) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x9ec1ff5f, __VMLINUX_SYMBOL_STR(video_device_release_empty) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0x2869831, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x43a53735, __VMLINUX_SYMBOL_STR(__alloc_workqueue_key) },
	{ 0x737f5ede, __VMLINUX_SYMBOL_STR(v4l2_ctrl_log_status) },
	{ 0x70118114, __VMLINUX_SYMBOL_STR(snd_pcm_period_elapsed) },
	{ 0xb9c5de86, __VMLINUX_SYMBOL_STR(v4l2_device_unregister) },
	{ 0x5d4d8b52, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_free) },
	{ 0xa6e63947, __VMLINUX_SYMBOL_STR(vb2_fop_poll) },
	{ 0xf7802486, __VMLINUX_SYMBOL_STR(__aeabi_uidivmod) },
	{ 0xfebe297, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamon) },
	{ 0x531fac56, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
	{ 0x947acaab, __VMLINUX_SYMBOL_STR(__video_register_device) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x54761a95, __VMLINUX_SYMBOL_STR(snd_pcm_stream_lock) },
	{ 0x5af282bc, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_integer) },
	{ 0xd697e69a, __VMLINUX_SYMBOL_STR(trace_hardirqs_on) },
	{ 0x2606a864, __VMLINUX_SYMBOL_STR(i2c_add_adapter) },
	{ 0x1aceb972, __VMLINUX_SYMBOL_STR(v4l2_device_register) },
	{ 0xc76cd74d, __VMLINUX_SYMBOL_STR(vb2_fop_read) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x91705c17, __VMLINUX_SYMBOL_STR(v4l2_device_disconnect) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xd865a6c3, __VMLINUX_SYMBOL_STR(vb2_vmalloc_memops) },
	{ 0xbffd2fa7, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xc43b6105, __VMLINUX_SYMBOL_STR(vb2_fop_mmap) },
	{ 0x6d5e2302, __VMLINUX_SYMBOL_STR(vb2_ioctl_qbuf) },
	{ 0x5cf1fcf2, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xdc798d37, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x23752739, __VMLINUX_SYMBOL_STR(video_unregister_device) },
	{ 0xe41498b3, __VMLINUX_SYMBOL_STR(snd_pcm_set_ops) },
	{ 0x82eec158, __VMLINUX_SYMBOL_STR(usb_set_interface) },
	{ 0x6dff4d90, __VMLINUX_SYMBOL_STR(v4l2_ctrl_subscribe_event) },
	{ 0x2e0b84db, __VMLINUX_SYMBOL_STR(vb2_plane_vaddr) },
	{ 0xed0b2ef2, __VMLINUX_SYMBOL_STR(vb2_buffer_done) },
	{ 0x9138fb6e, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_pow2) },
	{ 0x3f1e7835, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x73e20c1c, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0xe16b893b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x8c03d20c, __VMLINUX_SYMBOL_STR(destroy_workqueue) },
	{ 0x7348d0c4, __VMLINUX_SYMBOL_STR(vb2_ioctl_prepare_buf) },
	{ 0xbe9d953b, __VMLINUX_SYMBOL_STR(snd_pcm_lib_ioctl) },
	{ 0xae64025b, __VMLINUX_SYMBOL_STR(vb2_ioctl_create_bufs) },
	{ 0xc608fc2f, __VMLINUX_SYMBOL_STR(vb2_ioctl_dqbuf) },
	{ 0x42160169, __VMLINUX_SYMBOL_STR(flush_workqueue) },
	{ 0x7e975aba, __VMLINUX_SYMBOL_STR(snd_card_new) },
	{ 0x2495ea63, __VMLINUX_SYMBOL_STR(i2c_del_adapter) },
	{ 0x16822871, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xa31e99e4, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x3a8acba, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0xe16adfe1, __VMLINUX_SYMBOL_STR(vb2_fop_release) },
	{ 0xc5e68a51, __VMLINUX_SYMBOL_STR(video_devdata) },
	{ 0xe20c89b0, __VMLINUX_SYMBOL_STR(snd_pcm_stream_unlock) },
	{ 0xe6d10bbb, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x65fd066a, __VMLINUX_SYMBOL_STR(___ratelimit) },
	{ 0x78077b4f, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xf960fe12, __VMLINUX_SYMBOL_STR(v4l2_fh_open) },
	{ 0xb9e52429, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x73f6e1d3, __VMLINUX_SYMBOL_STR(vb2_ioctl_querybuf) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xb132fa2f, __VMLINUX_SYMBOL_STR(v4l2_ctrl_handler_init_class) },
	{ 0xb01a9715, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0x6fc7ee79, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0x67da748a, __VMLINUX_SYMBOL_STR(snd_card_free) },
	{ 0x9505497d, __VMLINUX_SYMBOL_STR(v4l2_i2c_new_subdev_board) },
	{ 0xc5d76a77, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0x34078dfb, __VMLINUX_SYMBOL_STR(snd_card_register) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xf0a38a07, __VMLINUX_SYMBOL_STR(snd_pcm_new) },
	{ 0xeca823a4, __VMLINUX_SYMBOL_STR(vmalloc_to_page) },
	{ 0x9805005a, __VMLINUX_SYMBOL_STR(vb2_ioctl_streamoff) },
	{ 0x9fb52b82, __VMLINUX_SYMBOL_STR(vb2_queue_release) },
	{ 0xec3d2e1b, __VMLINUX_SYMBOL_STR(trace_hardirqs_off) },
	{ 0x9c4d4885, __VMLINUX_SYMBOL_STR(v4l2_device_put) },
	{ 0x6dce2d14, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x1c483a9, __VMLINUX_SYMBOL_STR(v4l2_get_timestamp) },
	{ 0x89ffa9a9, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0x3e5b4008, __VMLINUX_SYMBOL_STR(video_ioctl2) },
	{ 0x6d72ab5e, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0xe6b83770, __VMLINUX_SYMBOL_STR(vb2_queue_init) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=videobuf2-v4l2,videodev,snd-pcm,videobuf2-vmalloc,videobuf2-core,snd,v4l2-common";

MODULE_ALIAS("usb:v1C88p0007d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Ed*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1C88p003Fd*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "4350E0F7CEB20A53B1504EB");
