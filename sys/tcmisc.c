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


void readerm (char *msg, int lck)
{
        int     ret;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}

void writerm (char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}

void testmisc2 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readerm, 2000, 5, "reader3", 2, "reader A", lck);
        rd2 = create(readerm, 2000, 5, "reader3", 2, "reader B", lck);
        wr1 = create(writerm, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A(prio 5) blocked on the lock\n");
        resume(rd1);
        sleep (1);

        kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(wr1));
        assert (getprio(wr1) == 20, "Test 3 failed");


        sleep (8);
        kprintf ("Test 3 OK\n");

}



void testmisc3 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readerm, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(readerm, 2000, 5, "reader3", 2, "reader B", lck);
        wr1 = create(writerm, 2000, 20, "writer3", 2, "writer", lck);

	kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

	kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
        resume(rd1);
        sleep (1);

	kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));
	assert (getprio(wr1) == 25, "Test 3 failed");

	

        kprintf("-start reader B, then sleep 1s. reader B(prio 5) blocked on the lock\n");
        resume (rd2);
	sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

	assert (getprio(wr1) == 25, "Test 3 failed");
	
	
	kprintf("---kill reader A (prio 25)\n");
	kill(rd1);
	sleep (1);
        kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(wr1));

	assert (getprio(wr1) == 20, "Test 3 failed");

/*	kprintf("-change prio of reader A to 10, then sleep 1s\n");
	chprio(rd1,10);
	sleep(1);
        kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(wr1));

	assert(getprio(wr1) == 20, "Test 3 failed"); 
*/


        sleep (8);
        kprintf ("Test 3 OK\n");  

}

void testmisc4 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readerm, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(readerm, 2000, 5, "reader3", 2, "reader B", lck);
        wr1 = create(writerm, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
        resume(rd1);
        sleep (1);

        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));
        assert (getprio(wr1) == 25, "Test 3 failed");



        kprintf("-start reader B, then sleep 1s. reader B(prio 5) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("---kill reader B (prio 5)\n");
        kill(rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");

       sleep (8);
        kprintf ("Test 3 OK\n");

}

void testmisc5 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readerm, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(readerm, 2000, 5, "reader3", 2, "reader B", lck);
        wr1 = create(writerm, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
        resume(rd1);
        sleep (1);

        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));
        assert (getprio(wr1) == 25, "Test 3 failed");



        kprintf("-start reader B, then sleep 1s. reader B(prio 5) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("---kill reader B (prio 5)\n");
        kill(rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");

	
	kprintf("---kill reader A (prio 25)\n");
        kill(rd1);
        sleep (1);
        kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 20, "Test 3 failed");

       sleep (8);
        kprintf ("Test 3 OK\n");

}


void testmisc6 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readerm, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(readerm, 2000, 25, "reader3", 2, "reader B", lck);
        wr1 = create(writerm, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);


        kprintf("-start reader B, then sleep 1s. reader B(prio 25) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("---chprio reader B (prio 35)\n");
        chprio(rd2,35);
        sleep (1);
        kprintf("------expected priority of wr1= 35 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 35, "Test 3 failed");


       sleep (8);
        kprintf ("Test 3 OK\n");

}


void testmisc7 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readerm, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(readerm, 2000, 25, "reader3", 2, "reader B", lck);
        wr1 = create(writerm, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);


        kprintf("-start reader B, then sleep 1s. reader B(prio 25) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("---chprio reader B (prio 5)\n");
        chprio(rd2,5);
        sleep (1);
        kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 20, "Test 3 failed");


       sleep (8);
        kprintf ("Test 3 OK\n");

}



void testmisc8 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(readerm, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(readerm, 2000, 15, "reader3", 2, "reader B", lck);
        wr1 = create(writerm, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);
	
	kprintf("-start reader A, then sleep 1s. reader A (prio 25) blocked on the lock\n");
        resume (rd1);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("-start reader B, then sleep 1s. reader B(prio 15) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("---chprio reader B (prio 5)\n");
        chprio(rd2,5);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


       sleep (8);
        kprintf ("Test 3 OK\n");

}


void readerm9 (char *msg, int lck)
{
        int     ret;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
}

void writerm9 (char *msg, int lck1, int lck2)
{
       int l1,l2;
	 kprintf ("  %s: to acquire lock\n", msg);
        l1 = lock (lck1, WRITE, DEFAULT_LOCK_PRIO);
        assert (l1 != SYSERR, "lock failed");

        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
	
	kprintf ("  %s: to acquire lock\n", msg);
        l2 = lock (lck2, WRITE, DEFAULT_LOCK_PRIO);
	assert (l2 != SYSERR, "lock failed");

        kprintf ("  %s: acquired lock, sleep 10s\n", msg);

        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
	int rs = releaseall (1, lck1);
	assert (rs != SYSERR, "release failed");

	sleep(1);
	kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(currpid));  

	assert(getprio(currpid) == 20, "Test 3 failed");

}

