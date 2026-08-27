#ifndef NENGINE_WEAPONS
#define NENGINE_WEAPONS
#include "engModel.h"
#define	WPN_PISTOL	0x00
#define	WPN_RIFLE	0x01
#define	WPN_SNIPER	0x02
#define	WPN_RPG		0x03

class cWeapon
{
public:
	cModelStatic	mdl;
	char			name[32];
	unsigned int	type;
	void load(char *namef);
};
void cWeapon::load(char *namef)
{
	FILE *f = fopen(namef,"r");
	char buf[256];
	char tmp[256];

	fgets(buf,256,f);
	sscanf(buf,"WEAPON	%s",&name);
	conHistory("Weapon \"%s\"",name);
	fgets(buf,256,f);
	sscanf(buf,"MODEL	%s",&tmp);
	mdl.load(tmp);
	conHistory("Model \"%s\"",tmp);

	fgets(buf,256,f);
	sscanf(buf,"TYPE	%u",&type);
	conHistory("Type \"%u\"",type);

	fclose(f);

}
#endif
