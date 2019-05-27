#include <vxWorks.h>
#include <iosLib.h>
#include <vme.h>
#include <stdioLib.h>
#include <semLib.h>
#include <taskLib.h>

#include "opio.h"

/* Library routines for support of the temperature data gathering 
 * routines for OVEN Level 1 code.
 *
 * Written by: D. A. Harvey
 * Date:       2/20/1990
 */

/* Hardware initialization routine.  Initializes all hardware associated
 * with a dual pio pair associated with a single dcu - should be called 
 * once at system boot
 */
pio_init(dcu)
int dcu;
{

    int board= dcu / NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    int card;
    register struct pit *ppit;
    int error= 0;
        
#ifndef CPU_FORCE
    if ( sysBusToLocalAdrs (VME_AM_STD_USR_DATA, BASE, &opiobase) == ERROR )
        error |= 0x40;
#else
/* bug in VxWorks for the Force board */
    opiobase = (char *) (0xfc000000 + BASE);
#endif

    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);
    
    /* initialize both pits associated with passed dcu */
    if (pitinit ( ppit ))
	error |= 0x80;
    if (pitinit ( (struct pit *) ((char *) ppit + 1) ))
	error |= 0x100;
    
    return (error);
} 
db_pio_init(dcu)
int dcu;
{

    int board= dcu / NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    int card;
    register struct pit *ppit;
    int error= 0;
        
#ifndef CPU_FORCE
    if ( sysBusToLocalAdrs (VME_AM_STD_USR_DATA, BASE, &opiobase) == ERROR )
        error |= 0x40;
#else
/* bug in VxWorks for the Force board */
    opiobase = (char *) (0xfc000000 + BASE);
#endif

    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);
    
    /* initialize both pits associated with passed dcu */
    if (db_pitinit ( ppit ))
	error |= 0x80;
    if (db_pitinit ( (struct pit *) ((char *) ppit + 1) ))
	error |= 0x100;
    
    return (error);
} 

/* Hardware initialization routine.  Initializes all hardware associated
 * with a single DCU - including the PIO pits, Bus Interface Card (DCU), 
 * Timers, Channel Select, and Counter Cards.  Can be called anytime an
 * integration is NOT in progress.
 */
thw_init(dcu)
int dcu;
{

    int board= dcu / NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    int card;
    register struct pit *ppit;
    int error= 0;
        
    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);
    
    /* initialize the DCU 8253 Timers 0 - 2 */
    /* timer 0 is ganged to counter 1. The pair
     * generate a retriggerable one-shot with a
     * period of 466.6667 milliseconds.
     */
    if (!dcuaddr ( ppit, DCUTIMER_MODE_REG ))
        error |= 1;
    if (!dcudata ( ppit, DCUTIMER0 | MODE3 | RLM ))
        error |= 2;
    if (!dcuaddr ( ppit, DCUTIMER0_LOAD_REG ))
        error |= 1;
    if (!dcudata ( ppit, LCOUNT0 ))
        error |= 2;
    if (!dcudata ( ppit, HCOUNT0 ))
        error |= 2;

    if (!dcuaddr ( ppit, DCUTIMER_MODE_REG ))
        error |= 1;
    if (!dcudata ( ppit, DCUTIMER1 | MODE1 | RLM ))
        error |= 2;
    if (!dcuaddr ( ppit, DCUTIMER1_LOAD_REG ))
        error |= 1;
    if (!dcudata ( ppit, LCOUNT1 ))
        error |= 2;
    if (!dcudata ( ppit, HCOUNT1 ))
        error |= 2;
    
    if (!dcuaddr ( ppit, DCUTIMER_MODE_REG ))
        error |= 1;
    if (!dcudata ( ppit, DCUTIMER2 | MODE3 | RLM ))
        error |= 2;
    if (!dcuaddr ( ppit, DCUTIMER2_LOAD_REG ))
        error |= 1;
    if (!dcudata ( ppit, LCOUNT2 ))
        error |= 2;
    if (!dcudata ( ppit, HCOUNT2 ))
        error |= 2;

    /* initialize the counter cards - data mode, test disable, gate enable */
    for (card = 0; card < NCOUNTERCARDS; card++) {
        if (!dcuaddr ( ppit, CNT_CARD_BASE + 1 + (card * CNT_CARD_OFFSET) )) 
            error |= 1;
        if (!dcudata ( ppit, 0 ))
            error |= 2;
    }

    /* disable timers 1 and 2 */
    if (!dcuaddr ( ppit, DCUTIMER_GATE_REG ))
        error |= 1;
    if (!dcudata ( ppit, TIMER1_DISABLE))
        error |= 2;

    /* globally clear all counter cards in DCU */
    if (!dcuaddr ( ppit, GLOBAL_COUNTER_CLR_REG ))
        error |= 1;    
    if (!dcudata ( ppit, 0))
        error |= 2;
        
    return ( error );
}


