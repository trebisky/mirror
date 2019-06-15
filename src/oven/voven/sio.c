/*************************************************************************
 * "driver" for the Force SIO-2 board under VxWorks
 *  Tom Trebisky 3/6/90	Steward Observatory
 *************************************************************************/

/* VxWorks include files */
#include "vxWorks.h"
#include "iosLib.h"
#include "stdioLib.h"
#include "semLib.h"
#include "vme.h"
#ifdef VX4
#include "iv68k.h"
#else
#include "iv.h"
#endif
#include "setjmp.h"

/*
#define PARITY
#define TEST
*/
#define PARITY

#ifdef TEST
jmp_buf userenv;
#endif


/************************************************************************
 * The following C structure gives the register layout for a
 * MC68230 PIT chip accessed from a 16 bit bus
 *
 * On the Force SIO board, this chip is used in a "minimal" fashion.
 * All pins on port C are simply pulled up and not connected to anything
 * (thus no timer interrupts and such), The timer could be set to run off
 * the Clock (8.064Mhz), but would need to be "polled" by the cpu.
 * The intent of this chip is just to use ports A and B in a dedicated
 * fashion to control various aspects of the MPCC operation.
 *	(See below)
 *************************************************************************/

struct pit {
/*00*/	unsigned char dum0, pgcr;	/* Port general control register */
/*02*/	unsigned char dum1, psrr;	/* Port service request */
/*04*/	unsigned char dum2, paddr;	/* Port A data direction */
/*06*/	unsigned char dum3, pbddr;	/* Port B data direction */
/*08*/	unsigned char dum4, pcddr;	/* Port C data direction */
/*0a*/	unsigned char dum5, pivr;	/* Port interrupt vector */
/*0c*/	unsigned char dum6, pactl;	/* Port A control */
/*0e*/	unsigned char dum7, pbctl;	/* Port B control */
/*10*/	unsigned char dum8, padata;	/* Port A data */
/*12*/	unsigned char dum9, pbdata;	/* Port B data */
/*14*/	unsigned char duma, paalt;	/* Port A alternate data */
/*16*/	unsigned char dumb, pbalt;	/* Port B alternate data */
/*18*/	unsigned char dumc, pcdata;	/* Port C data */
/*1a*/	unsigned char dumd, pstat;	/* Port status */
/*--*/	unsigned char space0, space1;	/* --- */
/*--*/	unsigned char space2, space3;	/* --- */
/*20*/	unsigned char zum0, tcr;	/* Timer control register */
/*22*/	unsigned char zum1, tivr;	/* Timer interrupt vector */
/*--*/	unsigned char zum2, null0;	/* --- */
/*26*/	unsigned char zum3, cprh;	/* Timer: counter preload high */
/*28*/	unsigned char zum4, cprm;	/* Timer: counter preload middle */
/*2a*/	unsigned char zum5, cprl;	/* Timer: counter preload low */
/*--*/	unsigned char zum6, null1;	/* --- */
/*2e*/	unsigned char zum7, cnth;	/* Timer: counter high */
/*30*/	unsigned char zum8, cntm;	/* Timer: counter middle */
/*32*/	unsigned char zum9, cntl;	/* Timer: counter low */
/*34*/	unsigned char zuma, tsr;	/* Timer: status */
};

#define DD_OUTS	0xff		/* data direction: output on all 8 pins */
#define DD_INS	0x00		/* data direction: input on all 8 pins */
/* In this hardware configuration, using ports A or B as inputs would not
 * do anything useful */

#define RCV_ON	0x3f		/* gate on all 6 receivers */
#define USE_422	0x3f		/* select RS422 drivers */
#define USE_232	0x00		/* select RS232 drivers */
#define USE_OPTO	0x00	/* select OPTO drivers (SIO_2A ch. 1&2 only) */
#define NO_OPTO		0xc0	/* NO OPTO drivers (SIO_2A ch. 1&2 only) */

/************************************************************************
 * The following C structure gives the register layout for a
 * Rockwell R68561 MPCC chip in 8 bit mode accessed from a 16 bit bus
 * in the wierd interleaving used on the Force SIO board.
 *************************************************************************/

struct bmpcc {
/*00*/	unsigned char dum0, rsr;	/* receiver status register */
/*02*/	unsigned char dum1, rdat;	/* receiver data register */
/*04*/	unsigned char dum2, riv;	/* receiver interrupt vector */
/*06*/	unsigned char dum3, pad0;	/* --- */
/*08*/	unsigned char dum4, tsr;	/* transmitter status register */
/*0a*/	unsigned char dum5, tdat;	/* transmitter data register */
/*0c*/	unsigned char dum6, tiv;	/* transmitter interrupt vector */
/*0e*/	unsigned char dum7, pad1;	/* --- */
/*10*/	unsigned char dum8, sisr;	/* serial status register */
/*12*/	unsigned char dum9, pad2;	/* --- */
/*14*/	unsigned char du10, siv;	/* serial interrupt vector */
/*16*/	unsigned char du11, pad3;	/* --- */
/*18*/	unsigned char du12, psr1;	/* protocol select (1) */
/*1a*/	unsigned char du13, addr1;	/* address register (1) */
/*1c*/	unsigned char du14, baud1;	/* baud rate register (1) */
/*1e*/	unsigned char du15, ccr;	/* clock control register */
/*20*/	unsigned char du16, rcr;	/* receiver control register */
/*22*/	unsigned char du17, pad4;	/* --- */
/*24*/	unsigned char du18, rier;	/* receiver interrupt enable */
/*26*/	unsigned char du19, pad5;	/* --- */
/*28*/	unsigned char du20, tcr;	/* transmitter control register */
/*2a*/	unsigned char du21, pad6;	/* --- */
/*2c*/	unsigned char du22, tier;	/* transmitter interrupt enable */
/*2e*/	unsigned char du23, pad7;	/* --- */
/*30*/	unsigned char du24, sicr;	/* serial control register */
/*32*/	unsigned char du25, pad8;	/* --- */
/*34*/	unsigned char du26, sier;	/* serial interrupt enable */
/*36*/	unsigned char du27, pad9;	/* --- */
/*38*/	unsigned char du28, psr2;	/* protocol select (2) */
/*3a*/	unsigned char du29, addr2;	/* address register (2) */
/*3c*/	unsigned char du30, baud2;	/* baud rate register (2) */
/*3e*/	unsigned char du31, ecr;	/* error control register */
/*40*/
};

