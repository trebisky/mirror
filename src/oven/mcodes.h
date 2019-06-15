#define	UNKNOWN	0	/* unrecognized key	*/
#define	HELP	1	/* cursor mode help	*/
#define	ENTER	2	/* enter new data	*/
#define	REPAINT	3	/* repaint screen	*/
#define	GOTO	4	/* go to related menu	*/
#define	RETURN	5	/* return to previous	*/
#define	CUR_D	6	/* cursor down one line	*/
#define	CUR_U	7	/* cursor up   one line	*/
#define	SCR_D2	8	/* scroll down half page*/
#define	SCR_U2	9	/* scroll up   half page*/
#define	SCR_D1	10	/* scroll down full page*/
#define	SCR_U1	11	/* scroll up   full page*/
#define	SCR_D0	12	/* scroll to end of page*/
#define	SCR_U0	13	/* scroll to top of page*/
#define	REFRESH	14	/* refresh screen	*/
#define	AUTO	15	/* auto refresh screen	*/
#define	CACHEP	16	/* cache parameters	*/
#define	CACHEB	17	/* cache biparameters	*/
#define	WRITEP	18	/* write parameters	*/
#define	IMCUR	19	/* enter new data =imcur*/
#define	GOERR	20	/* go to error   menu	*/
#define	ENTERG	21	/* enter new data /goto	*/
#define	IMCURG	22	/* enter imcur /goto   r*/

#define	M_REPAINT	1
#define	M_REFRESH	2
#define	M_AUTO		4
