#include "oven.h"

/* pwork - power work for each tick
 */
pwork (db)
database	*db;
{
	int	panel;

	for (panel = 0; panel < 18; panel++)	/* N_PANEL-1 ? */
	    phw_init (panel);

	pwait ();
	wpanel (db);

	forever {
	    pwait ();
	    psync (db);		/* send sync characters to power panels	*/
	    rpanel (db);	/* read in sensor data from panels	*/
	    ecalc (db);		/* check for errors			*/
	    wpanel (db);	/* send next tick's data to panels	*/
	    honk (db->intermediate.misc.horn); /* honk the stuckon horn	*/
	    ereport (db);	/* report errors			*/
	}
}

/* pcalc - pre-calculate ssr maps for the next minute for each panel
 */
pcalc (db, panel)
database	*db;
int	panel;
{
	int	clock;

	if (CONFIG)
	    for (clock = MAX_POWER; clock; clock--)
		pclockf (db, clock, panel);
	else
	    for (clock = MAX_POWER; clock; clock--)
		pclock (db, clock, panel);
}

/* pclockf - pre-calculate ssr maps for the next p clock for each panel
 * in config mode
 */
pclockf (db, clock, panel)
database	*db;
int		clock;
int	panel;
{
	p_panel	*ppanel = db->parameter.panel + panel;
	i_panel	*ipanel = db->intermediate.panel + panel;
	d_panel	*dpanel = db->data.panel + panel;
	DCL_FASE
	int	n = MAPCLOCK(db, panel) + (MAX_POWER-clock);
	i_map *issr = ipanel->ssr + n;

	for (fase = 0; fase < N_FASE; FASE__) {

	    DCL_ELEMENT

	    for (element = 0; element < N_ELEMENT; ELEMENT__) {
		if (configt[fase][element] == n) {
		    issr->mapbyte[mapbyt[fase][element]]
			|=  mapbit[fase][element];
		} else {
		    issr->mapbyte[mapbyt[fase][element]]
			&= ~mapbit[fase][element];
		}
	    }
	}
}

/* pclock - pre-calculate ssr maps for the next p clock for each panel
 */
pclock (db, clock, panel)
database	*db;
int		clock;
int	panel;
{
	p_panel	*ppanel = db->parameter.panel + panel;
	i_panel	*ipanel = db->intermediate.panel + panel;
	d_panel	*dpanel = db->data.panel + panel;

	DCL_FASE
	i_map *issr = ipanel->ssr + MAPCLOCK(db, panel) + (MAX_POWER-clock);

	for (fase = 0; fase < N_FASE; FASE__) {

	    DCL_ELEMENT
	    int		max_load = ifase->low_load;
	    int		order[N_ELEMENT];
	    int		prior[N_ELEMENT];
	    register	int		f;

	    if (clock <= ifase->nhigh)
		max_load++;
	    max_load = MIN (max_load, pfase->max_load);

	    for (element = 0; element < N_ELEMENT; ELEMENT__) {

		/* set the priority for this heater equal to the
		 * difference of the remaining power request to be
		 * satisfied and the remaining time.  This is usually
		 * a negative number.  If there is just enough time
		 * left (or we don't have enough time left), or if the
		 * heater is already on, boost its priority, unless there
		 * is no more remaining power request to be satisfied.
		 */
		PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		register	int	priority = ielement->left - clock;

		if (ielement->left && (priority >= 0 || ielement->state))
		    priority += pfase->epsilon;
		ielement->state = NO;

		for (f = element; f != 0; f--) {
		    if (prior[order[f-1]] < priority) {
			order[f] = order[f-1];
		    } else {
			break;
		    }
		}
		order[f] = element;
		prior[element] = priority;
	    }

	    /* now turn of the highest priority heaters we can
	     */
	    for (f = 0; f < max_load; f++) {

		int		element = order[f];
		p_element	*pelement = pfase->element + element;
		i_element	*ielement = ifase->element + element;
		d_element	*delement = dfase->element + element;
		PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		ielement->state = YES;
		ielement->left--;
	    }
	}

	pfase = ppanel->fase;
	ifase = ipanel->fase;
	dfase = dpanel->fase;

	for (fase = 0; fase < N_FASE; FASE__) {

	    DCL_ELEMENT

	    for (element = 0; element < N_ELEMENT; ELEMENT__) {
		if (ielement->state) {
		    issr->mapbyte[mapbyt[fase][element]]
			|=  mapbit[fase][element];
		} else {
		    issr->mapbyte[mapbyt[fase][element]]
			&= ~mapbit[fase][element];
		}
	    }
	}
}