/*
 * This C structure gives the register layout for a
 * Rockwell R68561 MPCC chip accessed from a 16 bit bus.
 * I have had nothing but trouble with this 16 bit mode, and I think
 * that trying to use this thing in word mode with asynchronous protocols
 * is off-the-wall anyhow.
 */
struct wmpcc {
/*00*/	short	mpcc_pad8[16];	/* unused addresses in 16 bit mode */ 
/*20*/	short	mpcc_rcsr;	/* receiver control and status */
/*22*/	short	mpcc_rdat;	/* receiver data (16 bit) */
/*24*/	short	mpcc_rir;	/* receiver interrupt */
/*26*/	short	mpcc_rpad;	/* --- */
/*28*/	short	mpcc_tcsr;	/* transmitter control and status */
/*2a*/	short	mpcc_tdat;	/* transmitter data (16 bit) */
/*2c*/	short	mpcc_tir;	/* transmitter interrupt */
/*2e*/	short	mpcc_tpad;	/* --- */
/*30*/	short	mpcc_scsr;	/* handshake control and status */
/*32*/	short	mpcc_pad2;	/* --- */
/*34*/	short	mpcc_sir;	/* handshake interrupt */
/*36*/	short	mpcc_pad3;	/* --- */
/*38*/	short	mpcc_prot;	/* protocol selection */
/*3a*/	short	mpcc_addr;	/* address (synchronous protocols) */
/*3c*/	short	mpcc_baud;	/* baud rate generator control */
/*3e*/	short	mpcc_eccr;	/* error & clock control */
/*40*/
};

/* for word mode, we only need to define different bits for the
 * registers which are in the even (upper) byte, for those in the
 * odd (lower) byte, the byte mode definitions work too.
 */

/* bits in the transmitter csr */
#define WTCR_ENA	0x8000	/* enable transmitter */
#define	WTCR_ICS	0x2000	/* yank high to generate break */
#define	WTCR_THW	0x1000	/* transmit half word (flush odd byte) */
#define	WTCR_RESET	0x0100	/* reset transmitter */

/* bits in the receiver csr */
#define WRCR_RESET	0x0100	/* reset receiver */

/* bits in the serial interface csr */
#define WSI_RTS		0x8000	/* RTS level */
#define WSI_DTR		0x4000	/* DTR level */
#define WSI_ECHO	0x0400	/* echo control */
#define WSI_TEST	0x0200	/* loopback test mode */

/* bits in the error control register (upper byte of mpcc_eccr) */
#define WEC_PAR		0x8000
#define WEC_ODD		0x4000
#define WEC_EVEN	0x0000
    /* other bits control CRC generator */

/* bits in the protocol select register (PSR2-PSR1) */
#define WPR_BY		0x0000	/* first time only, to get 8 bit mode */
#define WPR_WD		0x8000	/* run in 16 bit (word) mode */
#define WPR_ST1		0x0000	/* 1 stop bit */
#define WPR_STH		0x2000	/* 1.5 stop bit */
#define WPR_ST2		0x4000	/* 2 stop bit */
#define WPR_CH7		0x1000	/* 7 bit characters */
#define WPR_CH8		0x1800	/* 8 bit characters */
    /* any parity generated/checked is in addition to this */
#define WPR_ASY		0x0600	/* asynch protocol (not BOP, synch, ...) */

/* interrupt mask bits (set to enable) follow */
#define RIE_DA		0x80	/* receiver data available */
#define RIE_EOF		0x40	/* end of frame */
#define RIE_CPERR	0x10	/* CRC/Parity error */
#define RIE_FRERR	0x08	/* frame error */
#define RIE_OVRN	0x04	/* receiver overrun */
#define RIE_AB		0x02	/* receiver abort/break */

#define TIE_DRA		0x80	/* transmitter data register available */
#define TIE_FC		0x40	/* transmit frame complete */
#define TIE_UNRN	0x04	/* transmitter underrun */
#define TIE_FERR	0x02	/* transmit frame error */

#define SIE_CTS		0x80	/* CTS */
#define SIE_DSR		0x40	/* DSR */
#define SIE_DCD		0x20	/* DCD */

/** 8-bit bit definitions follow: **/

/* bits in the transmitter csr */
#define TCR_ENA		0x80	/* enable transmitter */
#define	TCR_ICS		0x20	/* yank high to generate break */
#define	TCR_THW		0x10	/* transmit half word (flush odd byte) */
#define	TCR_RESET	0x01	/* reset transmitter */

