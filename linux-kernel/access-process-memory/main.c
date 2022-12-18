/**
 * @file main.c
 * @author liuqi (liuqi1@kylinos.cn)
 * @brief 
 * @version 0.1
 * @date 2022-12-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>

static int pid = 1;
module_param(pid, int, 0644);

/**
 * @brief walk through process memory of a mmap.
 * 
 * @param task 
 * @param mmap 
 * @param private 
 * @param func 
 * @return int 
 */
int walk_process_vm(struct task_struct *task, struct vm_area_struct *mmap,
			void *private, int (*func)(struct task_struct *, char *, unsigned long, void *))
{
	int rc;
	int size;
	unsigned long vm_start = mmap->vm_start;
	unsigned long length = mmap->vm_end - mmap->vm_start;
	char *buffer;

	buffer = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	if(IS_ERR(buffer))
		return PTR_ERR(buffer);

	while(length>0){
		size = length > PAGE_SIZE ? PAGE_SIZE : length;
		rc = access_process_vm(task, vm_start, buffer, size, FOLL_REMOTE);
		if(IS_ERR_VALUE(rc)){
			pr_err("access process vm failed at %p (len:0x%x)", (void *)vm_start, size);
			return rc;
		}
		// do something with buffer...
		if(func){
			rc = func(task, buffer, size, private);
			if(IS_ERR_VALUE(rc)){
				pr_err("callback func return failed at %p (len:0x%x)", (void *)vm_start, size);
				return rc;
			}
		}
		length -= size;
		vm_start += size;
	}
	return 0;
}

/**
 * @brief walk through process mmaps.
 * 
 * @param vpid 
 * @param func 
 * @return int 
 */
int walk_process_mmaps(int vpid, int (*func)(struct task_struct *, struct vm_area_struct *))
{
	struct task_struct *task;
	struct mm_struct *mm;
	struct vm_area_struct *mmap;
	
	task = get_pid_task(find_get_pid(vpid), PIDTYPE_PID);
	if (IS_ERR(task))
		return PTR_ERR(task);

	mm = get_task_mm(task);
	if(IS_ERR(mm))
		return PTR_ERR(mm);

	for(mmap = mm->mmap; mmap != NULL; mmap = mmap->vm_next){
		if(func){
			int rc;
			rc = func(task, mmap);
			if(IS_ERR_VALUE(rc)){
				pr_err("callback func return failed at mmap :%p", mmap);
				return rc;
			}
		}
	}
	
	return 0;
}


// example functions
static int example_access_vm_cb(struct task_struct *task, char *buffer, unsigned long size, void * private)
{
	print_hex_dump(KERN_DEBUG, "", DUMP_PREFIX_OFFSET, 16, 4, buffer, size, true);
	return 0;
}

static int example_access_mmap_cb(struct task_struct *task, struct vm_area_struct *mmap)
{
	unsigned int length;

	// only code segment
	if(!(mmap->vm_flags & VM_EXEC))
		return 0;

	// must have backing file (in case [vdso] segment)
	if(mmap->vm_file == NULL || mmap->vm_file->f_path.dentry == NULL)
		return 0;

	// pr_debug("path: %s, start: %lx, end: %lx, pgoff: %lx, flags: %lx, length:%lx", \
	// 	mmap->vm_file->f_path.dentry->d_iname,
	// 	mmap->vm_start, mmap->vm_end, mmap->vm_pgoff, \
	// 	mmap->vm_flags, length );

	return walk_process_vm(task, mmap, NULL, example_access_vm_cb);
}

int example_access_process_memory(int vpid)
{
	return walk_process_mmaps(vpid, example_access_mmap_cb);
}


int __init init_main(void)
{
	int error = 0;
	pr_info("Hello module!\n");

	example_access_process_memory(pid);

	return error;
}


void __exit exit_main(void)
{
	pr_info("Goodbye module!\n");
}


module_init(init_main);
module_exit(exit_main);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liuqi");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("access_process_memory example");
