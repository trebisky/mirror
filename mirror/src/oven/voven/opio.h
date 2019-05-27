/* This C structure gives the register structure for a MC68230 PIT chip
 * accessed from a 16 bit bus (on every other byte).  The structure can
 * also accomodate a word oriented access to the Q-S and U-W ports
 * for read and the P-R and T-V ports for write. 
 */

#define NUMPORTS  54

struct pit {
    union {
        unsigned char pitc[NUMPORTS];
        unsigned short pitw[NUMPORTS / sizeof ( unsigned short)];
    } piou;
};

#define pgcr    piou.pitc[1]          /* General control register */
#define psrr    piou.pitc[3]          /* Service request register */
#define paddr   piou.pitc[5]          /* Port A data direction register */
#define pbddr   piou.pitc[7]          /* Port B data direction register */
#define pcddr   piou.pitc[9]          /* Port C data direction register */
#define pivr    piou.pitc[11]         /* Port interrupt vector */
#define pactl   piou.pitc[13]         /* Port A control */
#define pbctl   piou.pitc[15]         /* Port B control */
#define padata  piou.pitc[17]         /* Port A data */
#define pbdata  piou.pitc[19]         /* Port B data */
#define paalt   piou.pitc[21]         /* Port A alternate data */
#define pbalt   piou.pitc[23]         /* Port B alternate data */
#define pcdata  piou.pitc[25]         /* Port C data */
#define pstat   piou.pitc[27]         /* Port status */
#define tcr     piou.pitc[33]         /* Timer control register */
#define tivr    piou.pitc[35]         /* Timer interrupt vector */
#define cprh    piou.pitc[39]         /* Timer: counter preload high */
#define cprm    piou.pitc[41]         /* Timer: counter preload middle */
#define cprl    piou.pitc[43]         /* Timer: counter preload low */
#define cnth    piou.pitc[47]         /* Timer: counter high */
#define cntm    piou.pitc[49]         /* Timer: counter middle */
#define cntl    piou.pitc[51]         /* Timer: counter low */
#define tsr     piou.pitc[53]         /* Timer: status */



#define pgcrw   piou.pitw[0]          /* General control registers */
#define psrrw   piou.pitw[1]          /* Service request registers */
#define paddrw  piou.pitw[2]          /* Port A data direction registers */
#define pbddrw  piou.pitw[3]          /* Port B data direction registers */
#define pcddrw  piou.pitw[4]          /* Port C data direction registers */
#define pivrw   piou.pitw[5]          /* Port interrupt vectors */
#define pactlw  piou.pitw[6]          /* Port A controls */
#define pbctlw  piou.pitw[7]          /* Port B controls */
#define padataw piou.pitw[8]          /* Port A data */
#define pbdataw piou.pitw[9]          /* Port B data */
#define paaltw  piou.pitw[10]         /* Port A alternate data */
#define pbaltw  piou.pitw[11]         /* Port B alternate data */
#define pcdataw piou.pitw[12]         /* Port C data */
#define pstatw  piou.pitw[13]         /* Port status */
#define tcrw    piou.pitw[16]         /* Timer control registers */
#define tivrw   piou.pitw[17]         /* Timer interrupt vectors */
#define cprhw   piou.pitw[19]         /* Timer: counter preload high */
#define cprmw   piou.pitw[20]         /* Timer: counter preload middle */
#define cprlw   piou.pitw[21]         /* Timer: counter preload low */
#define cnthw   piou.pitw[23]         /* Timer: counter high */
#define cntmw   piou.pitw[24]         /* Timer: counter middle */
#define cntlw   piou.pitw[25]         /* Timer: counter low */
#define tsrw    piou.pitw[26]         /* Timer: status */



struct bim {
/*00*/     unsigned char bicr;             /* BIM interrupt control register */
/*--*/     unsigned int  dum1, dum2, dum3; /* dummy control registers */
/*a4*/     unsigned char dum4, bivr;       /* BIM interrupt vector register */
};



#define    YES                      1
#define    NO                       0

#define    CS_BASE                  0x08            /* DCU-relative address base for Channel Select Card 0 */
#define    CS_OFFSET                0x08

#define    CNT_CARD_BASE            0x20            /* DCU-relative address base for Counter Card 0 */
#define    CNT_CARD_OFFSET          0x08

