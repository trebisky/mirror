	if (mp->nitems >= MAX_ITEMS)
	    return ((MENUS *)2);
	if ((ip = (ITEM *)calloc (1, sizeof(ITEM))) == (ITEM *)0)
	    return ((MENUS *)0);
	mp->item[mp->nitems++] = ip;
