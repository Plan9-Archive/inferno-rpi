#
#	initially generated by c2l
#

implement Perms;

include "draw.m";

Perms: module
{
	init: fn(nil: ref Draw->Context, argl: list of string);
};

include "sys.m";
	sys: Sys;

init(nil: ref Draw->Context, argl: list of string)
{
	sys = load Sys Sys->PATH;
	main(len argl, argl);
}

# 
#  * generate permutations of N elements
#  *	from ``On Programming, an interim report on the SETL project'',
#  *	Jacob T Schwartz (ed), New York University
#  
Seq: adt{
	nel: int;
	el: array of int;
};

origin: int = 1;

main(argc: int, argv: list of string): int
{
	n: int;

	if(argc > 1 && (as := hd tl argv)[0] == '-'){
		origin = int (as[1: ]);
		argc--;
		argv = tl argv;
	}
	if(argc != 2){
		sys->fprint(sys->fildes(2), "Usage: perms #elements\n");
		exit;
	}
	n = int hd tl argv;
	if(n > 0)
		perms(n);
	exit;
}


perms(n: int)
{
	seq: ref Seq;

	seq = newseq(n);
	do
		putseq(seq);
	while(eperm(seq) != nil);
}

putseq(seq: ref Seq)
{
	k: int;

	for(k = 0; k < seq.nel; k++)
		sys->print(" %d", seq.el[k]+origin);
	sys->print("\n");
}

eperm(seq: ref Seq): ref Seq
{
	j, k, n: int;

	n = seq.nel;
	#  if sequence is monotone decreasing, there are no more
	# 		permutations.  Otherwise, find last point of increase 
	hit := 0;
	for(j = n-2; j >= 0; j--)
		if(seq.el[j] < seq.el[j+1]){
			hit = 1;
			break;
		}
	if(!hit)
		return nil;
	#  then find the last seq[k] which exceeds seq[j] and swop 
	for(k = seq.nel-1; k > j; k--)
		if(seq.el[k] > seq.el[j]){
			{
				t: int;

				t = seq.el[k];
				seq.el[k] = seq.el[j];
				seq.el[j] = t;
			}
			;
			break;
		}
	#  then re-arrange all the elements from seq[j+1] into
	# 		increasing order 
	for(k = j+1; k < (n+j+1)/2; k++){
		kk: int;

		kk = n-k+j;
		{
			t: int;

			t = seq.el[k];
			seq.el[k] = seq.el[kk];
			seq.el[kk] = t;
		}
		;
	}
	return seq;
}

newseq(n: int): ref Seq
{
	seq: ref Seq;
	k: int;

	seq = ref Seq;
	seq.nel = n;
	seq.el = array[n] of int;
	for(k = 0; k < n; k++)
		seq.el[k] = k;
	return seq;
}