#define TSR_TDRA	0x80	/* transmitter data reg. available */

/* bits in the receiver csr */
#define RCR_RESET	0x01	/* reset receiver */

#define RSR_RDA		0x80	/* data available */
#define RSR_CPERR	0x10	/* crc/parity error */
#define RSR_FERR	0x08	/* framing error */
#define RSR_OVRN	0x04	/* overrun */

/* bits in the serial interface control register */
#define SI_RTS		0x80	/* RTS level */
#define SI_DTR		0x40	/* DTR level */
#define SI_ECHO		0x04	/* echo control */
#define SI_TEST		0x02	/* loopback test mode */

/* bits in the serial status register */
#define SS_TCTS		0x80	/* CTS transition */
#define SS_TDSR		0x40	/* DSR transition */
#define SS_TDCD		0x20	/* DCD transition */
#define SS_CTS		0x10	/* CTS level */
#define SS_DSR		0x08	/* DSR level */
#define SS_DCD		0x04	/* DCD level */

/* bits in the error control register */
#define EC_PAR		0x80
#define EC_ODD		0x40
#define EC_EVEN		0x00
#define EC_CRC0		0x04

/* bits in the clock control register */
#define PSCDV2		0x00
#define PSCDV3		0x10
#define TXCOUT		0x08	/* use internal clock for TxC */
#define RXCINT		0x04	/* use internal clock for RxC */

/* bits in the protocol select register */
#define PR1_CTLEX	0x02	/* BOP only - select 16 bit control field */
#define PR1_ADDEX	0x01	/* BOP only - enable address extension */

#define PR2_BY		0x00	/* first time only, to get 8 bit mode */
#define PR2_WD		0x80	/* select 16 bit mode (LOOK out!) */
#define PR2_ST1		0x00	/* 1 stop bit */
#define PR2_STH 	0x20	/* 1.5 stop bit */
#define PR2_ST2		0x40	/* 2 stop bit */
#define PR2_CH7		0x10	/* 7 bit characters */
#define PR2_CH8		0x18	/* 8 bit characters */
    /* any parity generated/checked is in addition to these data bits */
#define PR2_ASY		0x06	/* asynch protocol (not BOP, synch, ...) */

#ifdef PARITY
#define PR2_ASYNC	(PR2_BY|PR2_ST1|PR2_CH7|PR2_ASY)
#else
#define PR2_ASYNC	(PR2_BY|PR2_ST1|PR2_CH8|PR2_ASY)
#endif

/*************************************************************************
 * That ends the chip-level definitions, now on to the board and system level.
 *************************************************************************/

/* Testing was done with a single board, and with the first channel on
 * that board for the most part.
 * I am told that in the final configuration we will have three boards
 * with three channels used from each (for redundancy/reliability).
 * So we want logical channel 3 (of 0,1,2,3) to be channel 0 on board 1.
 * Nothing has been specified about the "extra" 9 channels, so I will set
 * them up to be channels 9, 10, ..., 17.  This is done thru chmap, which
 * maps board and chip number to logical channel number.  After initialization
 * we don't know or care how they were mapped, we just lookup the uart base
 * address in chtab[] and go.
 * My sympathy for whoever has to hook up the cables and wonder why
 * it was done like this, but:
 * (a) It wasn't my idea.   and   (b) There are reasons for it.
 */

#define MAXBOARDS	3	/* number of boards */
#define NSIOCHAN	6	/* channels per board */
#define MAXCHAN		(MAXBOARDS*NSIOCHAN)	/* (for all boards) */

static char chmap[MAXBOARDS][NSIOCHAN] = 
	{ 0, 1, 2, 9, 10, 11,
	  3, 4, 5, 12, 13, 14,
	  6, 7, 8, 15, 16, 17 };

/* Now the issue of board addresses -
/* The vme147 has on-board RAM from 0x0 -- 0x00800000 (8Mb)
 * so, the SIO jumpers must be set to place it beyond RAM.
 * With other CPU boards this would not be so.
 * This card is an A24/D16 slave, factory setting is 0xb00000.
 * The following array holds the jumpered addresses, we let VxWorks
 * map them into logical addresses for us ( 1:1 for the vme147).
 */

char *boards[] = { (char *) 0xb00000, (char *) 0xb00200, (char *) 0xb00400 };

#define BSPACE	VME_AM_STD_USR_DATA

/* This structure covers a whole SIO board */

struct sio {
/*000*/	struct bmpcc	sio_mpcc0;
/*040*/	struct bmpcc	sio_mpcc1;
/*080*/	struct bmpcc	sio_mpcc2;
/*0c0*/	struct bmpcc	sio_mpcc3;
/*100*/	struct bmpcc	sio_mpcc4;
/*140*/	struct bmpcc	sio_mpcc5;
/*180*/	struct pit	sio_pit;
/*1b6*/	/* END */
};

#define	OBUFSIZE  64
#define	IBUFSIZE  64

struct siochan {
	char	s_obuf[OBUFSIZE];
	char	s_ibuf[IBUFSIZE];
	struct bmpcc *s_uart;
	char	*s_optr;
	char	*s_oend;	/* past last character to go out */
	char	*s_iptr;
	int	s_flags;
};

struct siochan chtab[MAXCHAN];

#ifdef PARITY
#ifdef TEST
int perrs = 0;	/* count of parity errors */
int pchar = 0;	/* last bad parity character */
int ppos = 0;	/* location of last bad parity byte */
#endif
#endif

