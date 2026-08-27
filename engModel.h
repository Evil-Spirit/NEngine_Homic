#ifndef NENGINE_MODELS
#define NENGINE_MODELS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "engTypes.h"
#include "engImage.h"
#include "engVars.h"
#include "engMisc.h"
#include "engLog.h"
#include "engMulTex.h"

struct tMatFlags {
	unsigned twoside	:1;
	unsigned wire		:1;
	unsigned diffuse	:1;
	unsigned opacity	:1;
	unsigned reflect	:1;
};
struct tMaterial {
	tColor		diffuse;
	double		transparency;
	tMatFlags	flags;
	unsigned	char idMapDiffuse;
	unsigned	char idMapOpacity;
	unsigned	char idMapReflect;
};
struct tPoint {
	tVector pos;
	unsigned int info;
};
struct tCoord {
	double u,v;
};
struct tVertex {
	tVector pos;
	tVector normal;
};
struct tFace { 
	unsigned int vertex[3],coord[3],id;
};
struct tFaceN { 
	unsigned int vertex[3],coord[3],id;
	tVector normal;
	double dist;
};
struct tFrame {
	tVector *nodes;
	double time;
};
struct tAnim {
	unsigned int countFrames;
	tFrame *frames;
};
class cModelAnimate {
private:
public:
	unsigned int currentAnim;
	double modelTime;
	bool inRen;
	unsigned int
		countVertex,
		countFace,
		countCoord,
		countAnim;
	tAnim *anim;
	tFace *face;
	tCoord *coord;
	void load(char *name);
	void deAllocate(void);
	void setAnim(unsigned int num);
	void incAnim(double val);
	unsigned int findKey(double time); 
	tVector lineInterpolate(unsigned int nn, double modelT);
	void render(void);
	void renderInc(double val);
	void resetTime(void);
};
class cModelStatic {
public:
	unsigned int 
		countVertex,
		countFace,
		countCoord,
		countMats,
		countPoint;
	tMaterial	*mat;
	tVertex		*vertex;
	tFaceN		*face;
	tCoord		*coord;
	tPoint		*points;
	tVector		pbbox,sbbox;
	unsigned char phongMap; 
	void calcCoord(void);
	void load(char *name);
	void render(void);
	void deAllocate(void);
};
void cModelAnimate::load(char *name) {
	inRen = false;
	logPrint("[*]\tLoading model animated file \"%s\"...\n",name);
	FILE *f = fopen(name,"rb");
	if (f==NULL) {
		logPrint("\t[!]\tError: file not found.\n");
		return;
	}
	unsigned int mustHeader = 'M'|'D'<<8|'A'<<16|'0'<<24,head;
	fread(&head,4,1,f);
	if (head!=mustHeader) {
		logPrint("\t[!]\tError: unknown file format.\n",name);
		fclose(f);
		return;
	}
	unsigned char countTex;
	currentAnim=0;
	modelTime=0;
	fread(&countVertex	,4,1,f);
	fread(&countFace	,4,1,f);
	fread(&countCoord	,4,1,f);
	fread(&countAnim	,4,1,f);
	fread(&countTex,1,1,f);
	logPrint("\tModel have:\n");
	logPrint("\t%u\tTextures\n",countTex);
	unsigned char	*tex = new unsigned char [countTex];
	char	*buf = new char [32];
	for(unsigned char k=0;k<countTex;k++) {
		fread(buf,32,1,f);
		tex[k]=engTexturesList.add(buf);
	}
	face = new tFace [countFace];
	fread(face,sizeof(tFace),countFace,f);
	for(unsigned int p=0;p<countFace;p++) 
		face[p].id=tex[face[p].id];
	coord = new tCoord [countCoord];
	fread(coord,sizeof(tCoord),countCoord,f);
	anim = new tAnim [countAnim];
	logPrint("\t%u\tVertexes\n",countVertex);
	logPrint("\t%u\tFaces\n",countFace);
	logPrint("\t%u\tTexcoords\n",countCoord);
	logPrint("\t%u\tAnims\n",countAnim);
	for (unsigned int i=0;i<countAnim;i++) {
		fread(&anim[i].countFrames,4,1,f);
		logPrint("\tAnim #%u\t\thave %u\tframes\n",i,anim[i].countFrames);
		anim[i].frames = new tFrame [anim[i].countFrames];
		for (unsigned int j=0;j<anim[i].countFrames;j++) {
			anim[i].frames[j].nodes = new tVector [countVertex];
			fread(&anim[i].frames[j].time,sizeof(double),1,f);
			fread( anim[i].frames[j].nodes,sizeof(tVector),countVertex,f);
		}
	}
	fclose(f);
	delete []tex;
	delete []buf;
	logPrint("[+]\tModel was loaded successfully.\n",countCoord);
}
void cModelAnimate::deAllocate(void) {
	logPrint("[-]\tDeallocating model animated...");
	delete []face;
	delete []coord;
	for (unsigned int i=0;i<countAnim;i++) {
		for (unsigned int j=0;j<anim[i].countFrames;j++) 
			delete []anim[i].frames[j].nodes;
		delete []anim[i].frames;
	}
	delete []anim;
	logPrint("OK.\n");
}
void cModelAnimate::setAnim(unsigned int num) {
	if (num<countAnim) currentAnim = num;
};
void cModelAnimate::resetTime(void) {
	modelTime=0;
};
void cModelAnimate::incAnim(double val) {
//	if (inRen) return;
	modelTime+=val;
	if (modelTime>1.0000) modelTime=0.0; else
	if (modelTime<0.0000) modelTime=1.0;
};
unsigned int cModelAnimate::findKey(double time) {
	unsigned int currentAni = currentAnim;
	for(unsigned int i=0;i<anim[currentAni].countFrames;i++) 
		if (anim[currentAni].frames[i].time<time && 
			anim[currentAni].frames[(i+1)%anim[currentAni].countFrames].time>=time) return i;
	return 0;
}
tVector cModelAnimate::lineInterpolate(unsigned int nn, double modelT) {
	tVector node;
	unsigned int currentAni = currentAnim;
	node.x=node.y=node.z=0;
	unsigned int k = findKey(modelT);
	double d;
	d=(anim[currentAni].frames[(k+1)%anim[currentAni].countFrames].nodes[nn].x-
	   anim[currentAni].frames[k].nodes[nn].x)/
	  (anim[currentAni].frames[(k+1)%anim[currentAni].countFrames].time-
	   anim[currentAni].frames[k].time);
	node.x=(modelT-anim[currentAni].frames[k].time)*d+anim[currentAni].frames[k].nodes[nn].x;

	d=(anim[currentAni].frames[(k+1)%anim[currentAni].countFrames].nodes[nn].y-
		anim[currentAni].frames[k].nodes[nn].y)/
			(anim[currentAni].frames[(k+1)%anim[currentAni].countFrames].time-
			anim[currentAni].frames[k].time);
	node.y=(modelT-anim[currentAni].frames[k].time)*d+anim[currentAni].frames[k].nodes[nn].y;

	d=(anim[currentAni].frames[(k+1)%anim[currentAni].countFrames].nodes[nn].z-
		anim[currentAni].frames[k].nodes[nn].z)/
			(anim[currentAni].frames[(k+1)%anim[currentAni].countFrames].time-
			anim[currentAni].frames[k].time);
	node.z=(modelT-anim[currentAni].frames[k].time)*d+anim[currentAni].frames[k].nodes[nn].z;

	return node;
}
void cModelAnimate::render(void) {
	double MT = modelTime;
	inRen = true;
	tVector v;
	v.x=v.y=v.z=0;
	glColor3d(1,1,1);
	for (unsigned int j=0;j<countFace;j++) {
		glBindTexture(GL_TEXTURE_2D,face[j].id); 
		glBegin(GL_TRIANGLES);
		for (unsigned int k=0;k<3;k++) {
			v=lineInterpolate(face[j].vertex[k],MT);
			if (countCoord) 
			glTexCoord2d(
				coord[face[j].coord[k]].u,
				coord[face[j].coord[k]].v);
			glVertex3d(v.x,v.y,v.z);
		}
		glEnd();
	}	
	inRen = false;
}
void cModelAnimate::renderInc(double val) {
	double MT = modelTime;
	tVector v;
	v.x=v.y=v.z=0;
	for (unsigned int j=0;j<countFace;j++) {
		glBindTexture(GL_TEXTURE_2D,face[j].id); 
		glBegin(GL_TRIANGLES);
		for (unsigned int k=0;k<3;k++) {
			v=lineInterpolate(face[j].vertex[k],MT);
			if (countCoord) 
			glTexCoord2d(
				coord[face[j].coord[k]].u,
				coord[face[j].coord[k]].v);
			glVertex3d(v.x,v.y,v.z);
		}
		glEnd();
	}
	incAnim(val);
}
void cModelStatic::load(char *name) {
	logPrint("[*]\tLoading model static file \"%s\"...\n",name);
	FILE *f = fopen(name,"rb");
	if (f==NULL) {
		logPrint("[!]\tError: file not found.\n");
		return;
	}
	unsigned int mustHeader = 'M'|'D'<<8|'S'<<16|'2'<<24,head;
	fread(&head,4,1,f);
	if (head!=mustHeader) {
		logPrint("[!]\tError: unknown file format.\n",name);
		fclose(f);
		return;
	}
	unsigned char countTex;
	fread(&countVertex	,4,1,f);
	fread(&countFace	,4,1,f);
	fread(&countCoord	,4,1,f);
	fread(&countTex		,1,1,f);
	fread(&countMats	,4,1,f);
	fread(&countPoint	,4,1,f);
	fread(&pbbox,sizeof(pbbox),1,f);
	fread(&sbbox,sizeof(sbbox),1,f);
	logPrint("\tModel have:\n");
	logPrint("\t%u\tTextures\n",countTex);
	unsigned char	*tex = new unsigned char [countTex];
	char	**tnam = new char*[countTex];
	char	*buf = new char [32];
	for(unsigned char k=0;k<countTex;k++) {
		tnam[k] = new char[32];
		fread(tnam[k],32,1,f);
	}
	logPrint("\t%u\tMaterials\n",countMats);
	logPrint("\t%u\tVertexes\n",countVertex);
	logPrint("\t%u\tFaces\n",countFace);
	logPrint("\t%u\tTexcoords\n",countCoord);
	mat = new tMaterial [countMats];
	fread(mat,sizeof(tMaterial),countMats,f);
	face = new tFaceN [countFace];
	fread(face,sizeof(tFaceN),countFace,f);
	for(unsigned int p=0;p<countMats;p++) {
		if (mat[p].flags.diffuse) 
			if (mat[p].flags.opacity) 
				mat[p].idMapDiffuse = engTexturesList.add(
					tnam[mat[p].idMapDiffuse],
					tnam[mat[p].idMapOpacity]);
			else
				mat[p].idMapDiffuse = engTexturesList.add(tnam[mat[p].idMapDiffuse]);
			else
				mat[p].idMapDiffuse = 0;

		if (mat[p].flags.reflect) 
				mat[p].idMapReflect = engTexturesList.add(tnam[mat[p].idMapReflect]);
			else
				mat[p].idMapReflect = 0;
	}
	coord = new tCoord [countCoord];
	fread(coord,sizeof(tCoord),countCoord,f);
	vertex = new tVertex [countVertex];
	fread(vertex,sizeof(tVertex),countVertex,f);
	points = new tPoint [countPoint];
	fread(points,sizeof(tPoint),countPoint,f);
	fclose(f);
	delete []tex;
	delete []buf;
	logPrint("[+]\tModel was loaded successfully.\n",countCoord);
}
void cModelStatic::calcCoord(void){
	for(unsigned int ii=0;ii<countFace;ii++)
		for(unsigned int jj=0;jj<3;jj++){
			tVector tmp;
//			tMatrix mat;
//			glGetFloatv(GL_MODELVIEW_MATRIX,(float *)mat);
//			tmp=vertex[face[ii].vertex[jj]].normal|mat;
			tmp=vertex[face[ii].vertex[jj]].normal;
			coord[face[ii].coord[jj]].u=(1+tmp.x)/2;
			coord[face[ii].coord[jj]].v=(1+tmp.y)/2;
		}
}
void cModelStatic::deAllocate(void) {
	logPrint("[-]\tDeallocating model static...");
	delete []face;
	delete []mat;
	delete []coord;
	delete []vertex;
	delete []points;
	logPrint("OK.\n");
};
void cModelStatic::render(void) {
	tVector vec,vecr;
	tMatrix matr;
	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)matr);
	bool	lasTra, lasTwo, lasTex;
	bool	curTra, curTwo, curTex;
	unsigned int	curId;
	unsigned int	lasId;
	for (unsigned int j=0;j<countFace;j++) {

		curTra = mat[face[j].id].transparency!=1.0f ||
				 mat[face[j].id].flags.opacity;
		if ((lasTra!=curTra)||!j)
		{
			if (curTra) glEnable(GL_BLEND); else glDisable(GL_BLEND);
			lasTra = curTra;
		}
		
		curTwo = mat[face[j].id].flags.twoside;
		if ((curTwo!=lasTwo)||!j) 
		{
			if (curTwo) glDisable(GL_CULL_FACE); else glEnable(GL_CULL_FACE);
			lasTwo = curTwo;
		}

		curTex = mat[face[j].id].flags.diffuse || mat[face[j].id].flags.reflect;
		if((curTex!=lasTex)||!j) 
		{
			if (curTex) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);
			lasTex = curTex;
		}
		glColor4d(
			mat[face[j].id].diffuse.r,
			mat[face[j].id].diffuse.g,
			mat[face[j].id].diffuse.b,
			mat[face[j].id].transparency);

		if (mat[face[j].id].flags.diffuse)
		{
			curId =  mat[face[j].id].idMapDiffuse;
			if (curId!=lasId) {
				glBindTexture(GL_TEXTURE_2D,curId); 
				lasId = curId;
			}
		}
		if (!(mat[face[j].id].flags.reflect&&
			 !mat[face[j].id].flags.diffuse)) 
		{
			glBegin(GL_TRIANGLES);
			for (unsigned int k=0;k<3;k++) {
				glTexCoord2d(
					coord[face[j].coord[k]].u,
					coord[face[j].coord[k]].v);
				glVertex3d(
					vertex[face[j].vertex[k]].pos.x,
					vertex[face[j].vertex[k]].pos.y,
					vertex[face[j].vertex[k]].pos.z);
			}
			glEnd();
		}
		if (mat[face[j].id].flags.reflect)
		{
			if(!lasTra)
			glEnable(GL_BLEND);
		if (!(mat[face[j].id].flags.reflect&&!mat[face[j].id].flags.diffuse)) 
			glBlendFunc(GL_SRC_COLOR,GL_ONE);

			curId =  mat[face[j].id].idMapReflect;
			if (curId!=lasId) {
				glBindTexture(GL_TEXTURE_2D,curId); 
				lasId = curId;
			}
			glBegin(GL_TRIANGLES);
			for (unsigned int k=0;k<3;k++) {
				vec = vertex[face[j].vertex[k]].normal;
				vecr = vecRotOnly(vec, matr);
				glTexCoord2d((vecr.x+1)/2,(vecr.y+1)/2);

				glVertex3d(
					vertex[face[j].vertex[k]].pos.x,
					vertex[face[j].vertex[k]].pos.y,
					vertex[face[j].vertex[k]].pos.z);
			}
			glEnd();
			if(!lasTra)
			glDisable(GL_BLEND);
			if (!(mat[face[j].id].flags.reflect&&!mat[face[j].id].flags.diffuse)) 
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		}
	}	
}
void modDrawBillboard(double s) {

	tMatrix matr;
	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)matr);

	tVector cp;
	cp.x = matr[3][0];
	cp.y = matr[3][1];
	cp.z = matr[3][2];
	double hs = s/2.0f;
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);

	glTexCoord2d(0,0);
	glVertex3d(cp.x-hs, cp.y-hs, cp.z);

	glTexCoord2d(1,0);
	glVertex3d(cp.x+hs, cp.y-hs, cp.z);
	
	glTexCoord2d(1,1);
	glVertex3d(cp.x+hs, cp.y+hs, cp.z);
	
	glTexCoord2d(0,1);
	glVertex3d(cp.x-hs, cp.y+hs, cp.z);

	glEnd();
	glPopMatrix();
}
#endif
