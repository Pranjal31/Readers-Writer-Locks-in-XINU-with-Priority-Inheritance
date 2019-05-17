/* insertinlqueue.c  -  insertinlqueue */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <lock.h>

extern unsigned long ctr1000;
/*----------------------------------------------------------------------------------
 * insertinlqueue.c  --  insert a process into a q list in key (lock priority) order
 *----------------------------------------------------------------------------------
 */
int insertinlqueue(int proc, int head, int wprio, int ltype)
{
	int	next;			/* runs through list		*/
	int	prev;

	next = q[head].qnext;
	while (q[next].qkey < wprio)	/* tail has maxint as key	*/
		next = q[next].qnext;
	q[proc].qnext = next;
	q[proc].qprev = prev = q[next].qprev;
	q[proc].qkey  = wprio;
	q[proc].qltype = ltype;
	q[proc].qlwstime = ctr1000;
	q[prev].qnext = proc;
	q[next].qprev = proc;
	
	return(OK);
}