/* ecalc - calculate error conditions for elements and panels
 */
ecalc (db)
database	*db;
{
	DCL_PANEL

	db->intermediate.misc.horn = NO;

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE
	    register	i_map	*issr = ipanel->omap + M_SAV;
	    register	i_map	*ierr = ipanel->omap + M_ERR;
	    register	i_map	*ivol = ipanel->imap + M_VOL;
	    register	i_map	*ilcs = ipanel->imap + M_LCS;
	    register	i_map	*ircs = ipanel->imap + M_RCS;

	    if (!(CONNECT (ppanel->connect)))
		continue;
	    if (ppanel->ss_down)
		continue;
	    if (ipanel->pp_error)
		continue;
	    if (ipanel->ss_error = PANEL_SENSOR_STATUS_ERROR)
		continue;

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE	 pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    int he = ((issr->mapbyte[mapbyt[fase][element]]
				& mapbit[fase][element]) != 0) ? 1 : 0;
		    int vs = ((ivol->mapbyte[mapbyt[fase][element]]
				& mapbit[fase][element]) != 0) ? 1 : 0;
		    int cs = ((ilcs->mapbyte[mapbyt[fase][element]]
				& mapbit[fase][element]) != 0) ? 1 : 0;
		    int rs = ((ircs->mapbyte[mapbyt[fase][element]]
				& mapbit[fase][element]) != 0) ? 1 : 0;

		    if (pelement->vs_down)
			vs = HE;
		    if (pelement->cs_down)
			cs = HE;
		    if (pelement->rs_down)
			rs = HE;

		    if ((ielement->error = perr[he][vs][cs][rs]) == HON) {
			db->intermediate.misc.horn = YES;
			ierr->mapbyte[mapbyt[fase][element]]
			    |=  mapbit[fase][element];
		    } else {
			ierr->mapbyte[mapbyt[fase][element]]
			    &= ~mapbit[fase][element];
		    }
		}
	    }
	}
	if (db->intermediate.misc.horn)
	    db->intermediate.misc.gong++;
}

/* ereport - report error conditions for elements and panels
 */
ereport (db)
database	*db;
{
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    if (!(CONNECT (ppanel->connect)))
		continue;
	    if (ppanel->ss_down)
		continue;
	    if (ipanel->pp_error) {
		PERRPPE;
		continue;
	    }
	    if (ipanel->ss_error) {
		PERRPKP;
		continue;
	    }

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE	 pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    switch (ielement->error) {
		    case CON:
			PERRCON;
			break;
		    case VON:
			PERRVON;
			break;
		    case HON:
			PERRHON;
			break;
		    case HOF:
			PERRHOF;
			break;
		    case VOF:
			PERRVOF;
			break;
		    case COF:
			PERRCOF;
			break;
		    case ROF:
			PERRROF;
			break;
		    case RON:
			PERRRON;
			break;
		    case OOII:
			PERROOII;
			break;
		    case OIOI:
			PERROIOI;
			break;
		    case OIIO:
			PERROIIO;
			break;
		    case IOOI:
			PERRIOOI;
			break;
		    case IOIO:
			PERRIOIO;
			break;
		    case IIOO:
			PERRIIOO;
			break;
		    default:
			break;
		    }
		}
	    }
	}
}

/* psync - write output sync to power panels
 */
psync (db)
database	*db;
{
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

#ifndef	ALWAYS_OUTPUT
	    if (!(CONNECT (ppanel->connect)))
		continue;
#endif

	    writepanel (ppanel->port, "\n");
	}
}

