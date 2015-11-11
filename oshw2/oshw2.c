#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/stdio.h>

static char *command = "\0";
static char *pid = "\0";
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "command");
MODULE_PARM_DESC(pid, "pid");

static int batoto_show(struct seq_file *m, void *v)
{
	if(strcmp(comment, "showall") == 0){
		struct task_struct *task_list;
		for_each_process(task_list) {
			seq_printf(m, "\n %s %d \n",task_list->comm,task_list->pid);
		}
	}
	else if(strcmp(command, "parentchild") == 0){
		char a[10];
		struct task_struct *task_list, *_prev_task_, *_next_task_;
		for_each_process(task_list) {
			sprintf(a, "%d", task_list->pid);
			if(strcmp(a, pid) == 0) 
				break;
		}
		_prev_task_ = task_list->prev_task;
		_next_task_ = task_list->next_task;
		seq_printf("%s's parent : %s (%d)", pid, _prev_task_->prev_task, _prev_task_->pid);
		seq_printf("%s's child : %s (%d)", pid, _next_task_->comm, _next_task_->pid);
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
    printk(KERN_INFO "Loading batoto module.\n");
    if(strcmp(command, "\0") != 0){
    	proc_create("batoto", 0, NULL, &batoto_fops);
    }
    else {
    	printk("No comment reqiured\n");
    }
    return 0;
}

static void batoto_exit(void)
{
    if(strcmp(command, "\0") != 0){
    	remove_proc_entry("batoto", NULL);
	}
	printk(KERN_INFO "Unloading batoto module.\n");
}

module_init(batoto_init);
module_exit(batoto_exit);

