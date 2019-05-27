typedef	struct	s_context	{
	struct	s_context	*prev;
	struct	s_context	*next;
	int	nmenu;		/* current menu number */
	int	nline;		/* current line number of cursor */
	int	nitem;		/* current item number */
	int	ntime;		/* current time number */
	int	mline;		/* current line number of top of screen */
	DNTX	dntx;
	DSB	dsb;
	PFE	pfe;
	int	zone;
	int	tctype;
	int	node;
} CONTEXT;

#define	Gmenu	( globalp->contextp->nmenu )
#define	Gnline	( globalp->contextp->nline )
#define	Gitem	( globalp->contextp->nitem )
#define	Gtime	( globalp->contextp->ntime )
#define	Gmline	( globalp->contextp->mline )

#define	Gdntx	( globalp->contextp->dntx )
#define	Gdsb	( globalp->contextp->dsb )
#define	Gpfe	( globalp->contextp->pfe )
#define	Gzone	( globalp->contextp->zone )
#define	Gnode	( globalp->contextp->node )
#define	Gtctype	( globalp->contextp->tctype )

#define	Ptc	( ptc_dntx (Pdb, Gdntx) )
#define	Dtc	( dtc_dntx (Ddb, Gdntx) )
#define	Ptic	( ptic_dntx (Pdb, Gdntx) )
#define	Dtic	( dtic_dntx (Ddb, Gdntx) )
#define	Pcounter ( pcounter_dntx (Pdb, Gdntx) )
#define	Dcounter ( dcounter_dntx (Ddb, Gdntx) )
#define	Pbuss	( pbuss_dsb (Pdb, Gdsb) )
#define	Dbuss	( dbuss_dsb (Ddb, Gdsb) )
#define	Pbusst	( pbuss_dsb (Pdb, Ptic->select) )
#define	Dbusst	( dbuss_dsb (Ddb, Ptic->select) )
#define	Pselect	( pselect_dsb (Pdb, Gdsb) )
#define	Dselect	( dselect_dsb (Ddb, Gdsb) )
#define	Pdcu	( pdcu_dntx (Pdb, Gdntx) )
#define	Ddcu	( ddcu_dntx (Ddb, Gdntx) )
#define	Ptctype ( &Pdb->tctype[Gtctype] )
#define	Phe	( phe_pfe (Pdb, Gpfe) )
#define	Dhe	( dhe_pfe (Ddb, Gpfe) )
#define	Ptcn(x)	( ptc_dntx (Pdb, Phe->nneighbor[x]) )
#define	Dtcn(x)	( dtc_dntx (Ddb, Phe->nneighbor[x]) )
#define	Ptcf(x)	( ptc_dntx (Pdb, Phe->fneighbor[x]) )
#define	Dtcf(x)	( dtc_dntx (Ddb, Phe->fneighbor[x]) )
#define	Pfase	( pfase_pfe (Pdb, Gpfe) )
#define	Pfasex(x) ( pfase_pfe (Pdb, x) )
#define	Dfase	( dfase_pfe (Ddb, Gpfe) )
#define	Ppanel ( ppanel_pfe (Pdb, Gpfe) )
#define	Ppanelx(x) ( ppanel_pfe (Pdb, x) )
#define	Dpanel ( dpanel_pfe (Ddb, Gpfe) )
#define	Bzone	( &Bdb->zone[Gzone] )
#define	Bzonex(x) ( &Bdb->zone[x] )
#define	Pzone	( &Pdb->zone[Gzone] )
#define	Pzonex(x) ( &Pdb->zone[x] )
#define	Dzone	( &Ddb->zone[Gzone] )
#define	Pzoneh	( &Pdb->zone[Phe->zone] )
#define	Dzoneh	( &Ddb->zone[Phe->zone] )
#define	Pnodex(x) ( &Pzone->node[MAX(0,x)] )
#define	Pnoden	( Pnodex(Gnode) )
#define	Pnodem	( Pnodex(Gnode-1) )
#define	Pnode	( Pnodex(Dzone->c_node) )
#define	Pnodeh	( &Pzoneh->node[Dzoneh->c_node] )