/* wpanel - write output data to power panels
 */
wpanel (db)
database	*db;
{
	char	string[LEN_PMAP+1];

	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    i_map	*issr = ipanel->ssr + PCLOCK (db, panel);
	    i_map	*ihel = ipanel->omap + M_HEL;
	    i_map	*isav = ipanel->omap + M_SAV;
	    register	i_map	*omap = ipanel->omap;
	    register	int	pmap;
	    register	int	l;
	    register	char	*cp = string;
	    register	unsigned long sum = 0;

#ifndef	ALWAYS_OUTPUT
	    if (!(CONNECT (ppanel->connect)))
		continue;
#endif

	    for (l = 0; l < LEN_MAP; l++) {
		isav->mapbyte[l] = ihel->mapbyte[l];
		ihel->mapbyte[l] = issr->mapbyte[l];
		issr->mapbyte[l] = 0;
	    }

	    for (pmap = 0; pmap < N_PMAP; omap++, pmap++) {
		for (l = 0; l < LEN_MAP; l++) {
		    *cp++ = tohex[MSN (omap->mapbyte[l])];
		    *cp++ = tohex[LSN (omap->mapbyte[l])];
		    sum += omap->mapbyte[l];
		}
	    }
	    *cp++ = tohex[MSN (sum)];
	    *cp++ = tohex[LSN (sum)];
	    *cp++ = 0;

	    writepanel (ppanel->port, string);
	}
}

/* rpanel - read input data from power panels
 */
rpanel (db)
database	*db;
{
	char	string[2048];

	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    register	i_map	*imap = ipanel->imap;
	    register	int	pmap;
	    register	int	l;
	    register	char	*cp = string;
	    register	unsigned long sum = 0;
	    unsigned char mapbyte = 0;
	    BOOL	error = NO;

	    if (!(CONNECT (ppanel->connect)))
		continue;

	    string[0] = 0;
	    readpanel (ppanel->port, string);

	    if (strlen (string) != LEN_PMAP) {
		ipanel->pp_error = YES;
		continue;
	    }

	    for (pmap = 0; pmap < N_PMAP; imap++, pmap++) {
		for (l = 0; l < LEN_MAP; cp++, cp++, l++) {

		    unsigned char mapbyte = 0;

		    if (byte_hex (cp, &mapbyte))
			error = YES;
		    imap->mapbyte[l] = mapbyte;
		    sum += mapbyte;
		}
	    }
	    if (error) {
		ipanel->pp_error = YES;
		continue;
	    }
	    if (byte_hex (cp, &mapbyte)) {
		ipanel->pp_error = YES;
		continue;
	    }
	    cp++; cp++;
	    if (mapbyte != (sum & 0xff)) {
		ipanel->pp_error = YES;
		continue;
	    }
	    if (*cp != '\n') {
		ipanel->pp_error = YES;
		continue;
	    }
	    ipanel->pp_error = NO;
	}
}

/* byte_hex - make a byte from two ascii hex digits
 */
byte_hex (string, b)
char	*string;
unsigned char *b;
{
	register char	*cp = string;
	register unsigned char mapbyte = 0;
	register unsigned char v;
	register int	iv;
	register int	n;

	for (n = 0; n < SZ_NIB; n++) {
	    mapbyte <<= 4;
	    if ((iv = fromhex (*cp++)) < 0) {
		return (YES);
	    } else {
		v = iv;
		mapbyte |= v;
	    }
	}
	*b = mapbyte;
	return (NO);
}

/* fromhex - convert a single ascii hex character to its binary value
 */
fromhex (c)
char	c;
{
	int	n;

	for (n = 0; n < strlen (tohex); n++) {
	    if (c == tohex[n])
		if (n < 16)
		    return (n);
		else
		    return (n-16+10);
	}
	return (-1);
}

radcv (channel, value)
int	channel;		/* 0 - 63 */
float	*value;
{
	int	v;

	rdadc (channel, &v);
	if (v == -1)
	    *value = INDEFA;
	else
	    *value = ((float)v/ADCFSV)*adcfsv[channel];
}

