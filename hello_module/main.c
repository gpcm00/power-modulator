#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/fs.h>

// static int modmajor = 0;
// static int modminor = 0;

MODULE_AUTHOR("gpm");
MODULE_LICENSE("Dual BSD/GPL");

static int __init mod_init(void)
{
    printk("Hello world!\n");
    return 0;
}

static void __exit mod_exit(void)
{
    printk("Good bye world!\n");
}

module_init(mod_init);
module_exit(mod_exit);
