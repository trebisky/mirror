#define	import_spp
#include <iraf.h>
#include "oxnames.h"

/* logdata - logdata C/SPP interface
 */
logdata (prefix, pix, ny)
char	*prefix;
float	*pix;
int	ny;
{
	XCHAR	pre[SZ_FNAME];

	c_strupk (prefix, pre, strlen (prefix) + 1);
	LGDATA (pre, pix, &ny);
}
