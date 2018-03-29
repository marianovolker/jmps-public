

/* Summary:
	always keep an eye on platform-specific implementations. It is common to deal with partial implementations. Make your way with the help of man.
	what to #include: semaphore.h
	types:
		sem_t: a semaphore descriptor. Commonly passed by reference.
	functions for activating unnamed semaphores:
		int sem_init(sem_t *sem, int shared, unsigned int value)
			the area pointed by sem is initialized with a new semaphore object. shared tells if the semaphore is local (0) or 
			shared between several processes (non-0). This isn't vastly supported.
			value is the number the semaphore is initialized with.
			Returns -1 if unsuccessful.
		int sem_destroy(sem_t *sem)
			deallocates memory associated with the semaphore pointed by sem.
			Returns 0 if successful, -1 otherwise.

	functions for activating named semaphores:
		sem_t sem_open(const char *name, int flags, ...)
			", ..." is zero or one occurrence of "mode_t mode, unsigned int value" (see below).
			opens a named semaphore. name specify its location in the filesystem hierarchy. flags can be zero, or O_CREAT 				(possibly associated with O_EXCL). Non-zero means creating the semaphore if it doesn't already exist. O_CREAT 
			requires two further parameters: mode specifying the permission set of the semaphore in the filesystem, value being 
			the same as in sem_destroy(). O_CREAT causes an exclusive behaviour if it appears with O_EXCL: a failure is 	
			returned if a semaphore name already exists in this case. Further values have been designed, while rarely 
			implemented: O_TRUNC for truncating if such semaphore already exists, and O_NONBLOCK for non-blocking mode.
			sem_open returns the address of the semaphore if successful, SEM_FAILED otherwise.
		int sem_close(sem_t *sem)
			closes the semaphore pointed by sem. Returns 0 on success, -1 on failure.
		int sem_unlink(const char name *name)
			removes the semaphore name from the filesystem. Returns 0 on success, -1 on failure.

	functions for working with open semaphores:
		int sem_wait(sem_t *sem)
			waits until the semaphore sem is non-locked, then locks it and returns to the caller. Even without getting the 
			semaphore unlocked, sem_wait may be interrupted with the occurrence of a signal.
			returns 0 on success, -1 on failure. If -1, sem is not modified by the function.
		int sem_trywait(sem_t *sem)
			tries to lock sem the same way sem_wait does, but doesn't hang if the semaphore is locked.
			Returns 0 on success (got semaphore), EAGAIN if the semaphore was locked.
		int sem_post(sem_t *sem)
			unlocks sem. Of course, this has to be atomic and non-reentrant. After this operation, some thread between those 				waiting for the semaphore to get free. Otherwise, the semaphore value simply steps up by 1.
			Returns 0 on success, -1 on failure.
		int sem_getvalue(sem_t *sem, int *restrict sval)
			compiles sval with the current value of sem. sem may possibly change its value before sem_getvalue returns to the 
			caller.
			Returns 0 on success, -1 on failure.
*/

/*
 *  semaphore.c
 *  
 *
 *  Created by Mij <mij@bitchx.it> on 12/03/05.
 *  Original source file available at http://mij.oltrelinux.com/devel/unixprg/


    Unnamed semaphores.
    One process; the main thread creates 2 threads. They access their critical section with mutual exclusion. The first one synchronously, 
    the second one asynchronously.
 *
 */


#define _POSIX_SOURCE
#include <stdio.h>
/* sleep() */
#include <errno.h>
#include <unistd.h>
/* abort() and random stuff */
#include <stdlib.h>
/* time() */
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>


/* this skips program termination when receiving signals */
void signal_handler(int type);

/*
 * thread A is synchronous. When it needs to enter its
 * critical section, it can't do anything other than waiting.
 */
void *thread_a(void *);

/*
 * thread B is asynchronous. When it tries to enter its
 * critical section, it switches back to other tasks if
 * it hasn't this availability.
 */
void *thread_b(void *);


/* the semaphore */
sem_t mysem;