/* Routine to test the status of a timed integration of the counter
 * cards - should be called to test ``done'' status before a getcounts
 * call.
 */
tmstat (dcu, timeout, invalid )
int dcu;
int *timeout;
int *invalid;
{

    int board= dcu / NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    register struct pit *ppit;
    int timeout_count;
    int error =0;
        
    *timeout = YES;
    *invalid = YES;

    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);
    
    if (!dcuaddr ( ppit, DCUTIMER_STAT_REG ))
        error |= 1;

    for (timeout_count = 0; timeout_count < STOPTIMEOUT; timeout_count++) {
        if ((dcuread ( ppit, &error ) & TIMER1_RUNNING_MASK) != 0) {
            *timeout = NO;
            break;
        }
    }

    for (timeout_count = 0; timeout_count < STOPTIMEOUT; timeout_count++) {
        if ((dcuread ( ppit, &error ) & TIMER2_RUNNING_MASK) == 0) {
            *invalid = NO;
            break;
        }
    }
    
    return ( error );
}


/* Routine to test the status of a timed integration of the counter
 * cards - should be called to start and validate an integration.
 */
tmstart (dcu, timeout, invalid )
int dcu;
int *timeout;
int *invalid;
{

    int board= dcu/ NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    register struct pit *ppit;
    int timeout_count;
    int error= 0;
                
    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);
    

    *timeout = YES;
    *invalid = YES;

    /* globally clear all counter cards in DCU */
    if (!dcuaddr ( ppit, GLOBAL_COUNTER_CLR_REG ))
        error |= 1;
    if (!dcudata ( ppit, 0))
        error |= 2;

    if (!dcuaddr ( ppit, DCUTIMER_GATE_REG ))
        error |= 1;
    if (!dcudata ( ppit, TIMER1_DISABLE ))
        error |= 2;
    if (!dcuaddr ( ppit, DCUTIMER_GATE_REG ))
        error |= 1;
    if (!dcudata ( ppit, TIMER1_ENABLE ))
        error |= 2;

    if (!dcuaddr ( ppit, DCUTIMER_STAT_REG ))
        error |= 1;
    
    for (timeout_count = 0; timeout_count < STARTTIMEOUT; timeout_count++) {
        if ((dcuread ( ppit, &error ) & TIMER1_RUNNING_MASK) == 0) {
            *timeout = NO;
            break;
        }
    }
    
    for (timeout_count = 0; timeout_count < STOPTIMEOUT; timeout_count++) {
        if ((dcuread ( ppit, &error ) & TIMER2_RUNNING_MASK) == 0) {
            *invalid = NO;
            break;
        }
    }
    
    return ( error );
}


/* routine to test the address put out to a Channel Select card.
 */
getmux (dcu, select, buss, mux)
int dcu;
int select;
int buss;
int *mux;
{

    int board= dcu/ NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    register struct pit *ppit;
    int error= 0;
            
    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);

    if (!dcuaddr ( ppit, CS_BASE + (select * CS_OFFSET) + (buss / 2) ))
        error |= 1;
    *mux = (dcuread ( ppit, &error ) >> ((buss % 2) * 4)) & 0x07;

    return ( error );
}


/* routine to put out an address to a Channel Select card.
 */
putmux (dcu, select, buss, mux)
int dcu;
int select;
int buss;
int mux;
{

    int board= dcu/ NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    register struct pit *ppit;
    int error = 0;
            
    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);

    if (!dcuaddr ( ppit, CS_BASE + (select * CS_OFFSET ) + (buss / 2) ))
        error |= 1;
    if (!dcudata ( ppit, mux | (mux << 4) ))
        error |= 2;
        
    return ( error );
}


/* Routine to read the integration count for a Counter Card.
 */