void testmisc9 ()
{
        int     lck1, lck2;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck1  = lcreate ();
        assert (lck1 != SYSERR, "Test 3 failed");
	kprintf("lock1 = %d", lck1);

	lck2  = lcreate ();
        assert (lck2 != SYSERR, "Test 3 failed");

	kprintf("lock2 = %d", lck2);

        rd1 = create(readerm9, 2000, 25, "reader3", 2, "reader A", lck1);
        rd2 = create(readerm9, 2000, 17, "reader3", 2, "reader B", lck2);
        wr1 = create(writerm9, 2000, 20, "writer3", 3, "writer", lck1,lck2);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A (prio 25) blocked on the lock 1\n");
        resume (rd1);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("-start reader B, then sleep 1s. reader B(prio 17) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");

	
       sleep (8);

	kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(currpid));

        assert(getprio(currpid) == 20, "Test 3 failed");
        kprintf ("Test 3 OK\n");

}


void readerm10 (char *msg, int lck)
{
        int     ret;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
}

void writerm10 (char *msg, int lck1, int lck2)
{
       int l1,l2;
         kprintf ("  %s: to acquire lock\n", msg);
        l1 = lock (lck1, WRITE, DEFAULT_LOCK_PRIO);
        assert (l1 != SYSERR, "lock failed");

        kprintf ("  %s: acquired lock, sleep 10s\n", msg);

        kprintf ("  %s: to acquire lock\n", msg);
        l2 = lock (lck2, WRITE, DEFAULT_LOCK_PRIO);
        assert (l2 != SYSERR, "lock failed");

        kprintf ("  %s: acquired lock, sleep 10s\n", msg);

        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck2);
        sleep(1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(currpid));

        assert(getprio(currpid) == 25, "Test 3 failed");

}

void testmisc10 ()
{
        int     lck1, lck2;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck1  = lcreate ();
        assert (lck1 != SYSERR, "Test 3 failed");
        kprintf("lock1 = %d", lck1);

        lck2  = lcreate ();
        assert (lck2 != SYSERR, "Test 3 failed");

        kprintf("lock2 = %d", lck2);

        rd1 = create(readerm10, 2000, 25, "reader3", 2, "reader A", lck1);
        rd2 = create(readerm10, 2000, 23, "reader3", 2, "reader B", lck2);
 wr1 = create(writerm10, 2000, 20, "writer3", 3, "writer", lck1,lck2);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A (prio 25) blocked on the lock 1\n");
        resume (rd1);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("-start reader B, then sleep 1s. reader B(prio 23) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


       sleep (8);
        kprintf ("Test 3 OK\n");

}

void readerm11 (char *msg, int lck)
{
        int     ret;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
}

void writerm11 (char *msg, int lck1, int lck2)
{
       int l1,l2;
         kprintf ("  %s: to acquire lock\n", msg);
        l1 = lock (lck1, WRITE, DEFAULT_LOCK_PRIO);
        assert (l1 != SYSERR, "lock failed");

        kprintf ("  %s: acquired lock, sleep 10s\n", msg);

        kprintf ("  %s: to acquire lock\n", msg);
        l2 = lock (lck2, WRITE, DEFAULT_LOCK_PRIO);
        assert (l2 != SYSERR, "lock failed");

        kprintf ("  %s: acquired lock, sleep 10s\n", msg);

        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (2, lck1, lck2);
        sleep(1);
        kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(currpid));

        assert(getprio(currpid) == 20, "Test 3 failed");

}
void testmisc11 ()
{
        int     lck1, lck2;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck1  = lcreate ();
        assert (lck1 != SYSERR, "Test 3 failed");
        kprintf("lock1 = %d", lck1);

        lck2  = lcreate ();
        assert (lck2 != SYSERR, "Test 3 failed");

        kprintf("lock2 = %d", lck2);

        rd1 = create(readerm10, 2000, 25, "reader3", 2, "reader A", lck1);
        rd2 = create(readerm10, 2000, 23, "reader3", 2, "reader B", lck2);
 	wr1 = create(writerm10, 2000, 20, "writer3", 3, "writer", lck1,lck2);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A (prio 25) blocked on the lock 1\n");
        resume (rd1);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


        kprintf("-start reader B, then sleep 1s. reader B(prio 23) blocked on the lock\n");
        resume (rd2);
        sleep (1);
        kprintf("------expected priority of wr1= 25 && actual prio of wr1 = %d\n",getprio(wr1));

        assert (getprio(wr1) == 25, "Test 3 failed");


       sleep (8);


	kprintf("------expected priority of wr1= 20 && actual prio of wr1 = %d\n",getprio(currpid));

        assert(getprio(currpid) == 20, "Test 3 failed");
        kprintf ("Test 3 OK\n");

}

