#ifdef	CPU
#include <vxWorks.h>
#endif
#include <setjmp.h>
#include <signal.h>

#define	YES	1
#define	NO	0

#ifdef	AOSVS
jmp_buf	tjmpfpe;

void
tsigfpe (sig)
int	sig;
{
	longjmp (tjmpfpe, YES);
}
#else

jmp_buf	tjmpfpe;
int	save_sig;
int	save_code;
struct	sigcontext *save_scp;
char	*save_addr;

void
tsigfpe (sig, code, scp, addr)
int	sig;
int	code;
struct	sigcontext *scp;
char	*addr;
{
	save_sig = sig;
	save_code = code;
	save_scp = scp;
	save_addr = addr;
	longjmp (tjmpfpe, YES);
}
#endif

trefv (c, r, s)
int	c;
float	r;
float	*s;
{
	int	status;
#ifdef	AOSVS
	void	(*psigfpe)();

	psigfpe = signal (SIGFPE, tsigfpe);
#else
	struct	sigvec	tsigvec, psigvec;

	tsigvec.sv_handler	= tsigfpe;
	tsigvec.sv_mask		= 0;
	tsigvec.sv_flags	= 0;

	sigvec (SIGFPE, &tsigvec, &psigvec);
#endif
	status = setjmp (tjmpfpe);
	if (status == NO) {
	    if (c == 0) {
		*s = 0;
		status = YES;
	    } else {
		*s = r/c;
	    }
	}
#ifdef	AOSVS
	signal (SIGFPE, psigfpe);
#else
	sigvec (SIGFPE, &psigvec, 0);
#endif
	return (status);
}

tscale (c, s, z, v)
int	c;
float	s;
float	z;
float	*v;
{
	int	status;
#ifdef	AOSVS
	void	(*psigfpe)();

	psigfpe = signal (SIGFPE, tsigfpe);
#else
	struct	sigvec	tsigvec, psigvec;

	tsigvec.sv_handler	= tsigfpe;
	tsigvec.sv_mask		= 0;
	tsigvec.sv_flags	= 0;

	sigvec (SIGFPE, &tsigvec, &psigvec);
#endif
	status = setjmp (tjmpfpe);
	if (status == NO) {
	    *v = c*s+z;
	}
#ifdef	AOSVS
	signal (SIGFPE, psigfpe);
#else
	sigvec (SIGFPE, &psigvec, 0);
#endif
	return (status);
}

tpoly (p, n, x, y)
float	p[];
int	n;
float	x;
float	*y;
{
	int	status;
#ifdef	AOSVS
	void	(*psigfpe)();

	psigfpe = signal (SIGFPE, tsigfpe);
#else
	struct	sigvec	tsigvec, psigvec;

	tsigvec.sv_handler	= tsigfpe;
	tsigvec.sv_mask		= 0;
	tsigvec.sv_flags	= 0;

	sigvec (SIGFPE, &tsigvec, &psigvec);
#endif
	status = setjmp (tjmpfpe);
	if (status == NO) {

	    register int   m = n;
	    register float r;

	    r = p[--m];
	    for (; m != 0; --m) {
		r *= x;
		r += p[m-1];
	    }
	    *y = r;
	}
#ifdef	AOSVS
	signal (SIGFPE, psigfpe);
#else
	sigvec (SIGFPE, &psigvec, 0);
#endif
	return (status);
}
