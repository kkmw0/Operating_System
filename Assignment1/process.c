#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>

/* performs a depth-first traversal of the list of tasks in the system. */
void traverse_init(struct task_struct *ptr) {
/* fill in your code */
	struct task_struct *task;
	struct list_head *list;

	printk(KERN_INFO "Process Name : %-15s Process ID : %4d", ptr->comm, ptr->pid);
	list_for_each(list, &ptr->children){
		task = list_entry(list, struct task_struct, sibling);
		traverse_init(task);
	}
}

void traverse_exit(struct task_struct *ptr){
	struct task_struct *task;
	struct list_head *list;

	/*Print root*/
	printk(KERN_INFO "Process Name : %-15s Process ID : %4d", ptr->parent->comm, ptr->parent->pid);
	list_for_each(list, &ptr->parent->children){
		task = list_entry(list, struct task_struct, sibling);
		printk(KERN_INFO "Process Name : %-15s Process ID : %4d", task->comm, task->pid);
	}
	for_each_process(ptr){
		list_for_each(list, &ptr->children){
			task = list_entry(list, struct task_struct, sibling);
			printk(KERN_INFO "Process Name : %-15s Process ID : %4d", task->comm, task->pid);
		}
	}
}

int simple_init(void){
	printk(KERN_INFO "Loading Module\n");
	traverse_init(&init_task);

	return 0;
}

void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
	traverse_exit(&init_task);
}

module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Process Module");
MODULE_AUTHOR("SGG");

