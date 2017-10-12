string	headerLayout();		/* header layout format string */
int	size();			/* size of full data in transport */
string	transport();		/* export full data as string */

private int type;		/* type of this network data */

/*
 * initialize network data
 */
static void create(int type)
{
    ::type = type;
}

int networkDataType()	{ return type; }
