/******************************************************************************\
|*                                                                            *|
|* Copyright (c) 2007-2009 by Vivante Corp.  All Rights Reserved.             *|
|*                                                                            *|
|* The material in this file is confidential and contains trade secrets of    *|
|* Vivante Corporation. This is proprietary information owned by Vivante      *|
|* Corporation. No part of this work may be disclosed, reproduced, copied,    *|
|* transmitted, or used in any way for any purpose, without the express       *|
|* written permission of Vivante Corporation.                                 *|
|*                                                                            *|
\******************************************************************************/

/*
 * Vivante specific definitions and declarations for all API library.
 */
#ifndef __khrvivante_h_

#include "KHR/khrplatform.h"

/* EGL image type enum. */
typedef enum _khrIMAGE_TYPE
{
	KHR_IMAGE_TEXTURE_2D			= 1,
	KHR_IMAGE_TEXTURE_CUBE,
	KHR_IMAGE_TEXTURE_3D,
	KHR_IMAGE_RENDER_BUFFER,
	KHR_IMAGE_VG_IMAGE,
	KHR_IMAGE_PIXMAP,
	KHR_IMAGE_ANDROID_NATIVE_BUFFER,
	KHR_IMAGE_WAYLAND_BUFFER,
} khrIMAGE_TYPE;

#define KHR_EGL_IMAGE_MAGIC_NUM		0x47414D49  /* "IMAG" */

/* EGL Image */
typedef struct _khrEGL_IMAGE
{
	khronos_uint32_t            magic;
	khrIMAGE_TYPE				type;
	struct _gcoSURF *           surface;
	struct _gcoSURF *           srcSurface;

	union
	{
		struct _khrEGL_IMAGE_TEXTURE
		{
			khronos_uint32_t	width;
			khronos_uint32_t	height;
			
			/* Format defined in GLES. */
			khronos_uint32_t	format;

			khronos_int32_t     level;
			khronos_int32_t		face;
			khronos_int32_t		depth;

			/* Address offset in surface, for cubemap. */
			khronos_uint32_t	offset;

			khronos_int32_t		texture;
			void *              object;
		} texture;

		struct _khrEGL_IMAGE_PIXMAP
		{
			khronos_uint32_t    width;
			khronos_uint32_t	height;
			khronos_int32_t		format;
			khronos_int32_t		stride;

			void *              address;
#ifdef	EGL_API_XXX
			gctINT				seqNo;
#endif
		} pixmap;

		struct _khrEGL_IMAGE_VGIMAGE
		{
            struct _gcoSURF *   texSurface;

			khronos_uint32_t    width;
			khronos_uint32_t    height;
			khronos_uint32_t    offset_x;
			khronos_uint32_t    offset_y;

			khronos_uint32_t    format;
			khronos_uint32_t    allowedQuality;
            khronos_int32_t     dirty;
            khronos_int32_t     *dirtyPtr;
            khronos_int32_t     rootWidth;
            khronos_int32_t     rootHeight;
            khronos_int32_t     rootOffsetX;
            khronos_int32_t     rootOffsetY;
		} vgimage;

#if defined(ANDROID)
        struct _khrEGL_IMAGE_ANDROID
        {
            void *              native;
        } androidNativeBuffer;
#endif
	} u;
}
khrEGL_IMAGE;

typedef khrEGL_IMAGE * khrEGL_IMAGE_PTR;

#define __khrvivante_h_
#endif
