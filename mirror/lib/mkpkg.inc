# Global MKPKG definitions for the MIRROR package.

$set XFLAGS	= "$(XFLAGS) -I/iraf/local/lib -Inolibc -p mirror"
$set XVFLAGS	= "$(XVFLAGS) -p mirror"
$set LFLAGS	= "-p mirror"

$ifeq (MACH, sparc) then
$include "mirror$lib/mkpkg.sf.SUN4"
$else $ifeq (MACH, i386) then
$include "mirror$lib/mkpkg.sf.I386"
$else $ifeq (MACH, f68881) then
$include "mirror$lib/mkpkg.sf.SUN3"
$else $ifeq (MACH, ffpa) then
$include "mirror$lib/mkpkg.sf.SUN3"
$else $ifeq (MACH, vms) then
$include "mirror$lib/mkpkg.sf.VMS"
$end