/* here are the flag bits in s_flags */
#define S_OPEN	0x8000	/* this channel is open (validly initialized) */
#define S_TBSY	0x4000	/* transmitter is busy (data in queue) */
#define S_WRAP	0x2000	/* input has wrapped around (circular buffer) */
#define S_RIO	0x1000	/* set if any Rx interrupts (debugging) */
#define S_CONN	0x0800	/* this channel is connected (via phw_init) */


/*************************************************************************
 * Definitions pertaining to the interrupt structure follow:
 *************************************************************************/

/* the boards are jumpered to use interrupt level 4 on the VME bus.
 * on the 147 system, interrupt levels are used as follows:
 *	level 1 = printer.
 *	level 3 = lance ethernet.
 *	level 4 = Zilog 8530 uart (tyCoDrv).
 *	level 5 = aux clock.
 *	level 6 = sys clock.
 * We use level 4, should coexist OK with the other uarts (4 channels).
 * During testing on 6-8-90 got one spurious interrupt to uninitialized
 * vector 24 of (0-255).  I suspect this was just noise.
 */

#define INT_LVL_SIO	4

/* special considerations apply in the assignment of interrupt vectors.
 * the 68561 data sheet gives the details, the issue is that vectors for
 * a lower priority interrupt may have 0's clobbered by 1's in the vector
 * for the higher priority interrupt (if they both come in together).
 * The solution is to allocate vectors so that hi pri vectors have 1's
 * only where the lo pri vectors also have ones.
 * RSIO is highest, then TSIO, lastly SSIO.
 * I have decided to assign all ports the same SSIO vector, since I intend
 * to just ignore these interrupts anyway.
 */

static char vtable[2][MAXCHAN] = {
  0x4a, 0x4b,  0x4c, 0x4d,  0x4e, 0x4f,  0x50, 0x51,  0x52, 0x53,  0x54, 0x55,
  0x56, 0x57,  0x58, 0x59,  0x5a, 0x5b,  0x5c, 0x5d,  0x5e, 0x5f,  0x60, 0x61,
  0x62, 0x63,  0x64, 0x65,  0x66, 0x67,  0x68, 0x69,  0x6a, 0x6b,  0x6c, 0x6d
};

#define INT_VEC_SIO	0x7f

static vused = 0;

/* Here is a routine to "dole-out" interrupt vectors.
 * It gets called during initialization once for each USART in the system.
 */
vec_alloc(pr,pt,ps)
char *pr, *pt, *ps;
{
	if ( vused >= MAXCHAN )
	    return ( 0 );
	*pr = vtable[0][vused];
	*pt = vtable[1][vused++];
	*ps = INT_VEC_SIO;	/* same for all channels (never enabled) */
	return (1);
}

/*************************************************************************
 * The following structure gives values needed to setup baud rates.
 * If you want to add other baud-rates to this set, take a look at
 * the data sheet, and be sure to set the prescaler divisor properly.
 * Also, the ecr is set here (to do, or not to do parity, as you wish).
 */

struct baudset {
	short	baud_ccr;
	short	baud_cnt2;
	short	baud_cnt1;
} baudtab[] = {
#define	B300	0
	PSCDV2|TXCOUT|RXCINT,	0x1a, 0x40,
#define	B1200	1
	PSCDV3|TXCOUT|RXCINT,	0x04, 0x60,
#define	B2400	2
	PSCDV2|TXCOUT|RXCINT,	0x034, 0x8,
#define	B9600	3
	PSCDV3|TXCOUT|RXCINT,	0x00, 0x8c,
#define	B19200	4
	PSCDV3|TXCOUT|RXCINT,	0x00, 0x46,
#define	B38400	5
	PSCDV3|TXCOUT|RXCINT,	0x00, 0x23
};

/**************************************************
 * Here are the "user-accessible" entry points:
 *	phw_reset()
 *	phw_init(chan)
 *	wrpanel(chan,buf)
 *	rdpanel(chan,buf)
 **************************************************/

static int sio_first = 1;

/* call this before phw_init to do a re-initialization.
 * A nice idea, but this simple approach doesn't work.
 * In fact, it crashes the system - the reason is that interrupts
 * are actively happening (in particular from the ADC) and if we
 * reinstall the interrupt handler while that is going on, the interrupt
 * enters who knows what code and .....
 */
phw_reset()
{
	sio_first = 1;
}

#ifdef TEST
	adc_init() {}
#endif

phw_init(chan)
{
	if ( sio_first ) {
	    adc_init();		/* hook to the BB adc driver */
	    sio_init(0);	/* no verbose messages */
	    sio_first = 0;
	}
	if ( chan < 0 || chan >= MAXCHAN )
	    return ( 1 );
	if ( ! (chtab[chan].s_flags & S_OPEN) )
	    return ( 2 );
	if ( chtab[chan].s_flags & S_CONN )
	    return ( 3 );
	chtab[chan].s_flags |= S_CONN;
	return ( 0 );
}

wrpanel(chan,buf)
char *buf;
{
	if ( badchan(chan) )
	    return (1);
	sio_send(&chtab[chan],buf);
	return (0);
}

