#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

aaexg ()
{
	int	t = 0;

	Gmenu = -1;
	return (t);
}

aacsg ()
{
	int	t = 0;

	t = menugoto ("cs");
	return (t);
}

aaerg ()
{
	int	t = 0;

	t = menugoto ("er");
	return (t);
}

aadbg ()
{
	int	t = 0;

	t = menugoto ("db");
	return (t);
}
