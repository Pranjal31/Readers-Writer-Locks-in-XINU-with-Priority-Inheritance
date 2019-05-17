#ifndef _LOCK_H_
#define _LOCK_H_

#define ALL		 -1
#define READ		 10
#define WRITE		 11
#ifndef	NLOCKS
#define	NLOCKS		50	/* number of locks, if not defined		*/
#endif

#define	LFREE	'\01'		/* this lock is free				*/
#define	LUSED	'\02'		/* this lock is used				*/

struct	lentry	{		/* lock table entry				*/
	char	lstate;		/* the state LFREE or LUSED			*/
	int 	ltype;		/* lock acquired by R or W			*/
	int 	lprio;		/* max priority among the processes waiting for this lock 	*/
	int	lacquired[50];	/* which processes have acquired this lock	*/
	int	ldeleted;	/* 1 if the lock has been deleted before, 0 otherwise		*/
	int	lnumreaders;	/* number of active readers with this lock	*/
	int	lockcnt;	/* count for this lock (initialized to 1)	*/
	int	lqhead;		/* q index of head of list			*/
	int	lqtail;		/* q index of tail of list			*/
};
extern	struct	lentry	locks[];
extern	int	nextlock;

#define	isbadlock(l)	(l<0 || l>=NLOCKS)

void linit (void);
void inherit_priorities_for_process(struct pentry *);
int max_schedprio_among_procs_waiting_for_lock(struct lentry *);

#endif