rdpanel(chan,buf)
char *buf;
{
	register struct siochan *cptr;
	register char *p;

	if ( badchan(chan) ) {
	    *buf = '\0';
	    return (1);
	}

	cptr = &chtab[chan];
	cptr->s_uart->rier &= ~RIE_DA;	/* no receiver interrupts */
	if ( cptr->s_flags & S_RIO ) {
	    if ( cptr->s_flags & S_WRAP ) {
		for ( p=cptr->s_iptr; p<&cptr->s_ibuf[IBUFSIZE]; )
			*buf++ = *p++;
		for ( p=cptr->s_ibuf; p<cptr->s_iptr; )
			*buf++ = *p++;
	    } else {
		for ( p=cptr->s_ibuf; p<cptr->s_iptr; )
			*buf++ = *p++;
	    }
	    *buf = '\0';
	    cptr->s_flags &= ~(S_RIO|S_WRAP);
	    cptr->s_iptr = cptr->s_ibuf;
	} else {
	    *buf = '\0';
	}
	cptr->s_uart->rier |= RIE_DA;	/* OK, allow interrupts */
	return (0);
}

badchan(chan)
{
	if ( chan < 0 || chan >= MAXCHAN )
	    return ( 1 );
	if ( ! (chtab[chan].s_flags & S_OPEN) )
	    return ( 1 );
	return ( 0 );
}

/**************************************************
 * internal code follows:
 **************************************************/


/* Handler for transmitter interrupts.
 * The transmitter has an 8 byte FIFO, so we can endeavor to really
 * cram it full, and thus reduce the number of interrupts we see.
 */
sio_tint(cptr)
register struct siochan *cptr;
{
	register struct bmpcc *uart;

	uart = cptr->s_uart;
	while ( uart->tsr & TSR_TDRA ) {
	    if ( cptr->s_optr < cptr->s_oend )
		uart->tdat = *cptr->s_optr++;
	    else {
		uart->tier &= ~TIE_DRA;		/* disable interrupts */
		cptr->s_flags &= ~S_TBSY;	/* done ! */
		break;
	    }
	}
}

/* handler for receiver interrupts. */
sio_rint(cptr)
register struct siochan *cptr;
{
	register struct bmpcc *uart;
#ifdef PARITY
	char junk;
#endif

	uart = cptr->s_uart;
	cptr->s_flags |= S_RIO;
	while ( uart->rsr & RSR_RDA ) {
#ifdef PARITY
	    if ( uart->rsr & RSR_CPERR ) {
#ifdef TEST
		++perrs;
		pchar = uart->rdat & 0x7f;
		ppos = cptr->s_iptr - cptr->s_ibuf;
#else
		junk = uart->rdat;	/* throw it away */
#endif
		uart->rsr = RSR_CPERR;	/* clear the error */
	    } else
		*cptr->s_iptr++ = uart->rdat & 0x7f;
#else
	    *cptr->s_iptr++ = uart->rdat & 0x7f;
#endif
	    if ( cptr->s_iptr >= &cptr->s_ibuf[IBUFSIZE] ) {
		cptr->s_iptr = cptr->s_ibuf;
		cptr->s_flags |= S_WRAP;
	    }
	}
}

/* handler for status interrupts. */
sio_sint(cptr)
register struct siochan *cptr;
{
	/* We just ignore these */
}

/* start sending a string by enabling transmitter interrupts
 */
sio_start(cptr)
register struct siochan *cptr;
{
	if ( cptr->s_optr < cptr->s_oend ) {
	    cptr->s_uart->tier |= TIE_DRA;	/* enable interrupts */
	    cptr->s_flags |= S_TBSY;
	}
}

/* sio_send -- queue up a message to be transmitted.
 * We make some serious simplifying assumptions here, the main one
 * is that any previous strings are gone by the time this routine
 * gets called. If this is violated, we don't check or care, we just
 * throw away any not-yet-sent part and post the new string to go out.
 */
sio_send(cptr,ubuf)
register struct siochan *cptr;
register char *ubuf;
{
	register char *p;
	register lim;

	cptr->s_uart->tier &= ~TIE_DRA;	/* disable xmitter interrupts */
	lim = OBUFSIZE;
	p = cptr->s_optr = cptr->s_obuf;
	while ( *ubuf && lim-- )
	    *p++ = *ubuf++;
	cptr->s_oend = p;
	sio_start(cptr);
}

#ifdef TEST
siotest(chan)
{
	char obuf[64];
	char ibuf[64];
	long wait;
	int n;
	int estat;

	phw_reset();
	if ( estat = phw_init(chan) ) {
	    printf("cannot initialize channel %d",chan);
	    printf(" code: %d\n",estat);
	    return;
	}
	printf("testing channel %d\n",chan);
	for ( n=0; n<10; n++ ) {
	    sprintf(obuf,"This is test string %d\n",n);
	    wrpanel(chan,obuf);
	    /* poll till transmitter clears */
	    while ( chtab[chan].s_flags & S_TBSY )
		;
	    /* at this stage, our buffer is empty, but up to 8 bytes
	     * may be in the transmitter fifo, wait for them.
	     */
	     for ( wait = 10000; wait; --wait )
		;
	    rdpanel(chan,ibuf);
	    if ( strcmp(obuf,ibuf) ) {
		printf("test %d failed\n",n);
		printf("wrote: %s",obuf);
		printf("read : %s\n",ibuf);
	    }
	}
	printf("%d tests completed\n",n);
}

sio(chan)
{
	sio_init(1);
	printf("interactive on channel %d\n",chan);
	user(chan);
}

#define LSIZE 81

