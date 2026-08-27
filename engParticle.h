#ifndef NENGINE_PARATICLE_SYSTEMS
#define NENGINE_PARATICLE_SYSTEMS
#include "engVector.h"
struct cPar
{
	tVector pos;
	tVector col;
	bool	live;
};
class cParSys
{
	cPar	*pars;
public:
	cParSys();
	~cParSys();
};
cParSys::cParSys()
{
	MessageBox(NULL,"OK, I am INITIALIZED!","OK",MB_OK);
}
cParSys::~cParSys()
{
	MessageBox(NULL,"OK, I am DE_INITIALIZED!","OK",MB_OK);
}
#endif