/* wrrdata - write rotation data
 */
wrrdata (db)
database	*db;
{
	int	speed = db->data.zone[ROT_ZONE].etmp * 1000.0 + 0.5;
	int	c = getmycompnumber();

	c = c < 0 ? 0 : c;
	wrrotation (db->parameter.misc.rport[c], speed);
}

wrrotation (p, speed)
int	p;
int	speed;
{
	char	string[13];
	char	*cp = string;
	unsigned char	sb1 = speed/256;
	unsigned char	sb2 = speed%256;

	*cp++ = 'R';
	*cp++ = tohex[MSN (sb1)];
	*cp++ = tohex[LSN (sb1)];
	*cp++ = tohex[MSN (sb2)];
	*cp++ = tohex[LSN (sb2)];
	*cp++ = tohex[MSN (sb1)];
	*cp++ = tohex[LSN (sb1)];
	*cp++ = tohex[MSN (sb2)];
	*cp++ = tohex[LSN (sb2)];
	*cp++ = '0';
	*cp++ = '\r';
	*cp++ = '\n';
	*cp++ = 0;

	writepanel (p, string);	/* generates even parity ? */
}

/* rdrdata - read rotation data
 */
rdrdata (db)
database	*db;
{
	int	speed;

	rdrotation (db, &speed);
	if (speed < 0)
	    db->data.zone[ROT_ZONE].ztmp = INDEFT;
	else
	    db->data.zone[ROT_ZONE].ztmp = (float)speed / 1000.0;
}

static	int	rotation_speed;
static	long	rotation_timestamp;

rdrotation (db, speed)
database	*db;
int	*speed;
{
	if (IS_STALEA (rotation_timestamp))
	    *speed = -1;
	else
	    *speed = rotation_speed;
}

int	logrspeed;
logrspeedon()	{ logrspeed = 1; }
logrspeedoff()	{ logrspeed = 0; }

/* rwork - rotation listener
 */
rwork (db)
database	*db;
{
	char	buffer[512];
	char	string[512];
	int	nsleep = 0;
	int	c = getmycompnumber();
	int	o = getmyovennumber();
	char	rspeed[32];
	int	n = 0;
	int	*fopen(), *fp = 0;	/* FILE */

	c = c < 0 ? 0 : c;
	o = o < 0 ? 0 : o;
	buffer[0] = 0;
	sprintf (rspeed, "rspeed.oven%dv%d", o, c);

	forever {
	    string[0] = 0;
#ifdef	TEST
	    rdpanelr (db->parameter.misc.rport[c], string);
#else
	    readpanel (db->parameter.misc.rport[c], string);
#endif
	    if (strlen (string) < 1) {
		if (nsleep++)
		    buffer[0] = 0;
		sleep (2);
		continue;
	    }
	    if (strlen (string) > 12) {
		buffer[0] = 0;
		continue;
	    }
	    strcat (buffer, string);
	    if (strlen (buffer) > 12) {
		buffer[0] = 0;
		continue;
	    }
	    if (strlen (buffer) == 12) {
		rotation_speed = rot_interpret (buffer);
		rotation_timestamp = db->data.misc.uclock;
		rfire ();
		if (!n)
		    if (db->parameter.misc.logrotv && logrspeed)
			fp = fopen (rspeed, "a");
		    else
			fp = 0;
		if (fp)
		    fprintf (fp, "%d %d\n", rotation_timestamp, rotation_speed);
		if (++n >= 12) {
		    n = 0;
		    if (fp) {
			fclose (fp);
			fp = 0;
		    }
		}
	    }
	}
}

/* getrspeed - Get the next rspeed reading
 */
getrspeed (t, s)
long	*t;		/* time stamp	*/
int	*s;		/* rspeed	*/
{
	rwait ();
	*t = rotation_timestamp;
	*s = rotation_speed;
}

/* rot_interpret - interpret the string from the rotation computer
 */
