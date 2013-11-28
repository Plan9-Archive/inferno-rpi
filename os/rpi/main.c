
#include "u.h"
#include "../port/lib.h"
#include "dat.h"
#include "mem.h"
#include "fns.h"
#include "version.h"

#include "../port/uart.h"
PhysUart* physuart[1];

Conf conf;
Mach *m = (Mach*)MACHADDR;
Proc *up = 0;

extern int main_pool_pcnt;
extern int heap_pool_pcnt;
extern int image_pool_pcnt;

void
confinit(void)
{
	ulong base;
	conf.topofmem = 128*MB;

	base = PGROUND((ulong)end);
	conf.base0 = base;

	conf.npage1 = 0;
	conf.npage0 = (conf.topofmem - base)/BY2PG;
	conf.npage = conf.npage0 + conf.npage1;
	conf.ialloc = (((conf.npage*(main_pool_pcnt))/100)/2)*BY2PG;

	conf.nproc = 100 + ((conf.npage*BY2PG)/MB)*5;
	conf.nmach = 1;

	print("Conf: top=%lud, npage0=%lud, ialloc=%lud, nproc=%lud\n",
			conf.topofmem, conf.npage0,
			conf.ialloc, conf.nproc);
}

static void
poolsizeinit(void)
{
	ulong nb;
	nb = conf.npage*BY2PG;
	poolsize(mainmem, (nb*main_pool_pcnt)/100, 0);
	poolsize(heapmem, (nb*heap_pool_pcnt)/100, 0);
	poolsize(imagmem, (nb*image_pool_pcnt)/100, 1);
}

void
main() {
	pl011_puts("Entered main() at ");
	pl011_addr(&main, 0);
	pl011_puts(" with SP=");
	pl011_addr((void *)getsp(), 1.);

	pl011_puts("Clearing Mach:  ");
	memset(m, 0, sizeof(Mach));
	pl011_addr((char *)m,		0); pl011_puts("-");
	pl011_addr((char *)(m+1),	1);

	pl011_puts("Clearing edata: ");
	memset(edata, 0, end-edata);
	pl011_addr((char *)&edata,	0); pl011_puts("-");
	pl011_addr((char *)&end,	1);

	conf.nmach = 1;
	serwrite = &pl011_serputs;

	confinit();
	xinit();
	poolinit();
	poolsizeinit();
	trapinit();
	printinit();

	print("\nARM %ld MHz id %8.8lux\n", (m->cpuhz+500000)/1000000, getcpuid());
	print("Inferno OS %s Vita Nuova\n\n", VERSION);

	procinit();
	links();
	chandevreset();

	eve = strdup("inferno");

	userinit();
	schedinit();

	pl011_puts("to inifinite loop\n\n");
	for (;;);
}

void
init0(void)
{
	Osenv *o;
	char buf[2*KNAMELEN];

	up->nerrlab = 0;

	print("Starting init0()\n");
	spllo();

	if(waserror())
		panic("init0 %r");

	/* These are o.k. because rootinit is null.
	 * Then early kproc's will have a root and dot. */

	o = up->env;
	o->pgrp->slash = namec("#/", Atodir, 0, 0);
	cnameclose(o->pgrp->slash->name);
	o->pgrp->slash->name = newcname("/");
	o->pgrp->dot = cclone(o->pgrp->slash);

	chandevinit();

	if(!waserror()){
		ksetenv("cputype", "arm", 0);
		snprint(buf, sizeof(buf), "arm %s", conffile);
		ksetenv("terminal", buf, 0);
		poperror();
	}

	poperror();

	disinit("/osinit.dis");
}

void
userinit(void)
{
	Proc *p;
	Osenv *o;

	p = newproc();
	o = p->env;

	o->fgrp = newfgrp(nil);
	o->pgrp = newpgrp();
	o->egrp = newegrp();
	kstrdup(&o->user, eve);

	strcpy(p->text, "interp");

	p->fpstate = FPINIT;

	/*	Kernel Stack
		N.B. The -12 for the stack pointer is important.
		4 bytes for gotolabel's return PC */
	p->sched.pc = (ulong)init0;
	p->sched.sp = (ulong)p->kstack+KSTACK-8;

	ready(p);
}

void	segflush(void*, ulong) { return; }
void	idlehands(void) { return; }

void	exit(int) { return; }
void	reboot(void) { return; }
void	halt(void) { spllo(); for(;;); }

Timer*	addclock0link(void (*)(void), int) { return 0; }
void	clockcheck(void) { return; }

void	fpinit(void) {}
void	FPsave(void*) {}
void	FPrestore(void*) {}
