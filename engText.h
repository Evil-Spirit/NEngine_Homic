#ifndef NENGINE_TEXT_AND_FONT
#define NENGINE_TEXT_AND_FONT
#include <windows.h>
#include <stdio.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdarg.h>
#include "engVars.h"
struct tFont {
	unsigned int	texture,sx,sy,xc,yc;
};
tFont fntFonts[256];
void textCreateFont(unsigned char n,char *name, 
					unsigned int sx,unsigned int sy,
					unsigned int xc,unsigned int yc)
{
	fntFonts[n].texture = engTexturesList.add(name,name,false);
	fntFonts[n].sx = sx;
	fntFonts[n].sy = sy;
	fntFonts[n].xc = xc;
	fntFonts[n].yc = yc;
}
void textDeleteFont(void) {
}

void textPrint2D(unsigned char fnt,double x,double y, double size,const char *fmt, ...)
{

	char		text[256];
	va_list	ap;
	if (fmt == NULL) return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fntFonts[fnt].texture);
	double sizy = 2.0f/(double)engResY*(double)fntFonts[fnt].sy;
	double sizx = 2.0f/(double)engResX*(double)fntFonts[fnt].sx;
	double tsx = 1.0f/(double)fntFonts[fnt].xc;
	double tsy = 1.0f/(double)fntFonts[fnt].yc;
	double tx;
	double ty;
	for (unsigned int i=0;i<strlen(text);i++)
	{
		tx = double((text[i])%fntFonts[fnt].xc)*tsx;
		ty = double((text[i])/fntFonts[fnt].yc)*tsy;
		glBegin(GL_QUADS);
		
		glTexCoord2d(tx,1-ty);
		glVertex2d(0,sizy);
		
		glTexCoord2d(tx,1-ty-tsy);
		glVertex2d(0,0);
		
		glTexCoord2d(tx+tsx,1-ty-tsy);
		glVertex2d(sizx,0);

		glTexCoord2d(tx+tsx,1-ty);
		glVertex2d(sizx,sizy);
		glEnd();
		glTranslated(sizx,0,0);

	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
void textConStrPrint(unsigned char fnt,double x,double y, double size,const char *fmt, ...)
{

	char		text[256];
	va_list	ap;
	if (fmt == NULL) return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fntFonts[fnt].texture);
	double sizy = 2.0f/(double)engResY*(double)fntFonts[fnt].sy;
	double sizx = 2.0f/(double)engResX*(double)fntFonts[fnt].sx;
	double tsx = 1.0f/(double)fntFonts[fnt].xc;
	double tsy = 1.0f/(double)fntFonts[fnt].yc;
	double tx;
	double ty;
	double ox = 0;
	double oy = 0;
	for (unsigned int i=0;i<strlen(text);i++)
	{
		tx = double((text[i])%fntFonts[fnt].xc)*tsx;
		ty = double((text[i])/fntFonts[fnt].yc)*tsy;
		glBegin(GL_QUADS);
		
		glTexCoord2d(tx,1-ty);
		glVertex2d(ox,sizy+oy);
		
		glTexCoord2d(tx,1-ty-tsy);
		glVertex2d(ox,oy);
		
		glTexCoord2d(tx+tsx,1-ty-tsy);
		glVertex2d(sizx+ox,oy);

		glTexCoord2d(tx+tsx,1-ty);
		glVertex2d(sizx+ox,sizy+oy);
		glEnd();
		ox+=sizx;
		if(ox+x+sizx>1.0)
		{
			ox=0;
			oy-=sizy;
		}
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
#endif