void writer12(char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (30);
}

void writer122(char *msg, int lck1, int lck2)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck2, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (5);

	kprintf ("  %s: to acquire lock\n", msg);
        lock (lck1, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);

}

void testmisc12 ()
{
        int     lck1,lck2;
        int    a,b,c;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck1  = lcreate ();
        assert (lck1 != SYSERR, "Test 3 failed");

	lck2  = lcreate ();
        assert (lck2 != SYSERR, "Test 3 failed");

        a = create(writer12, 2000, 25, "a", 2, "a", lck1);
        b = create(writer122, 2000, 30, "b", 3, "b", lck1,lck2);
        c = create(writer12, 2000, 40, "c", 2, "c", lck2);


        kprintf("-start wr A, then sleep 1s. wr A(prio 25) acquires the lock\n");
        resume(a);
        sleep (1);
        
	kprintf("-start wr B, then sleep 1s. lock granted to write (prio 30)\n");
        resume(b);
        sleep (1);


        kprintf("-start wr C, then sleep 1s.  C(prio 40) blocked on the lock\n");
        resume (c);
	sleep (1);
	
	sleep(10);

        kprintf("------expected priority of A= 40 && actual prio of A= %d\n",getprio(a));

        kprintf("------expected priority of B = 40 && actual prio of B = %d\n",getprio(b));
	assert (getprio(a) == 40, "Test 3 failed due to a **");
        assert (getprio(b) == 40, "Test 3 failed due to b **");


        sleep (8);
        kprintf ("Test 3 OK\n");
}





/* 	 R/W	*/
char output13[10];
int count13;

void writer13(char *msg, int lck)
{	output13[count13++] = *msg;
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (30);
}

void writer133(char *msg, int lck1, int lck2)
{	
        output13[count13++] = *msg;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck2, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (5);
	
	output13[count13++] = *msg;
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck1, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);

}
void testmisc13 ()
{
	count13 = 0;	
        int     lck1,lck2;
        int    a,b,c;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck1  = lcreate ();
        assert (lck1 != SYSERR, "Test 3 failed");

        lck2  = lcreate ();
        assert (lck2 != SYSERR, "Test 3 failed");

        a = create(writer13, 2000, 25, "a", 2, "a", lck1);
        b = create(writer133, 2000, 30, "b", 3, "b", lck1,lck2);
        c = create(writer13, 2000, 40, "c", 2, "c", lck2);


        kprintf("-start wr A, then sleep 1s. wr A(prio 25) acquires the lock\n");
        resume(a);
        sleep (1);

        kprintf("-start wr B, then sleep 1s. lock granted to write (prio 30)\n");
        resume(b);
        sleep (1);


        kprintf("-start wr C, then sleep 1s.  C(prio 40) blocked on the lock\n");
        resume (c);
        sleep (1);

        sleep(10);

        kprintf("------expected priority of A= 40 && actual prio of A= %d\n",getprio(a));

        kprintf("------expected priority of B = 40 && actual prio of B = %d\n",getprio(b));
        assert (getprio(a) == 40, "Test 3 failed due to a **");
        assert (getprio(b) == 40, "Test 3 failed due to b **");


        sleep (8);
	kprintf("output=%s\n", output13);
        kprintf ("Test 3 OK\n");
}
