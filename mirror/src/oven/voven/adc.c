#include "vxWorks.h"
#include "iosLib.h"
#include "semLib.h"
#include "vme.h"
#include "iv.h"
#include "stdioLib.h"

/* Driver for the Burr-Brown MVP-906 analog input board.
 *
 * You can buy a "digital i/o" option for this board, we have not done
 *  so, and so this driver does not support it.
 *
 *  We used another BB board called the MVP-901, only on the Vatican
 *  stressed lap, it is much different than the MVP-906.
 *  This driver is *ONLY* for the MVP-906 board.
 */

#define CR1_IEN	0x8000	/* enable all interrupts */
#define CR1_IRQ	0x7000	/* mask for interrupt line (1-7) */
#define CR1_CON	0x0800	/* continuous (0=transient) */
#define CR1_TRA	0x0000	/* transient */
#define CR1_IOV	0x0400	/* interrupt on oversampling */
#define CR1_IEA	0x0100	/* interrupt on each conversion */
#define CR1_IHF	0x0200	/* interrupt on half full and full */
#define CR1_IFU	0x0300	/* interrupt on full */
#define CR1_VEC	0x00ff	/* interrupt vector mask */

#define ST2_FULL  0x4000 /* data buffer full */
#define ST2_HALF  0x2000 /* data buffer half full */
#define ST2_OVER  0x1000 /* 0 = over-sampled */
#define ST2_VALID 0x0800 /* 0 = valid data in buffer */
#define ST2_BUSY  0x0400 /* 0 = busy, 1 = done */
#define ST2_SE    0x0200 /* 0 = differential, 1 = single ended */
#define ST2_SPTR  0x003f /* mask for scan list pointer */

#define CR3_READ  0x0000 /* reading data initiates conversions */
#define CR3_TIMER 0x0001 /* timer controls conversions */
#define CR3_EXT   0x0002 /* external trigger controls conversions */
#define CR3_EVENT 0x0003 /* trigger, then timer conversions */
#define CHMASK  0x1f	/* mask address to range 0-31 */

#define NSCAN	32	/* size of the scan list */
#define NCHAN	64	/* size of the scan data buffer */

/* this is a template for register addresses on the MPV-906
 */
struct bb906 {
	short data[NCHAN];
	short scan[NSCAN];
	short digio[4];		/* digital i/o (not present) */
	short pad[26];
	short cr4;
	short cr3;
	short cr1;
	short cr2;
};
 

/*************************************************************************
 * We plan to have 2 boards per CPU in the oven,
 * each handling 32 differential channels.
 * These addresses should be fine for the mv147 address map (A24/D16)
 */
#define NBOARDS		2
#define MAXCHAN		63	/* we have 0-63 channels on two boards */

struct adcio {
	char *jbase;		/* jumpered board base address */
	struct bb906 *bbase;	/* mapped board address */
} bmap[] = 
#ifdef BOGUS
	{	(char *) 0x00fff000, (struct bb906 *) 0,
		(char *) 0x00fff400, (struct bb906 *) 0 };
#else
	{	(char *) 0x00d00000, (struct bb906 *) 0,
		(char *) 0x00d00200, (struct bb906 *) 0 };
#endif

#define VMEADR		VME_AM_STD_USR_DATA
#define VEC_ADC		0x81
#define INT_LVL_ADC	4

/* adc_init - Initialize access to the BB A/D board.
 */