int main(int argc, char *argv[])
{
    pthread_t mytr_a, mytr_b;
    int ret;
    
    
    srand(time(NULL));
    signal(SIGHUP, signal_handler);
    signal(SIGUSR1, signal_handler);

    /*
     * creating the unnamed, local semaphore, and initialize it with
     * value 1 (max concurrency 1)
     */
    ret = sem_init(&mysem, 0, 1);
    if (ret != 0) 
    {
        /* error. errno has been set */
        perror("Unable to initialize the semaphore");
        abort();
    }
    
    /* creating the first thread (A) */
    ret = pthread_create(&mytr_a, NULL, thread_a, NULL);
    if (ret != 0)
    {
        perror("Unable to create thread");
        abort();
    }

    /* creating the second thread (B) */
    ret = pthread_create(&mytr_b, NULL, thread_b, NULL);
    if (ret != 0) 
    {
        perror("Unable to create thread");
        abort();
    }
    
    /* waiting for thread_a to finish */
    ret = pthread_join(mytr_a, (void *)NULL);
    if (ret != 0) 
    {
        perror("Error in pthread_join");
        abort();
    }

    /* waiting for thread_b to finish */
    ret = pthread_join(mytr_b, NULL);
    if (ret != 0) 
    {
        perror("Error in pthread_join");
        abort();
    }

    return 0;
}


void *thread_a(void *x)
{
    unsigned int i, num;
    int ret;
    
    
    printf(" -- thread A -- starting\n");
    num = ((unsigned int)rand() % 40);
    
    /* this does (do_normal_stuff, do_critical_stuff) n times */
    for (i = 0; i < num; i++) 
    {
        /* do normal stuff */
        sleep(3 + (rand() % 5));
        
        /* need to enter critical section */
        printf(" -- thread A -- waiting to enter critical section\n");

        /* looping until the lock is acquired */
        do 
        {
            ret = sem_wait(&mysem);
            if (ret != 0) 
            {
                /* the lock wasn't acquired */
                if (errno != EINVAL)
                {
                    perror(" -- thread A -- Error in sem_wait. terminating -> ");
                    pthread_exit(NULL);
                }
                else
                {
                    /* sem_wait() has been interrupted by a signal: looping again */
                    printf(" -- thread A -- sem_wait interrupted. Trying again for the lock...\n");
                }
            }
        } while (ret != 0);

        printf(" -- thread A -- lock acquired. Enter critical section\n");
        
        /* CRITICAL SECTION */
        sleep(rand() % 2);
        
        /* done, now unlocking the semaphore */
        printf(" -- thread A -- leaving critical section\n");

        ret = sem_post(&mysem);
        if (ret != 0) 
        {
            perror(" -- thread A -- Error in sem_post");
            pthread_exit(NULL);
        }
    }
    
    printf(" -- thread A -- closing up\n");

    pthread_exit(NULL);
}


void *thread_b(void *x)
{
    unsigned int i, num;
    int ret;
    
    printf(" -- thread B -- starting\n");
    num = ((unsigned int)rand() % 100);

    
    /* this does (do_normal_stuff, do_critical_stuff) n times */
    for (i = 0; i < num; i++) 
    {
        /* do normal stuff */
        sleep(3 + (rand() % 5));
        
        /* wants to enter the critical section */
        ret = sem_trywait(&mysem);
        if (ret != 0)
        {
            /* either an error happened, or the semaphore is locked */
            if (errno != EAGAIN)
            {
                /* an event different from "the semaphore was locked" happened */
                perror(" -- thread B -- error in sem_trywait. terminating -> ");
                pthread_exit(NULL);
            }
            
            printf(" -- thread B -- cannot enter critical section: semaphore locked\n");
        }
        else
        {
            /* CRITICAL SECTION */
            printf(" -- thread B -- enter critical section\n");

            sleep(rand() % 10);
            
            /* done, now unlocking the semaphore */
            printf(" -- thread B -- leaving critical section\n");

            sem_post(&mysem);
        }
    }
    
    printf(" -- thread B -- closing up\n");
    
    /* joining main() */
    pthread_exit(NULL);
}


void signal_handler(int type)
{
    /* do nothing */
    printf("process got signal %d\n", type);
    return;
}
