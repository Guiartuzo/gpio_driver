#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/proc_fs.h>
#include <linux/slab.h>

#define LLL_MAX_USER_SIZE 1024

static struct proc_dir_entry *lll_proc = NULL;
static char data_buffer[LLL_MAX_USER_SIZE];

ssize_t lll_read(struct file *file, char __user *user, size_t size, loff_t *off)
{
	const char *msg = "Hello!\n";
	size_t len = strlen(msg);

	if (*off >= len) {
		return 0;
	}

	if (size > len - *off) {
		size = len - *off;
	}

	if (copy_to_user(user, msg + *off, size)) {
		return -EFAULT;
	}
	
	*off += size;
	return size;
}

ssize_t lll_write(struct file *file, const char __user *user, size_t size, loff_t *off)
{
	if (size > LLL_MAX_USER_SIZE - 1) {
		size = LLL_MAX_USER_SIZE - 1;
	}

	memset(data_buffer, 0x0, sizeof(data_buffer));

	if (copy_from_user(data_buffer, user, size)) {
		return -EFAULT;
	}

	// Ensure the buffer is null terminated
	data_buffer[size - 1] = '\0';

	printk("You said '%s'!\n", data_buffer);
	return size;
}

static const struct proc_ops lll_proc_fops = {
	.proc_read = lll_read,
	.proc_write = lll_write,
};

static int __init gpio_driver_init(void)
{
	printk("Welcome to my driver!\n");

	// create an entry in the proc-fs
	lll_proc = proc_create("lll_gpio", 0666, NULL, &lll_proc_fops);
	if (lll_proc == NULL)
	{
		return -ENOMEM;
	}

	return 0;
}

static void __exit gpio_driver_exit(void)
{
	printk("Leaving my driver!\n");

	proc_remove(lll_proc);
	return;
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guilherme Martim");
MODULE_DESCRIPTION("Test of writing drivers for Rasp zero 2w");
MODULE_VERSION("1.0");