getcounts (dcu, counter, tic, count )
int dcu;
int counter;
int tic;
int *count;
{

    int board= dcu/ NDCUPERBOARD;       /* compute board number */
    int dcunum= dcu % NDCUPERBOARD;    /* compute pit group for dcu */
    register struct pit *ppit;
    int error = 0;
            
    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[dcunum]);

    if (!dcuaddr ( ppit, CNT_CARD_BASE + (counter * CNT_CARD_OFFSET) + tic ))
        error |= 1;
    *count = dcureadw ( ppit, &error );
    
    return ( error );
}

/* Support routine to write an device address to the DCU - returns YES
 * if successful - NO if timeout on address write.
 */
dcuaddr ( ppit, address )
register struct pit *ppit;
int address;
{

    register struct pit *addrpit = (struct pit *) ((char *) ppit + 1);
    long timeout_count;
    unsigned short test;
    unsigned short test1;
    unsigned short test2;


    /* set output mode/ bus master */    
    ppit->pcdata = PIO_OUT_MODE & 0xff;
    nop(2);
    addrpit->pcdata = PIO_OUT_MODE & 0xff;
    nop(2);
    test1 = ppit->pcdata;
    nop(2);
    test2 = addrpit->pcdata;
    test = (test1 << 8) | test2; 

    if ((test & 0x0101) == 0) 
       return ( NO );
   
    addrpit->padata = address;
    nop(2);
    addrpit->pactl = OUTDATAR;   /* strobe input */
    nop(2);
    addrpit->pactl = OUTDATA;

    return ( YES );


}
    
    
/* Support routine to write a byte of data to the DCU.  returns YES if 
 * successful - NO if time out on data write.
 */
dcudata ( ppit, data )
register struct pit *ppit;
int data;
{

    long timeout_count;
    unsigned short test;
    unsigned short test1;
    unsigned short test2;
    register struct pit *wordpit = (struct pit *) ((char *) ppit + 1);

    /* set output mode/ bus master */    
    ppit->pcdata = PIO_OUT_MODE & 0xff;
    nop(2);
    wordpit->pcdata = PIO_OUT_MODE & 0xff;
    nop(2);
    test1 = ppit->pcdata;
    nop(2);
    test2 = wordpit->pcdata;
    test = (test1 << 8) | test2; 

    if ((test & 0x0101) == 0) 
	return ( NO );
   
    ppit->padata = data;
    nop(2);
    ppit->pactl = OUTDATAR;   /* strobe input */
    nop(2);
    ppit->pactl = OUTDATA;

    return ( YES );

}
    
    
/* Support Routine to read a byte of data from the DCU.
 */
dcuread ( ppit, error )
register struct pit *ppit;
int *error;
{
    long timeout_count;
    int val;
    unsigned short test;
    unsigned short test1;
    unsigned short test2;
    register struct pit *wordpit = (struct pit *) ((char *) ppit + 1);

    /* set output mode/ bus master */    
    ppit->pcdata = PIO_IN_MODE & 0xff;
    nop(2);
    wordpit->pcdata = PIO_IN_MODE & 0xff;
    nop(2);
    test1 = ppit->pcdata;
    nop(2);
    test2 = wordpit->pcdata;
    test = (test1 << 8) | test2; 

    if ((test & 0x0101) != 0) 
        *error |= 8;
   
    ppit->pbctl = INDATAR;   /* strobe input */
    nop(2);
    val=  ppit->pbdata;
    nop(2);
    ppit->pbctl = INDATA;
    return ( val );    
}
    
    
/* Support Routine to read a word of data from the DCU.
 */
dcureadw ( ppit, error )
register struct pit *ppit;
int *error;
{

    int val;
    unsigned short low;
    unsigned short high;
    unsigned short test;
    unsigned short test1;
    unsigned short test2;
    register struct pit *wordpit = (struct pit *) ((char *) ppit + 1);

    /* set P/Q and R/S port C to read mode */ 
    ppit->pcdata = PIO_IN_MODE & 0xff;
    wordpit->pcdata = PIO_IN_MODE & 0xff;
    test1 = ppit->pcdata;
    test2 = wordpit->pcdata;
    test = (test1 << 8) | test2; 
     
    if ((test & 0x0101) != 0) 
        *error |= 8;
       
    ppit->pbctl = INDATAR;
    wordpit->pbctl = INDATAR;    
    nop(2);
    low = (wordpit->pbdata) & 0xff;
    high = (ppit->pbdata) & 0xff;
    nop(2);
    ppit->pbctl = INDATA;
    wordpit->pbctl = INDATA;

#ifdef BIG_ENDIAN
    /* value read is little endian - must byte swap for big endian */    
    return ( (low << 8) | high );
#else
    return ( (high << 8) | low );
#endif
    
}

