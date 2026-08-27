#ifndef NENGINE_MATERIAL_SUPPORT
#define NENGINE_MATERIAL_SUPPORT

#include <stdio.h>
#include <windows.h>
#include <gl\glu.h>
#include <stdlib.h>
#include "engVars.h"
#include "engLog.h"
#include "engMisc.h"
#include <math.h>
struct TargaHeader {
	char	tmp[12];
	short	imgWidth,
			imgHeight,
			imgDepth;
};

struct Texture {
	int width,height;
	unsigned char *data;
};

class textureList {
public:
    void init(void);
    unsigned int *list;
	unsigned char listPointer;
	char **source;
    unsigned char findName(char *name);
    unsigned char add(char *name);
	unsigned char add(char *name, char *opa);
	unsigned char add(char *name, Texture tex);
	unsigned char textureList::add(char *name, char *opa, bool fil);
	void reload();
	void done(void);
};
Texture LoadTarga(char *name)
{
	FILE *f;
	TargaHeader head;
	Texture temp;
	temp.data=NULL;
	f=fopen(name,"rb");
	if (f==NULL) return(temp);
	fread(&head,sizeof(head),1,f);
	temp.width=head.imgWidth;
	temp.height=head.imgHeight;
	temp.data = new unsigned char [head.imgWidth*head.imgHeight*3];
	fread(temp.data,3,head.imgWidth*head.imgHeight,f);
	for(int i=0;i<head.imgWidth*head.imgHeight*3;i+=3)
		Swap(&temp.data[i],&temp.data[i+2]);
	fclose(f);
	return(temp);
}
Texture texBuildPhongMap(double amb,double shin) {
	Texture temp;
	temp.height	= 256;
	temp.width	= 256;
	temp.data	= new unsigned char[256*256*4];
	unsigned int tmp=0;
	for(unsigned int ii=0;ii<256;ii++)
		for(unsigned int jj=0;jj<256;jj++) {
			char col=(char)(amb+(0xFF-amb)*pow(sin(ii*PI/0x100)*sin(jj*PI/0x100),shin));
			temp.data[tmp++]=col;
			temp.data[tmp++]=col;
			temp.data[tmp++]=col;

		}
	return temp;
}
void textureList::init() {
	logPrint("[*]\tInitializing texture list...");
	source = new char *[256]; 
	glGenTextures(256,list);
	listPointer=0;
	logPrint("OK.\n");
}
void textureList::done() {
	logPrint("[-]\tDeallocating texture list...");
	for(int ii=0;ii<listPointer;ii++)
	delete []source[ii];
	delete []source;
	glGenTextures(0,NULL);
	delete []list;
	logPrint("OK.\n");
}
unsigned char textureList::findName(char *name) {
	for(unsigned char i=0;i<listPointer;i++) 
		if (strcmp(source[i],name)==0) return i;
	return 255;
}
unsigned char textureList::add(char *name) {
	unsigned char n=findName(name);
	if (n==255) {
		logPrint("[*]\tLoading texture (index %u)\t\"%s\"\n",listPointer,name);
		Texture img;
		char *tmp = new char [256];
		strcpy(tmp,TEXTURESPATH);
		strcat(tmp,name);
		source[listPointer] = new char[32];
		strcpy(source[listPointer],name);
		glBindTexture(GL_TEXTURE_2D,listPointer);
		img=LoadTarga(tmp); 
		if (img.data==NULL) return 255;
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,img.width,img.height,GL_RGB,GL_UNSIGNED_BYTE,img.data);
		glTexImage2D(GL_TEXTURE_2D,0,3,img.width,img.height,0,GL_RGB,GL_UNSIGNED_BYTE,img.data);
		unsigned int mag=0,min=0;
		if(engMip) {
			min=0x2700;
			if(engTri) min|=0x0002;
		} else min=0x2600;
		if(engBil) {
			mag =0x2601;
			min|=0x0001;
		} else mag=0x2600;
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(float)min);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,(float)mag);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		if (tmp) delete [] tmp;
		if (img.data) delete [] img.data;
		return listPointer++;
	} else return n;
}
unsigned char textureList::add(char *name, char *opa, bool fil) {
	char aa[32];
	char tmp[256];
	sprintf(aa,"%s(%s)",name,opa);
	unsigned char n=findName(aa);
	if (n==255) {
		Texture img,opac,res;

		strcpy(tmp,TEXTURESPATH);
		strcat(tmp,name);
		source[listPointer] = new char[32];
		strcpy(source[listPointer],aa);
		logPrint("[*]\tLoading diffuse (index %u)\t\"%s\"\n",listPointer,name);
		img=LoadTarga(tmp); 
		if (img.data==NULL) return 255;
		tmp[0] = '\0';
		strcpy(tmp,TEXTURESPATH);
		strcat(tmp,opa);
		logPrint("[*]\tLoading opacity (index %u)\t\"%s\"\n",listPointer,opa);
		opac=LoadTarga(tmp); 
		if (opac.data==NULL) return 255;
		if (opac.width!=img.width||opac.height!=img.height) return 255;
		res.width	= img.width;
		res.height	= img.height;
		res.data	= new unsigned char [img.width*img.height*4];
		for (int c = 0;c<img.width*img.height;c++) {
			res.data[c*4+0] = img.data[c*3+0];
			res.data[c*4+1] = img.data[c*3+1];
			res.data[c*4+2] = img.data[c*3+2];
			res.data[c*4+3] =
				(opac.data[c*3+0]+
				 opac.data[c*3+1]+
			  	 opac.data[c*3+2])/3;
		}
		glBindTexture(GL_TEXTURE_2D,listPointer);
		gluBuild2DMipmaps(GL_TEXTURE_2D,4,res.width,res.height,GL_RGBA,GL_UNSIGNED_BYTE,res.data);
		glTexImage2D(GL_TEXTURE_2D,0,4,res.width,res.height,0,GL_RGBA,GL_UNSIGNED_BYTE,res.data);
		unsigned int mag=0,min=0;
		if(engMip) {
			min=0x2700;
			if(engTri) min|=0x0002;
		} else min=0x2600;
		if(engBil) {
			mag =0x2601;
			min|=0x0001;
		} else mag=0x2600;
		if (!fil) 
		{
			min = GL_NEAREST;
			mag = GL_NEAREST;
		}
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(float)min);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,(float)mag);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		if (img.data) delete [] img.data;
		if (opac.data) delete [] opac.data;
		if (res.data) delete [] res.data;
		return listPointer++;
	} else return n;
}
unsigned char textureList::add(char *name, char *opa) {
	char aa[32];
	char tmp[256];
	sprintf(aa,"%s(%s)",name,opa);
	unsigned char n=findName(aa);
	if (n==255) {
		Texture img,opac,res;

		strcpy(tmp,TEXTURESPATH);
		strcat(tmp,name);
		source[listPointer] = new char[32];
		strcpy(source[listPointer],aa);
		logPrint("[*]\tLoading diffuse (index %u)\t\"%s\"\n",listPointer,name);
		img=LoadTarga(tmp); 
		if (img.data==NULL) return 255;
		tmp[0] = '\0';
		strcpy(tmp,TEXTURESPATH);
		strcat(tmp,opa);
		logPrint("[*]\tLoading opacity (index %u)\t\"%s\"\n",listPointer,opa);
		opac=LoadTarga(tmp); 
		if (opac.data==NULL) return 255;
		if (opac.width!=img.width||opac.height!=img.height) return 255;
		res.width	= img.width;
		res.height	= img.height;
		res.data	= new unsigned char [img.width*img.height*4];
		for (int c = 0;c<img.width*img.height;c++) {
			res.data[c*4+0] = img.data[c*3+0];
			res.data[c*4+1] = img.data[c*3+1];
			res.data[c*4+2] = img.data[c*3+2];
			res.data[c*4+3] =
				(opac.data[c*3+0]+
				 opac.data[c*3+1]+
			  	 opac.data[c*3+2])/3;
		}
		glBindTexture(GL_TEXTURE_2D,listPointer);
		gluBuild2DMipmaps(GL_TEXTURE_2D,4,res.width,res.height,GL_RGBA,GL_UNSIGNED_BYTE,res.data);
		glTexImage2D(GL_TEXTURE_2D,0,4,res.width,res.height,0,GL_RGBA,GL_UNSIGNED_BYTE,res.data);
		unsigned int mag=0,min=0;
		if(engMip) {
			min=0x2700;
			if(engTri) min|=0x0002;
		} else min=0x2600;
		if(engBil) {
			mag =0x2601;
			min|=0x0001;
		} else mag=0x2600;
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(float)min);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,(float)mag);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		if (img.data) delete [] img.data;
		if (opac.data) delete [] opac.data;
		if (res.data) delete [] res.data;
		return listPointer++;
	} else return n;
}
unsigned char textureList::add(char *name, Texture tex) {
	if (tex.data==NULL) return 255;
	unsigned char n = findName(name);
	if (n==255) {
		logPrint("[*]\tSetting texture (index %u)\t\"%s\"\n",listPointer,name);
		source[listPointer] = new char[32];
		strcpy(source[listPointer],name);
		glBindTexture(GL_TEXTURE_2D,listPointer);
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,tex.width,tex.height,GL_RGB,GL_UNSIGNED_BYTE,tex.data);
		glTexImage2D(GL_TEXTURE_2D,0,3,tex.width,tex.height,0,GL_RGB,GL_UNSIGNED_BYTE,tex.data);
		unsigned int mag=0,min=0;
		if(engMip) {
			min=0x2700;
			if(engTri) min|=0x0002;
		} else min=0x2600;
		if(engBil) {
			mag =0x2601;
			min|=0x0001;
		} else mag=0x2600;
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(float)min);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,(float)mag);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		return listPointer++;
	} else return n;
}
void textureList::reload() {
	unsigned int mag=0,min=0;
	if(engMip) {
		min=0x2700;
		if(engTri) min|=0x0002;
	} else min=0x2600;
	if(engBil) {
		mag =0x2601;
		min|=0x0001;
	} else mag=0x2600;
	logPrint("in %x ag %x\n",min,mag);
	glGenTextures(0,NULL);
	delete []list;
	glGenTextures(256,list);
	for(int ii=0;ii<listPointer;ii++) {
		logPrint("[*]\tReloading texture (index %u)\t\"%s\"\n",ii,source[ii]);
		Texture img;
		char *tmp = new char [256];
		strcpy(tmp,TEXTURESPATH);
		strcat(tmp,source[ii]);
		glBindTexture(GL_TEXTURE_2D,ii);
		img=LoadTarga(tmp); 
		if (img.data==NULL) return;
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,img.width,img.height,GL_RGB,GL_UNSIGNED_BYTE,img.data);
		glTexImage2D(GL_TEXTURE_2D,0,3,img.width,img.height,0,GL_RGB,GL_UNSIGNED_BYTE,img.data);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(float)min);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,(float)mag);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		if (tmp) delete [] tmp;
		if (img.data) delete [] img.data;
	}
}
#endif