adc_init()
{
	short probe;
	register board;
	struct bb906 *bbase;

	for ( board=0; board<NBOARDS; ++board ) {
	    if ( sysBusToLocalAdrs(VMEADR,bmap[board].jbase,&bbase) == ERROR ) {
/*	    	printf("Cannot map addr for BB AtoD board\n");	*/
		continue;
	    } else if ( vxMemProbe(&bbase->cr2,READ,2,&probe) != OK ) {
/*	    	printf(" *NO* adc at address: %08x",bbase);
		printf(" (jumpered at %08x)\n",bmap[i].jbase);	*/
		continue;
	    }
/*	    printf("found adc at address: %08x",bbase);
	    printf(" (jumpered at %08x)\n",bmap[i].jbase);
	    printf("probe found %04x\n",probe&0xffff);
*/

	    bmap[board].bbase = bbase;
	    adc_all(board);
	    bbase->cr1 = (CR1_CON);
	    bbase->cr3 = CR3_TIMER;
	    bbase->cr4 = 5;		/* 30 microsecond timer */
	    bbase->cr2 = 0;		/* pull out the stops */
	    while ( ! (bbase->cr2 & ST2_FULL) )	/* wait for things to start */
		;
	    while ( bbase->cr2 & ST2_FULL )	/* wait for first load */
		;
	}
	return;
}

/* adc_all - set up the scan-list to convert all channels in sequence. */
adc_all(board)
{
	register i;
	register chan;

	chan = 0;
	for ( i=0; i<NSCAN; i++ ) {
	    bmap[board].bbase->scan[i] = ((chan+1)<<8 | chan);
	    chan += 2;
	}
}

#ifdef BOGUS
btest(chan)
{
	int buf[100];
	register i, j;

	for ( i=0; i<100; ++i )
	    rdadc(chan,&buf[i]);

	for ( i=0, j=0; i<100; ) {
	    printf(" %04x",buf[i++]);
	    if ( j++ == 9 ) {
		printf("\n");
		j = 0;
	    }
	}
	if ( j )
	    printf("\n");
}
#endif

/* rdadc -- this is the OFFICIAL user entry point
 */
rdadc(chan,value)
register chan;
int *value;
{
	register board;
	register struct bb906 *bbase;

	if ( chan < 0 || chan >= MAXCHAN )
	    *value = -1;
	board = (chan&0x20) ? 1 : 0;
	chan &= 0x1f;

	/* mapped address is zero if initialization failed */
	if ( bbase = bmap[board].bbase )
	    *value = bbase->data[chan] & 0xfff;
	else
	    *value = -1;
	return;
}

#ifdef BOGUS

/********************************************************
 * OLD, experimental, and other stuff follows:
 */

SEM_ID	adcsem;
int adccount = 0;

/* adc_int - interrupt handler */
adc_int(arg)
{
	semGive(adcsem);
	++adccount;
}

/* adc_inter - do initialization for interrupt stuff
 */
adc_inter(board)
{
	if ( (adcsem=semBCreate(SEM_EMPTY, SEM_Q_FIFO)) == NULL ) {
	    printf("Cannot get semaphore for ADC driver\n");
	    return;
	}

	sysIntDisable(INT_LVL_ADC);	/* turn off VME interrupts at CPU */
	(void) intConnect ( INUM_TO_IVEC(VEC_ADC), adc_int, 0 );
	sysIntEnable(INT_LVL_ADC);
}

adc_clean()
{
	semDelete(adcsem);
}

/* adc_stat - print status, for debugging */
adc_stat()
{
	printf("status %04x\n",bbase->cr2);
	if ( adccount ) {
	    printf("%d new interrupts\n",adccount);
	    adccount = 0;
	}
}

/* adc_batch - setup to do timer-triggered batch conversions */
adc_batch()
{
	adc_all();
	bbase->cr1 = CR1_TRA;	/* no interrupts, not continuous */
	bbase->cr3 = CR3_TIMER;	/* timer controls sampling */
	bbase->cr4 = 5;	/* period is (5us) * (cr4+1) == 30us */
}

#define INTOFF	(CR1_CON|CR1_IEA|VEC_ADC | INT_LVL_ADC<<12)
#define INTON	(CR1_IEN|INTOFF)

/* rdqk -- experimental - want to read one doober as fast as we can.
 */
