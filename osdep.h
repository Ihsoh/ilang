#ifndef	__OSDEP_H__
#define	__OSDEP_H__

/*
	__OS_WIN__
	__OS_MAC__
	__OS_LINUX__
*/
#if	defined(__OS_WIN__)
#	define OSDEP_PATH_SEPERATOR		"\\"
#else
#	define OSDEP_PATH_SEPERATOR		"/"
#endif

#endif
