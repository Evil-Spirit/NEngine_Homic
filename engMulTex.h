#ifndef NENGINE_MULTITEXTURES_SUPPORT
#define NENGINE_MULTITEXTURES_SUPPORT
#include "engLog.h"
#include "engMisc.h"

#define GL_TEXTURE0_ARB                   0x84C0
#define GL_TEXTURE1_ARB                   0x84C1
#define GL_TEXTURE2_ARB                   0x84C2
#define GL_TEXTURE3_ARB                   0x84C3
#define GL_TEXTURE4_ARB                   0x84C4
#define GL_TEXTURE5_ARB                   0x84C5
#define GL_TEXTURE6_ARB                   0x84C6
#define GL_TEXTURE7_ARB                   0x84C7
#define GL_TEXTURE8_ARB                   0x84C8
#define GL_TEXTURE9_ARB                   0x84C9
#define GL_TEXTURE10_ARB                  0x84CA
#define GL_TEXTURE11_ARB                  0x84CB
#define GL_TEXTURE12_ARB                  0x84CC
#define GL_TEXTURE13_ARB                  0x84CD
#define GL_TEXTURE14_ARB                  0x84CE
#define GL_TEXTURE15_ARB                  0x84CF
#define GL_TEXTURE16_ARB                  0x84D0
#define GL_TEXTURE17_ARB                  0x84D1
#define GL_TEXTURE18_ARB                  0x84D2
#define GL_TEXTURE19_ARB                  0x84D3
#define GL_TEXTURE20_ARB                  0x84D4
#define GL_TEXTURE21_ARB                  0x84D5
#define GL_TEXTURE22_ARB                  0x84D6
#define GL_TEXTURE23_ARB                  0x84D7
#define GL_TEXTURE24_ARB                  0x84D8
#define GL_TEXTURE25_ARB                  0x84D9
#define GL_TEXTURE26_ARB                  0x84DA
#define GL_TEXTURE27_ARB                  0x84DB
#define GL_TEXTURE28_ARB                  0x84DC
#define GL_TEXTURE29_ARB                  0x84DD
#define GL_TEXTURE30_ARB                  0x84DE
#define GL_TEXTURE31_ARB                  0x84DF
#define GL_ACTIVE_TEXTURE_ARB             0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB      0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB          0x84E2

bool	mtexIsSupported = false;
int		mtexMaxTex = 0;



typedef void (APIENTRY * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);

typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);

PFNGLMULTITEXCOORD1DARBPROC		glMultiTexCoord1d		= NULL;
PFNGLMULTITEXCOORD2DARBPROC		glMultiTexCoord2d		= NULL;
PFNGLMULTITEXCOORD3DARBPROC		glMultiTexCoord3d		= NULL;
PFNGLMULTITEXCOORD4DARBPROC		glMultiTexCoord4d		= NULL;

PFNGLACTIVETEXTUREARBPROC		glActiveTexture			= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTexture	= NULL;

void mtexInit(void) {
	
	if (glIsExtensionSupported("GL_ARB_multitexture") && glIsExtensionSupported("GL_EXT_texture_env_combine"))
	{       
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&mtexMaxTex);
		logPrint("[*]\tDetected MultiTexture support.\n");
		logPrint("\t\tMax texture units is: %u\n",mtexMaxTex);
	
		glMultiTexCoord1d	= (PFNGLMULTITEXCOORD1DARBPROC) wglGetProcAddress("glMultiTexCoord1dARB");
		glMultiTexCoord2d	= (PFNGLMULTITEXCOORD2DARBPROC) wglGetProcAddress("glMultiTexCoord2dARB");
		glMultiTexCoord3d	= (PFNGLMULTITEXCOORD3DARBPROC) wglGetProcAddress("glMultiTexCoord3dARB");
		glMultiTexCoord4d	= (PFNGLMULTITEXCOORD4DARBPROC) wglGetProcAddress("glMultiTexCoord4dARB");
		glActiveTexture		= (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glClientActiveTexture= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
               
		mtexIsSupported = true;
	} else; mtexIsSupported = false;
}


#endif
