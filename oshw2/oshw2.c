#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/sched.h>

static int batoto_show(struct seq_file *m, void *v)
{

	struct task_struct *task_list;
	for_each_process(task_list) {
		seq_printf(m, "\n %s %d \n",task_list->comm,task_list->pid);
	}
    return 0;
}

static int batoto_open(struct inode *inode, struct file *file)
{
    return single_open(file, batoto_show, NULL);
}

static const struct file_operations batoto_fops = {
    .owner      = THIS_MODULE,
    .open       = batoto_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static int batoto_init(void)
{
    printk(KERN_INFO "Loading hz module, BATOTO = %d.\n", BATOTO);
    proc_create("batoto", 0, NULL, &batoto_fops);
    return 0;
}

static void batoto_exit(void)
{
    remove_proc_entry("batoto", NULL);
    printk(KERN_INFO "Unloading batoto module.\n");
}

module_init(batoto_init);
module_exit(batoto_exit);

