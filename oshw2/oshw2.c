#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h> // proc access
#include <linux/seq_file.h> //seq_file
#include <linux/string.h> // strcmp()
#include <linux/sched.h> // task_struct
#include <linux/kernel.h> // sprintf()

/*
Some referecne:
	seq_file : http://www.crashcourse.ca/introduction-linux-kernel-programming/lesson-12-adding-proc-files-your-modules-part-2
	passing argument to kernel module : http://www.tldp.org/LDP/lkmpg/2.6/html/x323.html
*/

commnad
static char *command = "\0";
static char *pid = "\0";
//parameter declaration
module_param(command, charp, 0000);
module_param(pid, charp, 0000);
//parameter descriptpor
MODULE_PARM_DESC(command, "command");
MODULE_PARM_DESC(pid, "pid");

//write something to seq_file :batoto
static int batoto_show(struct seq_file *m, void *v)
{
	//show all command operation
	if(strcmp(command, "showall") == 0){
		//all about task_struct is include from linux/sched.h
		struct task_struct *task_list;
		for_each_process(task_list) {
			seq_printf(m, "\n %s %d \n",task_list->comm,task_list->pid);
			//print proces name and pid 
		}
	}
	else if(strcmp(command, "parentchild") == 0){
		char a[10]; //use to present the task_list->pid (itoa)
		struct task_struct *task_list, *_prev_task_, *_next_task_;
		// the variable is the task double link_list ...
		for_each_process(task_list) {
			_prev_task_ = next_task(task_list);
			sprintf(a, "%d", _prev_task_->pid);  // itoa (include from linux/kernel.h)
			if(strcmp(a, pid) == 0) 
				break;
		}
		_next_task_ = next_task(_prev_task_);
		_prev_task_ = task_list;
		if (strcmp(a, pid) != 0)
		{
			seq_printf(m, "No this process.Please confirm again.\n");
		}
		else{
			seq_printf(m, "%s's parent : %s (%d)\n", pid, _prev_task_->comm, _prev_task_->pid);
			seq_printf(m, "%s's child : %s (%d)\n", pid, _next_task_->comm, _next_task_->pid);
		}
	}
	else {
		seq_printf(m, "No this command support.\n");
	}
    return 0;
}

// open the /proc/batoto 
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
// seq_file setting 

static int batoto_init(void)
{
    printk(KERN_INFO "Loading batoto module.\n");
    if(strcmp(command, "\0") != 0){
    	proc_create("batoto", 0, NULL, &batoto_fops);
    	// Create a "batoto" file , and its struct  file_operation setting is file_operation
    }
    else {
    	printk("No command.\n");
    	// print message that user can know whether the module load or not 
    }
    return 0;
}

static void batoto_exit(void)
{
    if(strcmp(command, "\0") != 0){
    	remove_proc_entry("batoto", NULL);
    	//remove the /proc/batoto   : Important do not forget.
	}
	printk(KERN_INFO "Unloading batoto module.\n");
}

module_init(batoto_init);
module_exit(batoto_exit);
// Just module setting

MODULE_LICENSE("GPL");
// GPL License