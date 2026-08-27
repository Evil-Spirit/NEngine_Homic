#ifndef NENGINE_USER_DEFINED_FUNCTIONS
#define NENGINE_USER_DEFINED_FUNCTIONS

#include <math.h>
#include <windows.h>
#include <gl\\gl.h>
#include <gl\\glu.h>

#include "engInput.h"
#include "engTypes.h"
#include "engImage.h"
#include "engModel.h"
#include "engVars.h"
#include "engVector.h"
#include "engConsts.h"
#include "engMisc.h"
#include "engText.h"
#include "engWpn.h"
#include "engParticle.h"

cModelAnimate	gamModel;
cModelAnimate	gamModel1;
cModelStatic	gamLevel;
cModelStatic	gamSky;
cModelStatic	gamWep[4];
tVector			pos = vecVec(0,METER,0);
tVector			d = vecVec(0,0,0);
tVector			a = vecVec(0,0,0);
cWeapon			ww;
cParSys			ooo;
int				curWep = 0;
double			alp;
#define			mFac 0.016
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////

bool usrSceneInit(void)
{
//////////////////////////////////////////////////////////////////////////////////////////////
	engTexturesList.init();
	engTexturesList.add("blast.tga");
	textCreateFont(0,"Font2.tga",8,16,16,16);
	inInit();
//////////////////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);    
	glEnable(GL_COLOR_MATERIAL);    
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA_TEST);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glAlphaFunc(GL_GREATER,0.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glClear(GL_COLOR_BUFFER_BIT);
//////////////////////////////////////////////////////////////////////////////////////////////
	conInit(engTexturesList.add("tex076.tga"),"NEngine.con");
	conMaxSize=2;
	conMomentToggle();
	conHistory("Loading...");
	conDraw();
	SwapBuffers(engDC);
//////////////////////////////////////////////////////////////////////////////////////////////
	gamLevel.load("mds\\homic.mds");
	
 	gamModel.load ("mda\\womdown.mda");
	gamModel1.load("mda\\womup.mda");


	gamSky.load("mds\\SkyBox.mds");
	gamWep[0].load("mds\\akm.mds");
	gamWep[1].load("mds\\pushka.mds");
	gamWep[2].load("mds\\rpg7.mds");
	gamWep[3].load("mds\\ppsha.mds");
	ww.load("weapons\\ak-__.wpn");
	movStep=1;
//////////////////////////////////////////////////////////////////////////////////////////////
	conMomentToggle();
	conMaxSize=1;
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////

bool usrSceneDone(void) {
	inDone();
	textDeleteFont();
	engTexturesList.done();	
	gamModel.deAllocate();
	gamModel1.deAllocate();
	gamWep[0].deAllocate();
	gamWep[1].deAllocate();
	gamWep[2].deAllocate();
	gamLevel.deAllocate();
	gamSky.deAllocate();
	conDone();
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////

void usrSceneReSize(int width, int height) {
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(engFov,(float)width/(float)height, engMinZ, engMaxZ);
	glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void usrMouseDown(unsigned int but) {
	
}
void usrMouseUp(unsigned int but) {
	
}

void usrSceneChange(unsigned int time) {
	inGetStatus();
	conProcess();

	if(inKey(12)&&engFov>001) engFov--;
	if(inKey(13)&&engFov<179) engFov++;
	if(inKey(2)) curWep = 0;
	if(inKey(3)) curWep = 1;
	if(inKey(4)) curWep = 2;
	if(inKey(5)) curWep = 3;

	if(inKey(75)) alp+=mFac*100;
	if(inKey(77)) alp-=mFac*100;
	if(inKey(80)) 
	{
		pos.x+=cos((-alp+90)*PI_180)*mFac*100;
		pos.z+=sin((-alp+90)*PI_180)*mFac*100;
		gamModel.incAnim(-mFac);
		gamModel.setAnim(1);
	} else
	if(inKey(72)) 
	{
		pos.x-=cos((-alp+90)*PI_180)*mFac*100;
		pos.z-=sin((-alp+90)*PI_180)*mFac*100;
		gamModel.incAnim(+mFac);
		gamModel.setAnim(1);
	} else
	{
		gamModel.setAnim(0);
		gamModel.incAnim(+mFac);
		gamModel1.incAnim(+mFac);
	}

	
	if(inKey(60)) {
		mainEngineShutdown();
		mainEngineInit("Evil Spirit OpenGL Engine",engResX,engResY,engBpp,!engFScr);
	}

	if(inKey(29)) d.y+=movStep;
	if(inKey(57)) d.y-=movStep; 
	
	if(inKey(17)) { 
		d.x+=cos((a.y+90)*PI_180)*movStep;
		d.z+=sin((a.y+90)*PI_180)*movStep;
	}
	
	if(inKey(31)) {
		d.x+=cos((a.y-90)*PI_180)*movStep;
		d.z+=sin((a.y-90)*PI_180)*movStep;
	}
	
	if(inKey(30)) {
		d.x+=cos(a.y*PI_180)*movStep;
		d.z+=sin(a.y*PI_180)*movStep;
	}
	
	if(inKey(32)) {
		d.x-=cos(a.y*PI_180)*movStep;
		d.z-=sin(a.y*PI_180)*movStep;
	}
	a.x-=(double)inMYinc/(double)5.0;
	a.y-=(double)inMXinc/(double)5.0;
}	
//////////////////////////////////////////////////////////////////////////////////////////////

void usrSceneDraw(void) {
	if(inKey(1)) mainEngineExit();
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glSetViewMatrix(a,d*0);
	glDisable(GL_DEPTH_TEST);
	gamSky.render();
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();
	glSetViewMatrix(a,d);

	gamLevel.render();
	glTranslated(pos.x,pos.y,pos.z);
	glRotated(alp,0,1,0);
	gamModel.render();
	gamModel1.render();
	glTranslated(0.145*METER,0.060*METER,-0.00*METER);
	gamWep[curWep].render();
	glTranslated(0,0,-0.6*METER);

	
	
	glBlendFunc(GL_SRC_COLOR,GL_ONE);
//	glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D,0);
	glDepthMask(false);
	
	glTranslated(0,0,-0.6*METER);
	modDrawBillboard(0.5*METER);

	glTranslated(0,0,0.6*METER);
	modDrawBillboard(0.5*METER);

	glDepthMask(true);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	conDraw();
}

#endif
