#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>
//#include "task1.c"

#define DEFAULT_LOCK_PRIO 20

#define assert(x,error) if(!(x)){ \
            kprintf(error);\
            return;\
            }
/*int mystrncmp(char* des,char* target,int n){
    int i;
    for (i=0;i<n;i++){
        if (target[i] == '.') continue;
        if (des[i] != target[i]) return 1;
    }
    return 0;
}*/

/*--------------------------------Test 1--------------------------------*/
 
void rd1 (char *msg, int lck)
{
	int st;
	lock (lck, READ, DEFAULT_LOCK_PRIO);
	kprintf ("  %s: acquired lock, sleep 2s\n", msg);
	sleep (2);
	kprintf ("  %s: to release lock\n", msg);
	//releaseall (1, lck);

	ldelete(lck);
	kprintf(" %s: deleting the lock\n", msg);

	st = lock (lck, READ, DEFAULT_LOCK_PRIO);
	assert (st != SYSERR, "Test failed");

        kprintf ("  %s: acquired lock, sleep 2s\n", msg);
        sleep (2);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
	
	
}

void tcdel ()
{
	int	lck1, lck2;
	int	pid1;
	int	pid2;

	kprintf("\nTest 1: readers can share the rwlock\n");
	lck1  = lcreate ();
	assert (lck1 != SYSERR, "Test 1 failed");

	pid1 = create(rd1, 2000, 20, "reader a", 2, "reader a", lck1);
	//pid2 = create(rd1, 2000, 20, "reader b", 2, "reader b", lck1);
	
	resume(pid1);


	//ldelete(lck1);

	//`resume(pid2);	
	
	
	sleep (5);
	//ldelete (lck);
//	kprintf ("Test 1 ok\n");
}




