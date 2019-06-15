#include "../menus.h"
#include "menus.ext"

/* menus - initialize the menus database
 */
MENUS *
menus ()
{
	MENUS	*msp;
	MENU	*mp;
	ITEM	*ip;
	char	*calloc();

	if ((msp = (MENUS *)calloc (1, sizeof(MENUS))) == (MENUS *)0)
	    return (msp);

#include "menus.vc"

	return (msp);
}