rot_interpret (b)
char	*b;
{
	unsigned char	hi, lo;
	int	speed;

	if (b[0] != 'R')
	    return (-1);
	if (b[9] != '0')
	    return (-1);
	if (b[10] != '\r')
	    return (-1);
	if (b[11] != '\n')
	    return (-1);
	if (b[1] != b[5])
	    return (-1);
	if (b[2] != b[6])
	    return (-1);
	if (b[3] != b[7])
	    return (-1);
	if (b[4] != b[8])
	    return (-1);
	if (byte_hex (&b[1], &hi))
	    return (-1);
	if (byte_hex (&b[3], &lo))
	    return (-1);
	speed  = hi;
	speed *= 256;
	speed += lo;
	return (speed);
}

/* honk - honk the horn
 */
honk (horn)
BOOL	horn;
{
}

printa (channel)
int	channel;
{
	int	value;

	rdadc (channel, &value);
	return (value);
}

int	echowrpanel;
int	echordpanel;
wrpanelecho (x) int x; { echowrpanel = x; }
rdpanelecho (x) int x; { echordpanel = x; }

writepanel (panel, string)
int	panel;
char	*string;
{
	int	status;

	status = wrpanel (panel, string);
	if (echowrpanel)
	    printf ("wrpanel=%d %d `%s'\n", status, panel, string);
	return (status);
}

readpanel (panel, string)
int	panel;
char	*string;
{
	int	status;

	status = rdpanel (panel, string);
	if (echordpanel)
	    printf ("rdpanel=%d %d `%s'\n", status, panel, string);
	return (status);
}
#ifdef	TEST

rdpanelr (panel, string)
int	panel;
char	*string;
{
	strcpy (string, "");
}

rdadc (channel, value)
int	channel;		/* 0 - 63 */
int	*value;			/* 0 - 4095, or -1 for indef */
{
	*value = -1;
}

phw_init (panel)
int	panel;
{
}

wrpanel (panel, string)
int	panel;
char	*string;
{
}

rdpanel (panel, string)
int	panel;
char	*string;
{
	strcpy (string, "0000000000000000000000000000000000\n");
}
#endif
#ifdef	HELP_DOC



                     Steward Observatory Mirror Lab
                      Power Panel Serial Protocol
                             Skip Schaller
                             March 20, 1989

        The host will output a strobe character to the  power  panel  to
start  a  sequence  of  events.   This  strobe character will be a '\n',
(NEWLINE character = 0xA).  This  marks  the  beginning  of  a  250  ms.
timing  period.  Upon receipt of this strobe, the previously stored data
values of the 16 output bytes are output to  the  power  panel  latches.
If  no  valid  data  values (invalid because of a checksum error, parity
error, etc.)  have  been  received  since  the  last  strobe  character,
(within  the  last  250 ms. period), all zero values should be output to
the latches.  Likewise, if no strobe  character  is  received  for  1000
ms.,  the  latches should be zeroed.  Some time after the receipt of the
strobe character, but within the 250 ms. period, the host will  transmit
the  16 data values, encoded as 32 ascii hex digits and a checksum byte,
encoded as 2 ascii hex digits.  The most significant hex digit  (nibble)
is  transmitted  first.  The checksum byte is the least significant byte
of a simple unsigned sum of the 16 data bytes.  If the checksum  is  not
correct,  the data are invalid.  Data may also be invalidated because of
an invalid ascii hex character, even parity failure, or  serial  errors,
such  as  framing or data overrun.  The output data values are output as
sequential bytes of information.   The  first  four  data  bytes  (eight
characters)  are the heater words 0-3 (heaters 7-0, 14-8, 22-15, 29-23),
and the next four are the error lamp words 0-3.  (The next  eight  bytes
are  currently  unused.)  These  data  values  are stored until the next
strobe is received, at which time they  are  immediately  transfered  to
the power panel latches.

        The  strobe  character also starts an input sequence to transmit