user(chan)
{
	char line[LSIZE];
	char cmd;
	char *p;
	int iochar;
	long n;
	register struct bmpcc *uart;

	setjmp(userenv);	/* allow errors to reenter here */
	if ( ! (chtab[chan].s_flags & S_OPEN) ) {
	    printf("channel %d not open\n",chan);
	    return;
	}
	uart = chtab[chan].s_uart;

	for ( ;; ) {
	    printf("Sio> ");
	    fflush(stdout);
	    if ( gets(line) == NULL )
		break;
	    if ( line[0] == '\0' )
		continue;	/* blank line */
	    sscanf(line," %c",&cmd);
	    if ( cmd == 'q' ) {
		break;
	    } else if ( cmd == 'r' ) {
		iochar = 0;
		if ( chtab[chan].s_flags&S_RIO )
		    printf("Rx interrupts have happened\n");
#ifdef PARITY
		if ( perrs ) {
		    printf("%d parity errors\n",perrs);
		    printf("last one was %02x (at %d)\n",pchar&0xff,ppos);
		}
#endif
		if ( chtab[chan].s_flags&S_WRAP ) {
		    printf("Buffer has wrapped\n");
		    for (p=chtab[chan].s_iptr;p<&chtab[chan].s_ibuf[IBUFSIZE];)
			putchar ( *p++ );
		    for ( p = chtab[chan].s_ibuf; p<chtab[chan].s_iptr; )
			putchar ( *p++ );
		} else {
		    iochar = chtab[chan].s_iptr - chtab[chan].s_ibuf;
		    printf("Buffer has %d characters\n",iochar);
		    for ( p = chtab[chan].s_ibuf; p<chtab[chan].s_iptr; )
			putchar ( *p++ );
		}
		if ( chtab[chan].s_flags&S_WRAP || iochar )
		    putchar ( '\n' );
	    } else if ( cmd == 'd' ) {
		iochar = 0;
		if ( chtab[chan].s_flags&S_RIO )
		    printf("Rx interrupts have happened\n");
#ifdef PARITY
		if ( perrs )
		    printf("%d parity errors\n",perrs);
#endif
		if ( chtab[chan].s_flags&S_WRAP ) {
		    printf("Buffer has wrapped\n");
		    for (p=chtab[chan].s_iptr;p<&chtab[chan].s_ibuf[IBUFSIZE];)
			printf ( "%02x", *p++&0xff );
		    for ( p = chtab[chan].s_ibuf; p<chtab[chan].s_iptr; )
			printf ( "%02x", *p++&0xff );
		} else {
		    iochar = chtab[chan].s_iptr - chtab[chan].s_ibuf;
		    printf("Buffer has %d characters\n",iochar);
		    for ( p = chtab[chan].s_ibuf; p<chtab[chan].s_iptr; )
			printf ( "%02x", *p++&0xff );
		}
		if ( chtab[chan].s_flags&S_WRAP || iochar )
		    putchar ( '\n' );
	    } else if ( cmd == 't' ) {
		perrs = 0;
		for ( n=100; n--; ) {
		    sio_send(&chtab[chan],"Hurrah, hurrah !!\r\n");
		    while ( chtab[chan].s_optr < chtab[chan].s_oend )
			;
		}
	    } else if ( cmd == 'v' ) {
		printf("testing tiv\n");
		vtest(&uart->tiv);
		printf("testing riv\n");
		vtest(&uart->riv);
		printf("testing siv\n");
		vtest(&uart->siv);
	    } else if ( cmd == 's' ) {
		printf("tsr = %02x\n",uart->tsr&0xff);
		printf("rsr = %02x\n",uart->rsr&0xff);
		printf("ssr = %02x\n",uart->sisr&0xff);
	    } else if ( cmd == 'p' ) {
		for ( ;; ) {
		    for ( iochar='a'; iochar<='z'; ++iochar ) {
		    	while ( ! (uart->tsr&TSR_TDRA) )
			    ;
			uart->tdat = iochar;
		    }
		    while ( ! (uart->tsr&TSR_TDRA) )
			;
		    uart->tdat = 0x0d;
		    while ( ! (uart->tsr&TSR_TDRA) )
			;
		    uart->tdat = 0x0a;
		}
	    } else if ( cmd == 'i' ) {
      iochar = uart->rdat;
      printf("rdat = %02x\n",iochar&0xff);
    } else if ( cmd == 'o' ) {
      if ( sscanf(line," o %x",&iochar) != 1 )
	iochar = 0x55;		/* U = 0101 */
      uart->tdat = iochar;
    } else if ( cmd == 'l' ) {
      iochar = 0x55;			/* U */
      for ( n = 500; n--; ) {
	while ( ! (uart->tsr&TSR_TDRA) )
	  ;
        uart->tdat = iochar;
      }
    } else if ( cmd == 'e' ) {
      for ( n=100; n--; ) {
	while ( ! (uart->rsr&RSR_RDA) )
	  ;
        iochar = uart->rdat;
	printf("%02x",iochar&0xff);
	if ( n%20 == 0 )
	  printf("\n");
      }
      printf("\n");
    } else if ( cmd == 'h' ) {
	if ( sscanf(line," h %x",&iochar) != 1 )
	    uart->sicr = (SI_RTS|SI_DTR);
	else
	    uart->sicr = iochar;
    } else
      printf("Don't know: >%c<\n",cmd);
  }
}  /* END of user() */

