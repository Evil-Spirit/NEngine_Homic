#ifndef NENGINE_CONSOLE
#define NENGINE_CONSOLE
#include <conio.h>
#include <string.h>
#include <stdarg.h>
#define			CON_MAX_HIST 47

bool			conAni = false;
bool			conTog = false;
bool			conRes = false;
double			conSize = 0.0f;
double			conMaxSize = 1.0f;
unsigned char	conTexture = 0;
char			**conHist;
char			*conCom;
FILE			*conFile;
void conHistory(const char *fmt,...)
{
	if (strlen(fmt)>41) return;
	char comm[256];
	va_list	ap;
	if (fmt == NULL) return;
	va_start(ap, fmt);
	vsprintf(comm, fmt, ap);
	fprintf(conFile,"%s\n",comm);
	va_end(ap);

	for (int i = CON_MAX_HIST-1;i>0;i--) 
		strcpy(conHist[i],conHist[i-1]);
	strcpy(conHist[0],comm);
	fflush(conFile);
}
void conInit(unsigned char tex, char *name)
{
	conTexture =  tex;
	conCom = new char[41];
	for (int i=0;i<41;i++) conCom[i]=0;
	conHist = new char*[CON_MAX_HIST];
	for (int i=0;i<CON_MAX_HIST;i++)
	{
		conHist[i] = new char[41];
		for (int j=0;j<41;j++) conHist[i][j] = 0;

	}
	conFile = fopen(name,"w");
	conHistory("****************************************");
	conHistory("*  nEngine, Copyright(C), Evil Spirit  *");
	conHistory("*  Builded %s, %s       *",__DATE__,__TIME__);
	conHistory("****************************************");
}
void conDone()
{
	conHistory("****************************************");
	fclose(logFile);
	conTexture =  NULL;
	delete []conCom; 
	for (int i=0;i<CON_MAX_HIST;i++)
		delete []conHist[i];
	delete []conHist;
}
void conToggle() {
	conAni = true;
}
void conResize() {
	conRes = true;
}
void conMomentToggle() {
	if (conTog)
	{
		conTog = false;
		conSize = 0;
	} else
	{
		conTog = true;
		conSize = conMaxSize;
	}
}
void conCommand(char *com)
{
	if (!strncmp("halt\0",com,5))
	{
		mainEngineExit();
		return;
	}
	if (!strncmp("fov ",com,4))
	{
		double f = engFov;
		sscanf(com,"fov %lf",&f);
		engFov = f;
	}
	if (!strncmp("fov\0",com,4))
	{
		conHistory("\"fov\" is \"%f\"",engFov);
	}
	if (!strncmp("consize ",com,8))
	{
		double f = conMaxSize;
		sscanf(com,"consize %lf",&f);
		if (f>2.0||f<2.0f/(double)engResY*(double)fntFonts[0].sy)
		{
			conHistory("\"consize\"");
			conHistory("min=\"%lf\"",2.0f/(double)engResY*(double)fntFonts[0].sy,2);
			conHistory("max=\"%lf\"",2.0);
		}
		else
		{
			conMaxSize = f;
			conResize();
		}
	}
	if (!strncmp("consize\0",com,8))
	{
		conHistory("\"consize\" is \"%f\"",conMaxSize);
	}
	if (!strncmp("fps\0",com,4))
	{
		conHistory(FPSString);
	}
}
void conDraw() {
	if (!conTog&&!conAni) return;
	glColor3d(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, conTexture);

	glBegin(GL_QUADS);
	glTexCoord2d(0,1-conSize);
	glVertex2d(-1,1);
	glTexCoord2d(0,1);
	glVertex2d(-1,1-conSize);
	glTexCoord2d(1,1);
	glVertex2d(+1,1-conSize);
	glTexCoord2d(1,1-conSize);
	glVertex2d(+1,1);
	glEnd();

	glColor3d(1,1,0);
//	textPrint2D(0,-1,1.9-conSize,1,FPSString);
	textPrint2D(0,-1,1-conSize,1,conCom);
	double sizy = 2.0f/(double)engResY*(double)fntFonts[0].sy;
	for (int i = 0;i<CON_MAX_HIST;i++)
	{
		double cy = 1-conSize+double(i+1)*sizy;
		if (cy>1.0) break;
		textPrint2D(0,-1,cy,1,conHist[i]);
	}
	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void conProcess()
{
	if (conAni) 
	{
		if (conTog)
		{
			if (conSize>0.0f) conSize -= 0.01f;
			if (conSize<=0.0f)
			{
				conSize = 0.0f;
				conTog = false;
				conAni = false;
			}
		} else
		{
			if (conSize<conMaxSize) conSize += 0.01f;
			if (conSize>=conMaxSize)
			{
				conSize = conMaxSize;
				conTog = true;
				conAni = false;
			}
		}
	}
	else
	if (conRes) 
	{
		if (conSize>conMaxSize)
		{
			conSize -= 0.01f;
			if (conSize<=conMaxSize)
			{
				conSize = conMaxSize;
				conRes = false;
			}
		} else
		if (conSize<conMaxSize)
		{
			conSize += 0.01f;
			if (conSize>=conMaxSize)
			{
				conSize = conMaxSize;
				conRes = false;
			}
		}
	}
}
void conKeyProcess(char key) {
	switch (key)
	{
		case 8:
			if (!conTog&&!conAni) return;
			conCom[strlen(conCom)-1] = 0;
			break;
			break;
		case 13:
			if (!conTog&&!conAni) return;
			conHistory(conCom);
			conCommand(conCom);
			conCom[0] = 0;
			conCom[1] = 0;
			break;
		case '`': 
			conToggle();
			break;
		case '~': 
			conToggle();
			break;
		default:
			if (!conTog&&!conAni) return;
			if (strlen(conCom)<40)	
			{
				conCom[strlen(conCom)] = key;
				conCom[strlen(conCom)+1] = 0;
			}
			break;
	}
}
#endif
