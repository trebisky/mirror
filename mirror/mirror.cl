#{ The STEWARD MIRROR lab programs package.

cl < "mirror$lib/zzsetenv.def"
package mirror, bin = mirrorbin$

set	oven		= "mirror$src/oven/"

task	ograph,
	xytran,
	tcmark,
	odisp,
	ovend,
	ovene,
	oveng,
	ovenp,
	ovenb,
	ovenw,
	ovenr,
	oven		= mirror$x_mirror.e

hidetask	xytran

keep
clbye()
