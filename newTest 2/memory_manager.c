#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/rmap.h>


// TODO: 1
// Define your input parameter (pid - int)
static int pid = -1;
module_param(pid, int, 0);
MODULE_PARM_DESC(pid, "Process ID");

struct task_struct *task = NULL;    // Pointer to the task strucker 
// Initialize memory statistics variables
unsigned long total_rss = 0;
unsigned long total_swap = 0;
unsigned long total_wss = 0;


// TODO: 2
// Function to parse the virtual memory areas (VMAs)
static void parse_vma(void)
{
    struct mm_struct *mm = NULL;    // Pointer to the memory management structure
    struct vm_area_struct *vma;     // Pointer to the cirtuyal memory area structure
    unsigned long page;             // Variable to iterate over pages
    pgd_t *pgd;                     // Page global directory
    p4d_t *p4d;                     // Page 4 directory
    pud_t *pud;                     // Page upper directory
    pmd_t *pmd;                     // Page middle directory
    pte_t *ptep, pte;               // Page table entry pointer and entry


// TODO: 2, 3, 4, 5, 6, 7, 8
// Check if the PID is valid
    if (pid > 0) {
        task = pid_task(find_vpid(pid), PIDTYPE_PID);       // Get the task_struct for the given PID
        if (task && task->mm) {                             // Check if the task and its mm_struct are valid
            mm = task->mm;
            mmap_read_lock(mm);  // Acquire mmap lock for reading

            // Iterate over all VMAs using find_vma
            for (vma = find_vma(mm, 0); vma; vma = find_vma(mm, vma->vm_end)) {
                // Iterate through each page of the VMA
                for (page = vma->vm_start; page < vma->vm_end; page += PAGE_SIZE) {
                    pgd = pgd_offset(mm, page);
                    if (pgd_none(*pgd) || pgd_bad(*pgd))
                        continue;

                    p4d = p4d_offset(pgd, page);
                    if (p4d_none(*p4d) || p4d_bad(*p4d))
                        continue;

                    pud = pud_offset(p4d, page);
                    if (pud_none(*pud) || pud_bad(*pud))
                        continue;

                    pmd = pmd_offset(pud, page);
                    if (pmd_none(*pmd) || pmd_bad(*pmd))
                        continue;

                    ptep = pte_offset_map(pmd, page);
                    if (!ptep)
                        continue;

                    pte = *ptep;

                    // Check if the page table entry is valid
                    if (pte_none(pte))
                        continue;

                    // Check if the page is in memory, otherwise it is in swap
                    if (pte_present(pte)) {
                        total_rss++;

                        // Check if the page is actively used
                        if (pte_young(pte)) {
                            total_wss++;    // Increase WSS count
                            test_and_clear_bit(_PAGE_BIT_ACCESSED, (unsigned long *)ptep);
                        }
                    } else {
                        total_swap++;       // Increae swap count
                    }
                    pte_unmap(ptep);        // Unmap the page table entry
                }
            }

            mmap_read_unlock(mm);  // Release mmap lock
        }
    }
}

// Timer interval in nanoseconds 
unsigned long timer_interval_ns = 10e9; // 10 sec timer
static struct hrtimer hr_timer;         // High-resolution timer


// Timer callback function
enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart)
{
    ktime_t currtime, interval;
    currtime = ktime_get();                                     // get the current time
    interval = ktime_set(0, timer_interval_ns);                 // Set the timer interval
    hrtimer_forward(timer_for_restart, currtime, interval);     // Forward the timer

    // reset RSS, swap, WSS
    total_rss = 0;
    total_swap = 0;
    total_wss = 0;
    parse_vma();    // Parse the VMAs

    // Print the results
    printk("[PID-%i]:[RSS:%lu MB] [Swap:%lu MB] [WSS:%lu MB]\n", pid, total_rss * 4 / 1024, total_swap * 4 / 1024, total_wss * 4 / 1024);

    // Restart the timer
    return HRTIMER_RESTART;
}


// Module initialization function
int memory_init(void)
{
    // Timer structure
    ktime_t ktime;                  // Move the declaration to the beginning of the block

    printk("CSE330 Project Kernel Module Inserted\n");
    ktime = ktime_set(0, timer_interval_ns);                       // Set the timer interval
    hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);    // Initialize the timer
    hr_timer.function = &timer_callback;                           // Set the timer callback function
    hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);             // Start the timer
    return 0;
}


// Module cleanup function
void memory_cleanup(void)
{
    int ret;
    ret = hrtimer_cancel(&hr_timer);    // Cancel the timer
    if (ret)
        printk("HR Timer cancelled ...\n");
    printk("CSE330 Project 2 Kernel Module Removed\n");
}


// Register the module initialization and cleanup functions
module_init(memory_init);
module_exit(memory_cleanup);

MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raejae Soiha");
MODULE_DESCRIPTION("CSE330 Project Memory Management\n");