pitinit ( ppit )
register struct pit *ppit;
{
	char	probe;

	if (vxMemProbe (&ppit->pivr, READ, 1, &probe))
	    return (1);
    ppit->pgcr = 0;
    ppit->pcdata = 0x01;    /* PC0 = 1 (output) PC1 =0 (master) */
    ppit->pcddr = 0x03;     /* PC0 and PC1 are outputs */
    ppit->paddr = 0xff;     /* Port A is output */
    ppit->pbddr = 0x00;     /* Port B is input */
    ppit->pactl = OUTDATA;  /* port A is output */ 
    ppit->pbctl = INDATA;   /* port B is input */
    ppit->psrr = 0x0;       /* DMA not used, PC4,PC5,PC6 carry function */
    ppit->pgcr = 0x30;      /* Mode 00, all handshakes enabled, negative logic 
                             */
	return (0);
}
db_pitinit ( ppit )
register struct pit *ppit;
{
	char	probe;
	int	status;

	if (status = vxMemProbe (&ppit->pivr, READ, 1, &probe)) {
	    printf ("db_pitinit vxMemProbe = %d\n", status);
	    return (1);
	}
    ppit->pgcr = 0;
    ppit->pcdata = 0x01;    /* PC0 = 1 (output) PC1 =0 (master) */
    ppit->pcddr = 0x03;     /* PC0 and PC1 are outputs */
    ppit->paddr = 0xff;     /* Port A is output */
    ppit->pbddr = 0x00;     /* Port B is input */
    ppit->pactl = OUTDATA;  /* port A is output */ 
    ppit->pbctl = INDATA;   /* port B is input */
    ppit->psrr = 0x0;       /* DMA not used, PC4,PC5,PC6 carry function */
    ppit->pgcr = 0x30;      /* Mode 00, all handshakes enabled, negative logic 
                             */
	return (0);
}


nop(x)
int x;
{

    int i,j;
    
    for (i=0;i<x;i++)
        j++;
}


/****************************************************************************
 *                                                                          *
 *                      Low Level Hardware Debug Routines                   *
 *                                                                          *
 ****************************************************************************
 

/* opiowb - output a byte of data to a PIT channel, channel 0,1,2, or 3
 * are valid.
 */
opiowb ( board, chan, val)
int board;
int chan;
int val;
{
    register struct pit *ppit;
    long i;
    
    ppit = (struct pit *) (opiobase + boardbase[board] + pitbase[chan&0x03]);
    ppit->pcdata = 0x01; /* output mode, bus master */

    /* wait for data to clear out */
    for (i = 0; i < 2000000; i++) {
        if ((ppit->pstat & 0x01) == 0x01) { 
            ppit->padata = val;
            return ( 0 );
        }
    } 
    return ( -1 );
}

/* opiorb - input a byte from a PIT channel, channel 0,1,2, or 3
 * are valid.
 */
opiorb ( board, chan )
int board;
int chan;
{
    register struct pit *ppit;
    long i;
    int error= 0;
    int val;
    
    ppit = (struct pit *) (opiobase + boardbase[board] + pitbase[chan&0x03]);
    val= dcuread (ppit, &error);
    if (error != 0)
        printf("read error . . .\n");
    return (val);
}

/* 16 bit data read - works on only on Q/S input pit pair (channel 0).
 */
readdata ( board, addr)
int board;
int addr;
{

    register struct pit *ppit;
    unsigned int high,low;
    int error= 0;
    int val;
    
    ppit = (struct pit *) (opiobase + boardbase[board] + dcu_pitbase[0]);

    if (!dcuaddr ( ppit, addr))
        error |= 1;
    val = dcureadw ( ppit, &error );
    if (error != 0)
        printf("read error . . .\n");

    return ( val );
}            
   
    
/* write 8 bit data to dcu at address.  Valid only for channel 0.
 */
writdata ( board, addr, data)
int board;
int addr;
int data;
{

     /* write the address to R */
    if (opiowb ( board, 1, addr ) != 0) {
        printf ( "timeout on address write\n" );
        return ( -1 );
    }

    /* write data to P */
    if (opiowb ( board, 0, data ) != 0) {
        printf ( "timeout on data write\n" );
        return ( -1 );
    }
        
    return (0);
}            
    
