#ifndef _HR_CLOBALDEF_H_
#define _HR_CLOBALDEF_H_

#ifndef TRUE
	#define TRUE		1
#endif

#ifndef FALSE
	#define FALSE		0
#endif

#ifndef NULL
	#define NULL		0
#endif

#ifndef max
	#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
	#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//data type definition
typedef char			sint8;
//typedef char			uint8;
typedef short			sint16;
typedef sint16			int16;
typedef unsigned short	uint16;
typedef int				sint32;
typedef sint32			int32;
typedef unsigned int	uint32;
typedef long			slong;
typedef unsigned long	ulong;
typedef int				sint;
typedef unsigned int	uint;
typedef unsigned char	byte;
typedef double			float64;

#endif
