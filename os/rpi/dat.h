
#define HZ      	(100)       /*! clock frequency */
#define MS2HZ       (1000/HZ)   /*! millisec per clock tick */
#define TK2SEC(t)   ((t)/HZ)    /*! ticks to seconds */
#define MS2TK(t)    ((t)/MS2HZ) /*! milliseconds to ticks */

#define MACHP(n)    (n == 0 ? (Mach*)(MACHADDR) : (Mach*)0)

typedef struct Lock Lock;
typedef struct Ureg Ureg;
typedef struct Label Label;
typedef struct FPenv FPenv;
typedef struct Mach Mach;
typedef struct FPU FPU;
typedef ulong Instr;
typedef struct Conf Conf;

struct Lock
{
	ulong   key;
	ulong   sr;
	ulong   pc;
	int pri;
};

struct Label
{
	int x;
};

enum
{
	FPINIT,
	FPACTIVE,
	FPINACTIVE
};

struct FPenv
{
	int x;
};

struct  FPU
{
	FPenv env;
};

struct Conf
{
	ulong   nmach;      /* processors */
	ulong   nproc;      /* processes */
	ulong   npage0;     /* total physical pages of memory */
	ulong   npage1;     /* total physical pages of memory */
	ulong   base0;      /* base of bank 0 */
	ulong   base1;      /* base of bank 1 */
	ulong   ialloc;     /* max interrupt time allocation in bytes */
};

#include "../port/portdat.h"

struct Mach
{
	int     machno;     /* physical id of processor */
	ulong   ticks;      /* of the clock since boot time */
	Proc*   proc;       /* current process on this processor */
	Label   sched;      /* scheduler wakeup */
};

extern Mach *m;
extern Proc *up;

