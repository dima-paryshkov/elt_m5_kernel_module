#include <linux/kernel.h>  /* We're doing kernel work */
#include <linux/module.h>  /* Specifically, a module */
#include <linux/proc_fs.h> /* Necessary because we use the proc fs */
#include <linux/uaccess.h> /* for copy_from_user */ 
#include <linux/version.h>
#include <linux/slab.h>    /* for kmalloc */ 

#define PROCFS_MAX_SIZE 128

/* proc/<PROCFS_NAME>*/
#define PROCFS_NAME "hello"

/* This structure hold information about the /proc file */
static struct proc_dir_entry *our_proc_file;

/* The buffer used to store character for this module */
static char *procfile_buff;

/* The size of the buffer */
static int procfile_buff_size = 0, tmp;

/* This function is called then the /proc file is read */
static ssize_t procfile_read(struct file *filePointer, char __user *buffer, size_t buffer_length, loff_t *offset)
{
    if (buffer_length > procfile_buff_size)
        buffer_length = procfile_buff_size;

    copy_to_user(buffer, procfile_buff, buffer_length);
    procfile_buff_size -= buffer_length;

    return buffer_length;
}

/* This function is called with the /proc file is written. */
static ssize_t procfile_write(struct file *file, const char __user *buff, size_t len, loff_t *off)
{
    procfile_buff_size = len;
    if (procfile_buff_size > PROCFS_MAX_SIZE)
        procfile_buff_size = PROCFS_MAX_SIZE;

    if (copy_from_user(procfile_buff, buff, procfile_buff_size))
        return -EFAULT;

    procfile_buff[procfile_buff_size & (PROCFS_MAX_SIZE - 1)] = '\0';
    tmp = procfile_buff_size;
    // pr_info("procfile write %s\n", procfile_buff);

    return procfile_buff_size;
}

/* The read and write functions for the proc entry */
static const struct file_operations proc_file_fops = {
    .read = procfile_read,
    .write = procfile_write,
};

/* Create a proc entry using the function */
static int create_proc(void)
{
    our_proc_file = proc_create(PROCFS_NAME, 0666, NULL, &proc_file_fops);
    if (our_proc_file == NULL)
    {
        proc_remove(our_proc_file);
        pr_alert("Error: Could not initialize /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }

    procfile_buff = kmalloc(PROCFS_MAX_SIZE * sizeof(char), GFP_KERNEL);
    if (procfile_buff == NULL)
    {
        proc_remove(our_proc_file);
        pr_alert("Error: Could not alloc memory (kmalloc)\n");
        return -ENOMEM;
    }

    return 0;
}

/* Init function */
static int __init procfile_init(void)
{
    int exit_code = create_proc();
    if (exit_code == 0)
    {
        pr_info("/proc/%s created\n", PROCFS_NAME);
        return 0;
    }
    else
        return exit_code;

    return 0;
}

/* Clean up function */
static void __exit procfile_exit(void)
{
    proc_remove(our_proc_file);
    pr_info("/proc/%s removed\n", PROCFS_NAME);
    kfree(procfile_buff);
}

module_init(procfile_init);
module_exit(procfile_exit);

MODULE_LICENSE("GPL");