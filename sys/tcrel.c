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
 
void rr1 (char *msg, int lck1, int lck2, int lck3)
{
	int i ,st;
	lock (lck1, READ, DEFAULT_LOCK_PRIO);
	kprintf ("  %s: acquired lock 1, sleep 2s\n", msg);
	sleep (2);
//	kprintf ("  %s: to release lock\n", msg);
	//releaseall (1, lck);

//	ldelete(lck);
//	kprintf(" %s: deleting the lock\n", msg);
//
	lock (lck2, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock 2, sleep 2s\n", msg);
        sleep (2);

	//st = lock (lck, READ, DEFAULT_LOCK_PRIO);
	//assert (st != SYSERR, "Test failed");

        //kprintf ("  %s: acquired lock, sleep 2s\n", msg);
        //sleep (2);
        kprintf ("  %s: to release both locks\n", msg);
	 for(i=0;i<NPROC; i++)
                kprintf("%d\n",proctab[currpid].plocksheld[i]);
	//releaseall(2,lck1,lck2);     
	 st = releaseall (3, lck1, lck2, lck3);

        assert (st != SYSERR, "Test 1 failed*********");
	kprintf("\n\n\n\n");
	for(i=0;i<NPROC; i++)
		kprintf("%d\n",proctab[currpid].plocksheld[i]); 
	
	
}

void tcrel ()
{
	int	lck1, lck2,lck3;
	int	pid1;
	int	pid2;

	kprintf("\nTest 1: readers can share the rwlock\n");
	lck1  = lcreate ();
	assert (lck1 != SYSERR, "Test 1 failed");
        lck2  = lcreate ();
        assert (lck2 != SYSERR, "Test 1 failed");
	lck3  = lcreate ();
        assert (lck3 != SYSERR, "Test 1 failed");


	pid1 = create(rr1, 2000, 20, "reader a", 4, "reader a", lck1, lck2,lck3);
	//pid2 = create(rd1, 2000, 20, "reader b", 2, "reader b", lck1);
	
	resume(pid1);


	//ldelete(lck1);

	//`resume(pid2);	
	
	
	sleep (5);
	//ldelete (lck);
//	kprintf ("Test 1 ok\n");
}




