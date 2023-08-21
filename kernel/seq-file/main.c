#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/fs.h>

static char* data[] = {
    "Line 1\n",
    "Line 2\n",
    "Line 3\n",
};

static void *my_seq_start(struct seq_file *m, loff_t *pos)
{
    if(*pos >= sizeof(data)/sizeof(char *))
        return NULL;
    return data[*pos];
}

static void *my_seq_next(struct seq_file *m, void *v, loff_t *pos)
{
    (*pos)++;
    if(*pos >= sizeof(data)/sizeof(char *))
        return NULL;
    return data[*pos];
}

static void my_seq_stop(struct seq_file *m, void *v)
{
    /* Nothing to do here */
}

static int my_seq_show(struct seq_file *m, void *v)
{
    seq_printf(m, "%s", (char *)v);

    return 0;
}

static const struct seq_operations my_seq_ops = {
        .start = my_seq_start,
        .next  = my_seq_next,
        .stop  = my_seq_stop,
        .show  = my_seq_show
};

static int my_seq_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &my_seq_ops);
}

// static const struct file_operations my_file_ops = {
//     .owner = THIS_MODULE,
//     .open = my_seq_open,
//     .read = seq_read,
//     .llseek = seq_lseek,
//     .release = seq_release,
// };

static struct proc_ops my_file_ops={
    .proc_open = my_seq_open,
    .proc_release = seq_release,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    // .proc_write = my_proc_write
};

int init_main(void)
{
    struct proc_dir_entry *entry;

    entry = proc_create("my_seq_file", 0, NULL, &my_file_ops);
    if (!entry) {
        pr_err("Failed to create /proc/my_seq_file\n");
        return -ENOMEM;
    }

    pr_info("my_seq_file module loaded\n");
    return 0;
}

void exit_main(void)
{
    remove_proc_entry("my_seq_file", NULL);
    pr_info("my_seq_file module unloaded\n");
}