#define    DCUBASE                  0               /* DCU-relative address base for counters */
#define    DCUTIMER0_LOAD_REG       DCUBASE+0       /* DCU-relative address for counter 0 load/read register */
#define    DCUTIMER1_LOAD_REG       DCUBASE+1       /* DCU-relative address for counter 1 load/read register */
#define    DCUTIMER2_LOAD_REG       DCUBASE+2       /* DCU-relative address for counter 2 load/read register */
#define    DCUTIMER_MODE_REG        DCUBASE+3       /* DCU-relative address for counter mode register */
#define    DCUTIMER_GATE_REG        DCUBASE+4       /* DCU-relative address for counter 1 enable */
#define    DCUTIMER_STAT_REG        DCUBASE+5

#define    DCUTIMER0                0
#define    DCUTIMER1                0x40
#define    DCUTIMER2                0x80
#define    TIMER1_DISABLE           0x01
#define    TIMER1_ENABLE            0x03
#define    TIMER1_RUNNING_MASK      0x01
#define    TIMER2_RUNNING_MASK      0x02


#define    MODE0                    0               /* Bit pattern for counter mode 0 */
#define    MODE1                    0x02            /* Bit pattern for counter mode 1 */
#define    MODE2                    0x04            /* Bit pattern for counter mode 2 */
#define    MODE3                    0x06            /* Bit pattern for counter mode 3 */
#define    MODE4                    0x08            /* Bit pattern for counter mode 4 */
#define    MODE5                    0x0a            /* Bit pattern for counter mode 5 */

#define    LCOUNT0                  17              /* LSB of count for counter 0 count = 17 micro seconds */
#define    HCOUNT0                  0               /* MSB of count for counter 0 */
#define    LCOUNT1                  0x3B            /* LSB of count for counter 1 count= 466.667 milliseconds */
#define    HCOUNT1                  0x6B            /* MSB of count for counter 1 */
#define    LCOUNT2                  17              /* LSB of count for counter 2 count = 17 micro seconds */
#define    HCOUNT2                  0               /* MSB of count for counter 2 */

#define    RLM                      0x30            /* Bit pattern for R0 & R1 load Least then Most */

#define    GLOBAL_COUNTER_CLR_REG   0x80            /* Counter clear address */
#define    RDYMASK                  0x84            /* Control word for integration test */
#define    TIMEOUT                  5
#define    STOPTIMEOUT              3               /* timeout loop count */
#define    STARTTIMEOUT             3
#define    MUXDELAY                 30


/*************************************************************************
 * OPIO board specific defines 
 */

#define    NDCUPERBOARD             2
#define    NCOUNTERCARDS            6
#define    PIO_OUT_MODE             0x01
#define    PIO_IN_MODE              0x00
#define    OBFMASK                  0x01
#define    IBFMASK                  0x04
#define    BASE                     0x008001FF      /* jumpered board base address -1 */
#define    NPITCHAN                 4
#define    BIG_ENDIAN                               /* Non intel machine */

#define    INDATA                   0xF0            /* submode 1X, no interlocked input handshake, 
                                                     * H4 always negated, H3 interrupt disabled
                                                     * H3S status not used 
                                                     */
#define    INDATAR                  0xF8            /* submode 1X, no interlocked input handshake, 
                                                     * H4 always asserted, H3 interrupt disabled
                                                     * H3S status not used 
                                                     */

#define    OUTDATA                   0xF0            /* submode 1X, no interlocked input handshake, 
                                                     * H2 always negated, H1 interrupt disabled
                                                     * H1S status not used 
                                                     */
#define    OUTDATAR                  0xF8            /* submode 1X, no interlocked input handshake, 
                                                     * H2 always asserted, H1 interrupt disabled
                                                     * H1S status not used 
                                                     */

#define    INDATAW                  0xF0F0          /* submode 1X, no interlocked input handshake, 
                                                     * H4 always negated, H3 interrupt disabled
                                                     * H3S status not used 
                                                     */
#define    INDATARW                 0xF8F8          /* submode 1X, no interlocked input handshake, 
                                                     * H4 always asserted, H3 interrupt disabled
                                                     * H3S status not used 
                                                     */

static long pitbase[] = { 0x00, 0x01, 0x40, 0x41 };
static long dcu_pitbase[] = { 0x00, 0x40 };
static long pbimbase[] = { 0x82, 0x84, 0x86, 0x88 };
static long tbimbase[] = {0xa2, 0xa4, 0xa6, 0xa8 };
static long boardbase[] = { 0x00, 0x200, 0x400, 0x600 };

char *opiobase;                                     /* base address of opio board */
