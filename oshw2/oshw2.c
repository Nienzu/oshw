#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>


static char *mystring = "\0";
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

int read_proc(char *buf,char **start,off_t offset,int count,int *eof,void *data )
{
	int len=0;
	struct task_struct *task_list;
	for_each_process(task_list) {
		len  += sprintf(buf+len, "\n %s %d \n",task_list->comm,task_list->pid);
	}
	return len;
}

void create_new_proc_entry()
{	if(mystring == "showall"){
		create_proc_read_entry("ps_list",0,NULL,read_proc,NULL);
	}
	else if(mystring == "parentchild"){

	}
	else if(mystring == "\0"){

	}
}


int functn_init (void) {
	int ret;
	if(mystring == "showall"){
		create_new_proc_entry();
	}
	else{
		
	}
	return 0;
}

void functn_cleanup(void) {
	remove_proc_entry("ps_list",NULL);
}

module_init(functn_init);
module_exit(functn_cleanup);