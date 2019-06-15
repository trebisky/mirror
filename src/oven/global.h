typedef	struct	{
	database	*db;
	b_database	*bdb;
	b_database	*obdb;		/* original */
	p_database	*pdb;
	p_database	*opdb;		/* original */
	i_database	*idb;
	d_database	*ddb;
	MENUS		*msp;
	CONTEXT		*contextp;
	int		noven;
	int		ncomp;
	int		readonly;
	BOOL		editp;		/* enable edit parameters */
	BOOL		editb;		/* enable edit biparameters */
	database	*gdbn[N_OVEN];
	char		*pdbn[N_OVEN];
	char		*bdbn[N_OVEN];
} GLOBAL;

#define	Gdb		( globalp->db )
#define	Edb		( &globalp->db->error )
#define	Bdb		( globalp->bdb )
#define	Pdb		( globalp->pdb )
#define	OBdb		( globalp->obdb )
#define	OPdb		( globalp->opdb )
#define	Idb		( globalp->idb )
#define	Ddb		( globalp->ddb )
#define	Geditp		( globalp->editp )
#define	Geditb		( globalp->editb )

#define	Menus		( globalp->msp->menu )

#ifdef	GLOBALS
GLOBAL	*globalp;
#else
extern	GLOBAL	*globalp;
#endif
