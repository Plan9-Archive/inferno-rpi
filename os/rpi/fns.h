
#define KADDR(p)    ((void *)p)
#define PADDR(p)    ((ulong)p)
#define DMAADDR(va)	(BUSDRAM |((uintptr)(va)))
#define waserror()  (up->nerrlab++, setlabel(&up->errlab[up->nerrlab-1]))
#define procsave(p)		/* Save the mach part of the current */
						/* process state, no need for one cpu */
#define kmapinval()

void	(*serwrite)(char*, int);
void    (*screenputs)(char*, int);

#include "../port/portfns.h"

void	pl011_putc(int);
void	pl011_puts(char *);
void	pl011_serputs(char *, int);
void	pl011_addr(void *a, int nl);

ulong	getsp(void);
ulong	getpc(void);
ulong	getcpuid(void);
ulong	getcallerpc(void*);
u32int	lcycles(void);

void	idlehands(void);
void	coherence(void);
void	clockinit(void);
void	trapinit(void);
char *	trapname(int psr);
int		isvalid_va(void *v);
int		isvalid_wa(void *v);
void	setr13(int, void*);
void	vectors(void);
void	vtable(void);
void	dumpregs(Ureg*);
void	dumparound(uint addr);
int		(*breakhandler)(Ureg*, Proc*);
void	irqenable(int, void (*)(Ureg*, void*), void*);

void cachedwbinv(void);
void cachedwbse(void*, int);
void cachedwbinvse(void*, int);
void cacheiinv(void);
void cacheuwbinv(void);

void*   fbinit(int, int*, int*, int*);
