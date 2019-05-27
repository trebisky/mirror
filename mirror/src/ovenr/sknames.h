/* sknames.h - socket kernel names
 */

#ifdef	AOSVS
#define	ZWMSEC	ZWMSEC
#endif
#ifdef	VMS
#define	ZWMSEC	zwmsec
#endif
#ifndef	AOSVS
#ifndef	VMS
#define	ZWMSEC	zwmsec_
#endif
#endif

#ifdef	AOSVS
#define	ZOPNSO	ZOPNSO
#define	ZCLSSO	ZCLSSO
#define	ZARDSO	ZARDSO
#define	ZAWRSO	ZAWRSO
#define	ZAWTSO	ZAWTSO
#define	ZSTTSO	ZSTTSO
#endif
#ifdef	VMS
#define	ZOPNSO	zopnso
#define	ZCLSSO	zclsso
#define	ZARDSO	zardso
#define	ZAWRSO	zawrso
#define	ZAWTSO	zawtso
#define	ZSTTSO	zsttso
#endif
#ifndef	AOSVS
#ifndef	VMS
#define	ZOPNSO	zopnso_
#define	ZCLSSO	zclsso_
#define	ZARDSO	zardso_
#define	ZAWRSO	zawrso_
#define	ZAWTSO	zawtso_
#define	ZSTTSO	zsttso_
#endif
#endif
