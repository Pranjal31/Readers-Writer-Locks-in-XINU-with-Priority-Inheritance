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


void readercp (char *msg, int lck)
{
        int     ret;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}

void writercp (char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}

void testcp ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readercp, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(readercp, 2000, 30, "reader3", 2, "reader B", lck);
        wr1 = create(writercp, 2000, 20, "writer3", 2, "writer", lck);

	kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

	kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
        resume(rd1);
        sleep (1);

	kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));
	assert (getprio(wr1) == 25, "Test 3 failed");

        kprintf("-start reader B, then sleep 1s. reader B(prio 30) blocked on the lock\n");
        resume (rd2);
	sleep (1);
        kprintf("------expected priority of wr1= 30 && actual prio of wr1 = %d\n",getprio(wr1));

	assert (getprio(wr1) == 30, "Test 3 failed");
	
	kprintf("-change prio of reader B to 15, then sleep 1s\n");
	chprio(rd2, 15);
	sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

	assert (getprio(wr1) == 25, "Test 3 failed");

	kprintf("-change prio of reader A to 10, then sleep 1s\n");
	chprio(rd1,10);
	sleep(1);
        kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(wr1));

	assert(getprio(wr1) == 20, "Test 3 failed"); 

        sleep (8);
        kprintf ("Test 3 OK\n");

}