vtest(vr)
register char *vr;
{
	char vsave;
	register i;
	register test;

	vsave = *vr;
	printf("found %02x\n",vsave&0xff);
	for ( i=0; i<8; i++ ) {
	    *vr = test = (1<<i)&0xff;
/*	    printf("Test with %02x\n",test);	*/
	    if ( (*vr)&0xff != test )
		printf("wrote %02x, read %02x\n",test&0xff,*vr&0xff);
	    *vr = test = (~test)&0xff;
	    if ( (*vr)&0xff != test )
		printf("wrote %02x, read %02x\n",test&0xff,*vr&0xff);
	}
	*vr = vsave;
	printf("done\n");
}
#endif TEST

/* sio_init - hardware initialization for the Force-SIO.
 * At present, this initializes all channels on a single board.
 * Will need changes to support multiple boards.
 */
sio_init(verbose)
{
	char *jp;
	struct sio *bp;
	register struct pit *pitbase;
	register i, j;
	char probe;

	/* should take care here if we are reinitializing active channels
	 * for some reason.  I really don't expect that, but I make some
	 * effort to keep things sane if it should. (but what if a channel
	 * generating interrupts gets lost in the shuffle?).
	 */
	vused = 0;	/* restart the vector allocator */
	for ( i=0; i<MAXCHAN; i++ )
	    chtab[i].s_flags = 0;

	sysIntDisable(INT_LVL_SIO);	/* turn off VME interrupts at CPU */

	for ( i=0; i<MAXBOARDS; i++ ) {
	    /* let VxWorks map the board base address for us,
	     * then see if it is really there.
	     */
	    jp = boards[i];	/* jumpered address */
	    if ( sysBusToLocalAdrs(BSPACE,jp,&bp) == ERROR ) {
#ifdef TEST
		printf("Cannot map address for SIO jumpered at %08x\n",jp);
#endif
		continue;
	    } else if ( vxMemProbe(&bp->sio_pit.pivr,READ,1,&probe) != OK ) {
#ifdef TEST
		printf(" *NO* sio at address: %08x",bp);
		printf(" (jumpered at %08x)\n",jp);
#endif
		continue;
	    }
#ifdef TEST
	    if ( verbose ) {
		printf("found sio at address: %08x",bp);
		printf(" (jumpered at %08x)\n",jp);
/*		printf("probe found %02x\n",probe&0xff);	*/
	    }
#endif

	    /* Initialize the PIT chip on this board.
	     * We leave it alone for the most part.
	     * the state after reset is suitable except for data directions.
	     * Ports A and B come up as 8-bit unidirectional with single
	     * buffered outputs, which is just dandy.
	     */
	    pitbase = &bp->sio_pit;
	    pitbase->paddr = DD_OUTS;	/* port A must be output pins */
	    pitbase->pbddr = DD_OUTS;	/* port B must be output pins */
	    pitbase->padata = RCV_ON;
	    pitbase->pbdata = (USE_232|NO_OPTO);

	    chinit ( chmap[i][0], &bp->sio_mpcc0 );
	    chinit ( chmap[i][1], &bp->sio_mpcc1 );
	    chinit ( chmap[i][2], &bp->sio_mpcc2 );
	    chinit ( chmap[i][3], &bp->sio_mpcc3 );
	    chinit ( chmap[i][4], &bp->sio_mpcc4 );
	    chinit ( chmap[i][5], &bp->sio_mpcc5 );
	}

	/* enable VMEbus interrupts at the level our cards are jumpered at.
	 * Note that, at each level, VMEbus interrupts are a separate
	 * entity and of lowest priority (see the 147 manual p. 5-16).
	 */
	sysIntEnable(INT_LVL_SIO);

	/* turn on receiver interrupts for all open channels */
	for ( i=0; i<MAXCHAN; i++ )
	    if ( chtab[i].s_flags & S_OPEN )
		chtab[i].s_uart->rier |= RIE_DA;

#ifdef TEST
	if ( verbose )
	    printf("done with initialization\n");
#endif
}

chinit(chan,uart)
register struct bmpcc *uart;
{
	register struct wmpcc *wuart;
	char rint, tint, sint;

	/* These crazy USART's can be used in either 8 or 16 bit mode.
	 * You get all messed up if you try to talk to it in one mode
	 * and it is really in another, and since we are never really
	 * sure what state it is in when we enter this routine,
	 * we first put it in 8-bit mode.
	 * (That works regardless of the state it's in.)
	 * Then if we want to (but we don't) we can go to 16-bit mode.
	 */

	wuart = (struct wmpcc *) uart;	/* ugly, but works */
	wuart->mpcc_prot = WPR_BY;	/* might be in 16 bit mode */

	set_baud(uart,B9600);

	/* in case someone is using a Null-modem loopback to CTS */
	/* fix 8/17/93 -- need to delay this a bit before sampling it,
	 * with gcc2 the code is fast enough to get there before the
	 * signal is valid (must be long oven cables...).
	 */
	uart->sicr = (SI_RTS|SI_DTR);	/* assert handshake lines */
	uart->sicr = (SI_RTS|SI_DTR);	/* assert handshake lines */
	uart->sicr = (SI_RTS|SI_DTR);	/* assert handshake lines */
	uart->sicr = (SI_RTS|SI_DTR);	/* assert handshake lines */
	/*  Note: pin 20 (DB-25) is connected to CTS
	 *  and must be asserted to transmit.  */
	if ( ! (uart->sisr&SS_CTS) ) {
#ifdef TEST
	    printf("sio channel %d, no CTS\n",chan);
#endif
	    return;
	}

	/* get the interrupt vectors and load them up */
	if ( ! vec_alloc(&rint,&tint,&sint) ) {
#ifdef TEST
	    printf("sio channel %d, cannot get interrupt vectors\n",chan);
#endif
	    return;
	}
	uart->riv = rint;
	uart->tiv = tint;
	uart->siv = sint;

	uart->rier = 0;	/* be sure nothing is enabled */
	uart->tier = 0;
	uart->sier = 0;

	/* this passes the interrupt routine the address of the driver
	 * data structure for the channel interrupting */
	(void) intConnect ( INUM_TO_IVEC(rint), sio_rint, &chtab[chan] );
	(void) intConnect ( INUM_TO_IVEC(tint), sio_tint, &chtab[chan] );
	(void) intConnect ( INUM_TO_IVEC(sint), sio_sint, &chtab[chan] );

	chtab[chan].s_uart = uart;	/* hardware base address */
	chtab[chan].s_optr = chtab[chan].s_obuf;
	chtab[chan].s_oend = chtab[chan].s_obuf;
	chtab[chan].s_iptr = chtab[chan].s_ibuf;
	chtab[chan].s_flags = S_OPEN;

	/* added this 6/25/91 -- probably needless, but want to reset
	 * things when the initialization is all done. */
	uart->tcr = TCR_RESET;
	uart->tcr = TCR_ENA;
	uart->rcr = RCR_RESET;
	uart->rcr = 0;

	/* !! Made it! */
}

