#ifndef _PROTO_H
#define _PROTO_H

#include <stdint.h>

enum marker_type 
{
	/* Start of stream */
	MARKER_TYPE_SOS		= 0x01,
	MARKER_TYPE_SOF		= 0x02
};

typedef uint16_t marker_type_t;
typedef uint16_t format_t;
typedef uint16_t image_format_t;
typedef uint8_t frame_type_t;
typedef uint16_t marker_size_t;
#define MARKER_DEF(name)	typedef struct name##_marker 		\
				{					\
					marker_type_t type;		\
					marker_size_t size;
#define MARKER_END(name)	} name##_marker_t;


MARKER_DEF(sos)
	format_t format;
#define FORMAT_T_IMAGE		0
#define FORMAT_T_DCT		1
#define FORMAT_T_DHUFFMAN	2	
#define FORMAT_T_SHUFFMAN	3

	image_format_t image_format;
#define IMAGE_F_YUV420		0
	
	uint32_t frames_number;
	uint32_t width;
	uint32_t height;
MARKER_END(sos)
	
enum frame_type 
{
	FRAME_TYPE_I	= 0x00,
	FRAME_TYPE_P	= 0x01,
	FRAME_TYPE_B	= 0x02
};

MARKER_DEF(sof)
	frame_type_t frame_type;
	uint16_t quant_coeff;
MARKER_END(sof)	

#endif //_PROTO_H