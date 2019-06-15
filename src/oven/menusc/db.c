#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

dbrfg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_bpread_disk ();
	t = GOER_EXECUTED;
	return (t);
}

dbwfg ()
{
	int	t = 0;

	db_bpwrite_disk ();
	t = GOER_EXECUTED;
	return (t);
}

dbrog ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_bpread_oven ();
	t = GOER_EXECUTED;
	return (t);
}

dbwog ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_bpwrite_oven ();
	t = GOER_EXECUTED;
	return (t);
}

dbrdg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_dread_oven ();
	t = GOER_EXECUTED;
	return (t);
}

dbreg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_eread_oven ();
	t = GOER_EXECUTED;
	return (t);
}

dbwtg ()
{
	int	t = 0;

	databascw ();
	t = GOER_EXECUTED;
	return (t);
}

dbrtg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	databascr ();
	t = GOER_EXECUTED;
	return (t);
}
