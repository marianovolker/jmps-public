/*
    Summary:
        what to #include: sys/types.h , sys/mman.h , possibly fcntl.h for O_* macros
    functions:
        int shm_open(const char *name, int flags, mode_t mode)
            open (or create) a shared memory object with the given POSIX name. The flags argument instructs on how to open the object: the 
            most  relevant ones are O_RDONLY xor O_RDWR for access type, and O_CREAT for create if object doesn't exist. mode is only used 
            when the object is to be created, and specifies its access permission (umask style).
            Returns a file descriptor if successful, otherwise -1.

        void *mmap(void *start_addr, size_t len, int protection, int flags, int fd, off_t offset)
            maps the file fd into memory, starting from offset for a segment long len. len is preferably a multiple of the system page  
            size; the actual size alloced is the smallest number of pages needed for containing the length requested. Notably, if the file 
            is shorter than offset + len, the orphaned portion is still available for accessing, but what's written to it is not 
            persistent. protection specifies the rules for accessing the segment: PROT_READ , PROT_WRITE, PROT_EXEC to be ORed 
            appropriately. flags sets some details on how the segment will be mapped, the most relevant ones being private (MAP_PRIVATE) or 
            shared (MAP_SHARED, default) and, on the systems supporting it, if it is an anonymous segment (MAP_ANON), in which case fd is 
            good to be -1. fd is simply the file descriptor as returned by shm_open.

        int shm_unlink(const char *name)
            removes a shared memory object specified by name. If some process is still using the shared memory segment associated to name, 
            the segment is not removed until all references to it have been closed. Commonly, shm_unlink is called right after a successful 
            shm_create, in order to assure the memory object will be freed as soon as it's no longer used. 
            anonymous mapping is useful when the persistence of the shared segment isn't requested, but like all unnamed mechanisms it only 
            works between related processes. Two methods are available throughout the different OSes: 
            BSD4.4 anonymous memory mapping: the mmap function is passed -1 as fd and an ored MAP_ANON in flags. The offset argument is 
            ignored. The filesystem isn't touched at all.
            System V anonymous mapping: the /dev/zero file is opened to be passed to mmap. Then, the segment content is initially set to 0, 
            and everything written to it is lost. This machanism also works with systems supporting the BSD way, of course.
*/


/*
 *  shm_anon_bsd.c
 *  
 *  Anonymous shared memory via BSD's MAP_ANON.
 *  Create a semaphore, create an anonymous memory segment with the MAP_ANON
 *  BSD flag and loop updating the segment content (increment casually) with
 *  short intervals.
 *
 *
 *  Created by Mij <mij@bitchx.it> on 29/08/05.
 *  Original source file available at http://mij.oltrelinux.com/devel/unixprg/
 *
 */

#include <stdio.h>
/* for shm_* and mmap() */
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
/* for getpid() */
#include <unistd.h>
/* exit() */
#include <stdlib.h>
/* for sem_* functions */
#include <sys/stat.h>
#include <semaphore.h>
/* for seeding time() */
#include <time.h>

/* name of the semaphore */
#define     SEMOBJ_NAME         "/semshm"

/* maximum number of seconds to sleep between each loop operation */
#define     MAX_SLEEP_SECS      3

/* maximum value to increment the counter by */
#define     MAX_INC_VALUE       10 


int main(int argc, char *argv[]) 
{
    int shared_seg_size = 2 * sizeof(int);
    int *shared_values;     /* this will be a (shared) array of 2 elements */
    sem_t *sem_shmsegment;  /* semaphore controlling access to the shared segment */
    pid_t mypid;
    
    
    /* getting a new semaphore for the shared segment       -- sem_open()   */
    sem_shmsegment = sem_open(SEMOBJ_NAME, O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, 1);
    if (sem_shmsegment == SEM_FAILED)
    {
        perror("In sem_open()");
        exit(1);
    }

    /* requesting the semaphore not to be held when completely unreferenced */
    sem_unlink(SEMOBJ_NAME);
    
    /* requesting the shared segment    --  mmap() */    
    shared_values = (int *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if ((int)shared_values == -1)
    {
        perror("In mmap()");
        exit(1);
    }


    fprintf(stderr, "Shared memory segment allocated correctly (%d bytes) at %u.\n", shared_seg_size, (unsigned int)shared_values);

    /* dupping the process */
    if (! fork() )
    {
        /* the child waits 2 seconds for better seeding srandom() */
        sleep(2);
    }
    
    /* seeding the random number generator (% x for better seeding when child executes close) */
    srandom(time(NULL));

    /* getting my pid, and introducing myself */
    mypid = getpid();
    printf("My pid is %d\n", mypid);

    /*
       main loop:
        - pause
        - print the old value
        - choose (and store) a random quantity
        - increment the segment by that
    */
    do 
    {
        sleep(random() % (MAX_SLEEP_SECS+1));       /* pausing for at most MAX_SLEEP_SECS seconds */
        
        sem_wait(sem_shmsegment);
        /* entered the critical region */
        
        printf("process %d. Former value %d.", mypid, shared_values[0]);
        
        shared_values[1] = random() % (MAX_INC_VALUE+1);            /* choose a random value up to MAX_INC_VALUE */
        shared_values[0] += shared_values[1];   /* and increment the first cell by this value */
        
        printf(" Incrementing by %d.\n", shared_values[1]);

        /* leaving the critical region */
        sem_post(sem_shmsegment);
    } while (1);
    
    /* freeing the reference to the semaphore */
    sem_close(sem_shmsegment);

    
    return 0;
}
