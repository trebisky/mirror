#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

csoog ()
{
	int	t = 0;

	t = menugoto ("oo");
	return (t);
}

cstcg ()
{
	int	t = 0;

	t = menugoto ("tc");
	return (t);
}

cstci (s)
char	*s;
{
	int	t = 0;

	t = sctcloc (s, Pdb, &Gdntx);
	return (t);
}

cstco (s)
char	*s;
{
	int	t = 0;

	prloc (s, &Ptc->loc);
	return (t);
}

csheg ()
{
	int	t = 0;

	t = menugoto ("he");
	return (t);
}

cshei (s)
char	*s;
{
	int	t = 0;

	t = scheloc (s, Pdb, &Gpfe);
	return (t);
}

csheo (s)
char	*s;
{
	int	t = 0;

	prloc (s, &Phe->loc);
	return (t);
}

cszog ()
{
	int	t = 0;

	if (Gzone == ROT_ZONE)
	    t = menugoto ("rz");
	else
	    t = menugoto ("zo");
	return (t);
}

cszoi (s)
char	*s;
{
	int	t = 0;

	t = sczone (s, &Gzone);
	return (t);
}

cszoo (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

cstig ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

cstii (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

cstio (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

csdcg ()
{
	int	t = 0;

	t = menugoto ("dc");
	return (t);
}

csdci (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

csdco (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

cssbg ()
{
	int	t = 0;

	t = menugoto ("sb");
	return (t);
}

cssbi (s)
char	*s;
{
	int	t = 0;

	t = scdsb (s, &Gdsb);
	return (t);
}

cssbo (s)
char	*s;
{
	int	t = 0;

	prdsb (s, Gdsb);
	return (t);
}

cstyg ()
{
	int	t = 0;

	t = menugoto ("ty");
	return (t);
}

cstyi (s)
char	*s;
{
	int	t = 0;

	t = sctctype (s, &Gtctype);
	return (t);
}

cstyo (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Gtctype);
	return (t);
}

csfsg ()
{
	int	t = 0;

	t = menugoto ("fs");
	return (t);
}

csfsi (s)
char	*s;
{
	int	t = 0;

	t = scpfe (s, &Gpfe);
	return (t);
}

csfso (s)
char	*s;
{
	int	t = 0;

	prpfe (s, Gpfe);
	return (t);
}

csppg ()
{
	int	t = 0;

	t = menugoto ("pp");
	return (t);
}

csppi (s)
char	*s;
{
	int	t = 0;

	t = scpfe (s, &Gpfe);
	return (t);
}

csppo (s)
char	*s;
{
	int	t = 0;

	prpfe (s, Gpfe);
	return (t);
}

csadg ()
{
	int	t = 0;

	t = menugoto ("ad");
	return (t);
}

csalg ()
{
	int	t = 0;

	t = menugoto ("al");
	return (t);
}

csjtg ()
{
	int	t = 0;

	t = menugoto ("jt");
	return (t);
}

csjti (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

csjto (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

csttg ()
{
	int	t = 0;

	t = menugoto ("tt");
	return (t);
}

cstti (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

cstto (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

cshtg ()
{
	int	t = 0;

	t = menugoto ("ht");
	return (t);
}

csztg ()
{
	int	t = 0;

	t = menugoto ("zt");
	return (t);
}

cszti (s)
char	*s;
{
	int	t = 0;

	t = sczone (s, &Gzone);
	return (t);
}

cszto (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

cshpg ()
{
	int	t = 0;

	t = menugoto ("hp");
	return (t);
}

cszpg ()
{
	int	t = 0;

	t = menugoto ("zp");
	return (t);
}

cszpi (s)
char	*s;
{
	int	t = 0;

	t = sczone (s, &Gzone);
	return (t);
}

cszpo (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

csglg ()
{
	int	t = 0;

	t = menugoto ("gl");
	return (t);
}

csmsg ()
{
	int	t = 0;

	t = menugoto ("ms");
	return (t);
}