rdqk(chan)
int chan;
{
	short sbuf[128];
	register val;
	register nbsy;
	register i, j, n;
	register stat;

	printf("entering rdadc ");
	adc_stat();
	chan &= 0x3f;
	/* low byte in the following is what counts */
	bbase->scan[0] = (chan<<8 | chan);
	bbase->cr1 = INTOFF;
	bbase->cr3 = CR3_TIMER;

	printf("prior to softw reset ");
	adc_stat();
	bbase->cr1 = INTON;
	bbase->cr2 = 0xff;		/* software reset */
	semTake(adcsem);	/* wait for interrupt */
	bbase->cr1 = INTOFF;
	val = bbase->cr2;
	printf("status after interrupt: %04x",val&0xffff);
	printf("after sample interrupt ");
	adc_stat();

	val = bbase->data[0];	/* discard, start conversion */
#ifdef NOWAY
	semTake(adcsem);	/* wait for interrupt */
#endif
/*
	while ( bbase->cr2 & ST2_VALID )
	    ++nbsy;
	printf("nbsy = %d\n",nbsy);
	for ( n=0; n<128; ) {
	    stat = bbase->cr2;
	    sbuf[n++] = stat;
	    if ( ! (stat & ST2_VALID) )
		break;
	}
	for ( i=0, j=0; i<n; ) {
	    printf(" %04x",sbuf[i++]&0xffff);
	    if ( j++ == 9 ) {
		printf("\n");
		j = 0;
	    }
	}
	if ( j )
	    printf("\n");
	while ( ! (bbase->cr2 & ST2_VALID) )
	    ++nbsy;
	printf("nbsy = %d\n",nbsy);
*/
	bbase->cr1 = INTOFF;
	return ( bbase->data[0] & 0xfff );
}

/* adc_one - set up the scan-list to convert a single channel over and over.
 */
adc_one(chan)
{
	register i;

	chan &= 0x3f;
	for ( i=0; i<NSCAN; i++ )
	    bbase->scan[i] = (chan<<8 | chan);
}

/* adc_test - for interactive/debugging use only.
 *	    collect and display a batch of conversions.
 */
adc_test()
{
	short test[NCHAN];
	register i;

	printf("status %04x\n",bbase->cr2);
	bbvals(test);
	for ( i=0; i<NCHAN; i += 4 )
	    printf(" %8d %8d %8d %8d\n",test[i],test[i+1],test[i+2],test[i+3]);
}

/* adc_show - for interactive/debugging use only.
 *	    display the scan list - must have previously collected the data
 *	    by typing "adc_go".
 */
adc_show()
{
	register i;
	short test[NCHAN];

	printf("status %04x\n",bbase->cr2);
	for ( i=0; i<NCHAN; i++ )
	    test[i] = bbase->data[i] & 0xfff;
	for ( i=0; i<NCHAN; i += 4 )
	    printf(" %8d %8d %8d %8d\n",test[i],test[i+1],test[i+2],test[i+3]);
}

/* adc_go - for interactive/debugging use only.
 *	  begin a batch of conversions, then use adc_show to look at them.
 *	  no polling, but it will take you longer to type "adc_show" than
 *	  the board needs to get the batch.
 */
adc_go()
{
	printf("status %04x\n",bbase->cr2);
	bbase->cr2 = 0;		/* any write starts conversions */
	printf("status %04x\n",bbase->cr2);
}

/* adc_vals - collect a full batch of conversions.
 *	    This is the recommended user interface when in transient mode
 *	    with the timer providing an internal trigger.
 */
adc_vals(buffer)
short *buffer;
{
	register i;

	adc_all();
	bbase->cr1 = CR1_TRA;	/* no interrupts, not continuous */
	bbase->cr3 = CR3_TIMER;	/* timer controls sampling */
	bbase->cr4 = 5;	/* period is (5us) * (cr4+1) == 30us */

	bbase->cr2 = 0;		/* any write to cr2 starts conversions */
	while ( ! (bbase->cr2 & ST2_FULL) )	/* wait for action to start */
	    ;
	/* FULL goes zero when all conversions are done */
	while ( bbase->cr2 & ST2_FULL )
	    ;
	for ( i=0; i<NCHAN; i++ )
	    buffer[i] = bbase->data[i] & 0xfff;
}
#endif BOGUS
