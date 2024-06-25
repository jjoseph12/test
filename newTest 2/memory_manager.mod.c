#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x2d0684a9, "hrtimer_init" },
	{ 0x3c5d543a, "hrtimer_start_range_ns" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x46a4b118, "hrtimer_cancel" },
	{ 0xb43f9365, "ktime_get" },
	{ 0x828e22f4, "hrtimer_forward" },
	{ 0xa250e321, "find_vpid" },
	{ 0x5a865367, "pid_task" },
	{ 0xe6d37819, "__tracepoint_mmap_lock_start_locking" },
	{ 0x668b19a1, "down_read" },
	{ 0xed9c9040, "__tracepoint_mmap_lock_acquire_returned" },
	{ 0xae7d3c8f, "find_vma" },
	{ 0x72d79d83, "pgdir_shift" },
	{ 0xd1d6e0b7, "boot_cpu_data" },
	{ 0x509fb967, "__tracepoint_mmap_lock_released" },
	{ 0x53b954a2, "up_read" },
	{ 0x1d19f77b, "physical_mask" },
	{ 0x8a35b432, "sme_me_mask" },
	{ 0xdad13544, "ptrs_per_p4d" },
	{ 0x75176672, "pv_ops" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xc978473c, "__mmap_lock_do_trace_acquire_returned" },
	{ 0xe7793a5f, "__mmap_lock_do_trace_start_locking" },
	{ 0xd0f1f671, "__mmap_lock_do_trace_released" },
	{ 0xa648e561, "__ubsan_handle_shift_out_of_bounds" },
	{ 0xbfc40be2, "param_ops_int" },
	{ 0x2b18de38, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "06ED12C838664CF8CB0FCE7");
