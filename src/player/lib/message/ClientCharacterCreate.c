# include "Message.h"
# include "User.h"

inherit Message;


# define STYLE		0
# define COLOUR		1
# define HUE		2

private string accountName;	/* name of associated account */
private string name;		/* proposed name of character */

private int sex;		/* male 1, female 0 */
private int eyes;
private int nose;
private int mouth;
private mixed *body;		/* heritage, eye colour, skin hue */
private mixed *hair;		/* style, colour, hue */
private mixed *headgear;	/* style, colour, hue */
private mixed *shirt;		/* style, colour, hue */
private mixed *trousers;	/* style, colour, hue */
private mixed *footwear;	/* style, colour, hue */

private int template;		/* 0 = custom */
private int *attributes;	/* 330 points total */
private int slot;
private int classId;
private int *skills;		/* unusable, untrained, trained, specialized */

private int startArea;
private int admin;		/* wannabe admin */
private int envoy;		/* wannabe envoy */
private int points;		/* skill points? */

/*
 * CharacterCreate message
 */
static void create(string blob)
{
    int offset, one, numSkills;

    ::create(MSG_CHARACTER_CREATE);
    body = allocate(3);
    hair = allocate(3);
    headgear = allocate(3);
    shirt = allocate(3);
    trousers = allocate(3);
    footwear = allocate(3);
    attributes = allocate_int(6);
    skills = allocate_int(55);

    ({
	offset,

	accountName,
	one,
	body[STYLE],
	sex,
	eyes,
	nose,
	mouth,
	hair[COLOUR],
	body[COLOUR],
	hair[STYLE],
	headgear[STYLE],
	headgear[COLOUR],
	shirt[STYLE],
	shirt[COLOUR],
	trousers[STYLE],
	trousers[COLOUR],
	footwear[STYLE],
	footwear[COLOUR],
	body[HUE],
	hair[HUE],
	headgear[HUE],
	shirt[HUE],
	trousers[HUE],
	footwear[HUE],

	template,
	attributes[0], attributes[1], attributes[2],
	attributes[3], attributes[4], attributes[5],
	slot,
	classId,

	numSkills,
	skills[ 0], skills[ 1], skills[ 2], skills[ 3], skills[ 4],
	skills[ 5], skills[ 6], skills[ 7], skills[ 8], skills[ 9],
	skills[10], skills[11], skills[12], skills[13], skills[14],
	skills[15], skills[16], skills[17], skills[18], skills[19],
	skills[20], skills[21], skills[22], skills[23], skills[24],
	skills[25], skills[26], skills[27], skills[28], skills[29],
	skills[30], skills[31], skills[32], skills[33], skills[34],
	skills[35], skills[36], skills[37], skills[38], skills[39],
	skills[40], skills[41], skills[42], skills[43], skills[44],
	skills[45], skills[46], skills[47], skills[48], skills[49],
	skills[50], skills[51], skills[52], skills[53], skills[54],

	name,
	startArea,
	admin,
	envoy,
	points
    }) = deSerialize(blob, 4,
		     "tiiiiiiiiiiiiiiiiidddddd" +
		     "iiiiiiiii" +
		     "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"+
		     "tiiii");
    checkEmpty(blob, offset);
}


string accountName()		{ return accountName; }
string name()			{ return name; }
mixed *bodyStyleColourHue()	{ return body; }
mixed *headgearStyleColourHue()	{ return headgear; }
mixed *shirtStyleColourHue()	{ return shirt; }
mixed *trousersStyleColourHue()	{ return trousers; }
mixed *footwearStyleColourHue()	{ return footwear; }
int template()			{ return template; }
int *attributes()		{ return attributes; }
int slot()			{ return slot; }
int classId()			{ return classId; }
int *skills()			{ return skills; }
int startArea()			{ return startArea; }
int admin()			{ return admin; }
int envoy()			{ return envoy; }
int points()			{ return points; }