set_baud(uart,bpick)
register struct bmpcc *uart;
{
	/* we don't need to set psr1 for asynchronous modes */
	uart->psr2 = PR2_ASYNC;
	uart->ccr   = baudtab[bpick].baud_ccr;
#ifdef PARITY
	uart->ecr   = (EC_PAR|EC_EVEN|EC_CRC0);	/* even parity */
#else
	uart->ecr   = EC_CRC0;	/* no parity, 0 preset CRC */
#endif
	uart->baud1 = baudtab[bpick].baud_cnt1;
	uart->baud2 = baudtab[bpick].baud_cnt2;

	/* must reset rcvr & xmtr after setting protocol */
	uart->tcr = TCR_RESET;
	uart->tcr = TCR_ENA;
	uart->rcr = RCR_RESET;
	uart->rcr = 0;
}

#ifdef OLD
/* I doubt that this will work anymore. */
sio_setup()
{
	register struct bmpcc *uart;
	register struct wmpcc *wuart;
	register struct pit *pitbase;
	struct sio *bp;
	int chan;
	int always = 1;
	int first = 1;
	long i;

	pitbase = &bp->sio_pit;

	/* We can leave the PIT chip alone for the most part.
	 * the state after reset is suitable except for data directions.
	 * Ports A and B come up as 8-bit unidirectional with single
	 * buffered outputs, which is just dandy.
	 */
	pitbase->paddr = DD_OUTS;	/* port A must be output pins */
	pitbase->pbddr = DD_OUTS;	/* port B must be output pins */
	pitbase->padata = RCV_ON;
	pitbase->pbdata = (USE_232|NO_OPTO);

	for ( chan=0; chan<MAXCHAN; ++chan )  {
	    uart = chtab[chan].s_uart;
	    wuart = (struct wmpcc *) uart;	/* ugly */
	    wuart->mpcc_prot = WPR_BY;		/* might be in 16 bit mode */
#ifdef SHAKELOOP
	    /* scope loop on the RTS and DTR pins.
	     * notice that the RTS pin kind of does it's own thing.
	     * It stays asserted, even when told to deassert till any
	     * character it thinks it is (or might be) transmitting
	     * gets out of the transmitter.
	     */
	    while ( always ) {
		uart->sicr = 0x00;
		uart->sicr = (SI_RTS|SI_DTR);
	    }
#endif
	    set_baud(uart,B9600);
	    uart->sicr = (SI_RTS|SI_DTR);	/* assert handshake lines */
	    if ( chan == 0 )
		set_int ( chan, INT_VEC_RSIO, INT_VEC_TSIO, INT_VEC_SSIO );

	    /* enable VMEbus interrupts at the level our card is jumpered at.
	     * Note that, at each level, VMEbus interrupts are a separate
	     * entity and of lowest priority (see the 147 manual p. 5-16).
	     */
	    sysIntEnable(INT_LVL_SIO);

#ifdef SHAKETEST
	    /* wiggle the DTR pin and see if we see the response on
	     * the DSR pin, this requires a jumper from DSR to DTR.
	     */
	    if ( first ) {
		printf("begin DSR/DTR test\n");
		for ( i=0; i<1000000; i++ ) {
		    if ( (uart->sisr & SS_DSR) != SS_DSR )
			printf("DSR not asserted\n");
		    uart->sicr = (SI_RTS);
		    if ( (uart->sisr & SS_DSR) == SS_DSR )
			printf("DSR asserted\n");
		    uart->sicr = (SI_RTS|SI_DTR);
		}
		printf("end of DSR/DTR test\n");
		first = 0;
	    }
#endif
	}
}
#endif OLD

#ifdef TEST
uerror(s,n)
char *s;
{
    printf(s,n);
    printf("\n");
    longjmp(userenv,1);
}

/* error - print error string and exit (to where??) */
error(s)
char *s;
{
  printf("%s\n",s);
  exit(1);
}
#endif TEST

/* EOF */