information from the power panels to the host.  After a delay of 160 ms.
from  the receipt of the strobe character, the power panel input buss is
read, and the 16 data bytes are encoded as characters in  the  same  way
as  the  output  sequence.  These characters are transmitted immediately
to the host.  The first four data bytes are  the  voltage  sensor  words
0-3,  the  second  four are the local current sensor words, and the next
four are the remote current sensor words.  (The last four are  currently
unused.)  A  checksum  byte  is  generated  as above, and encoded as two
ascii hex digits.  This is followed by a trailer character (NEWLINE).

        Thus, both the output and input data packets  are  of  the  same
format  and length (35 characters).  At 9600 baud, this information will
be transmitted in 37 ms.


                ROTATION COMPUTER COMMUNICATION PROTOCOL


     Serial  communication between the Power and rotation  computer  has 
been beefed up to provide for a more robust interface.  The protocol now 
consists of two tiers - one hardware and one software.  

     The hardware protocol expects even parity to be present in the  MSb 
of the received 8 bit data word.  If hardware detects a parity error the 
packet  being received is marked invalid and receive  software  protocol 
processing  is  reset to state 0 - which begins  searching  for  another 
header character.

     The software protocol utilizes a data - redundant packet  structure 
with  a  single byte header (sync) character and a three  byte  trailer.  
(See  figure 1.)  In all the packet length is 12 bytes long and is  made 
up of only valid ASCII characters.

     The header or sync character provides for a bit pattern that can be 
recognized  by the receiver as the start of data.  Only a  single  ASCII 
character  is considered valid in this field and must match one  of  the 
predetermined  sync  characters  between R and Z  which  determines  the 
ultimate  destination  of  the  data once  a  valid  message  packet  is 
completely received.

     The trailer consists of three bytes and must match the sequence 
0<CR><LF>  where  '0'  is an ASCII zero character, <CR>  is  a  carriage 
return  (0Dh)  and  <LF>  is a line feed (0Ah).   The  purpose  of  this 
header/trailer   combination  is  to  test  all  of  the  possible   bit 
combinations  in the ASCII serial stream.  If any of the 7 LS UART  data 
bits  are stuck low or high on either the receive or transmit end,  this 
header/trailer  combination  of the protocol will be corrupted  and  the 
message should be subsequently flaged as invalid.

     The  data  portion  of the packet is a  4  byte  ASCII  Hexidecimal 
representation  of  the  two byte binary  number  being  transmitted  or 
received.  The number is repeated to make an eight byte data field  that 
can   be  split  and  compared  against  itself  as  further   test   of 
communication integrity.  

     If  at  any  point  in the reception of the  message  the  datum  is 
found  to  be a nonvalid ASCII character the receive  buffer  should  be 
flushed  and  the  protocol reset to state 0 -  awaiting  a  valid  sync 
character.    In  addition  if  any  one  of  the   Hexidecimal   digits 
representing  the  number being transmitted is found to be  out  of  the 
range  between  '0' through '9' or 'A' through 'F' or the copy  of  this 
number does not match its partner - a similar reset to state 0 should be 
performed.




     FIGURE 1 - The P to R computer communication packet structure



      +---+----+----+----+----+----+----+----+----+---+------+------+
      | s | h1 | h2 | h3 | h4 | h5 | h6 | h7 | h8 | 0 | <CR> | <LF> |
      +---+----+----+----+----+----+----+----+----+---+------+------+
byte    1   2    2    4    5    6    7    8    9   10    11     12

     where:

               s ==>  ASCII sync character  -  'R' >= s <= 'Z' 

               h1 h2 h3 h4 ==> ASCII Hexidecimal representation of
                               the two byte binary number being sent
                               or received - h1 = MSB and h4 = LSB
                               '0' >=  hn <= '9' || 'A' >= hn <= 'F'

               h5 h6 h7 h8 ==> Copy of ASCII Hexidecimal  representation of
                               the two byte binary number being sent
                               or received - h5 = MSB and h8 = LSB.
                               '0' >=  hn <= '9' || 'A' >= hn <= 'F'

               0 ==>  ASCII zero (30h)

               <CR> ==> Carriage return (0Dh)

               <LF> ==> Line Feed (0Ah)


#endif
