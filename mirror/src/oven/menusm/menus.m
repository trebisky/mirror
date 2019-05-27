	if (msp->nmenus >= MAX_MENUS)
	    return ((MENUS *)1);
	if ((mp = (MENU *)calloc (1, sizeof(MENU))) == (MENU *)0)
	    return ((MENUS *)0);
	msp->menu[msp->nmenus++] = mp;
