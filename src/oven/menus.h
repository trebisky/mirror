#define	MAX_ITEMS	1000
#define	MAX_MENUS	100

typedef	int	(*PFI)();	/* pointer a function returning an integer */

typedef	struct	{
	PFI	ntimes;		/* number of iterations (NULL => 1)	*/
	int	mtimes;		/* cached result of above		*/
	char	*text;		/* the text string	*/
	PFI	gfunc;		/* go     function	*/
	PFI	ofunc;		/* output function	*/
	PFI	ifunc;		/* input  function	*/
	PFI	tfunc;		/* toggle function	*/
	int	text_start;	/* text start column	*/
	int	text_end;	/* text end   column	*/
	int	func_start;	/* func start column	*/
	int	func_end;	/* func end   column	*/
} ITEM;

typedef	struct	{
	char	*id;		/* menu id string	*/
	int	mlines;		/* sum of above mtimes	*/
	int	nitems;
	ITEM	*item[MAX_ITEMS];
} MENU;

typedef	struct	{
	int	nmenus;
	MENU	*menu[MAX_MENUS];
} MENUS;
