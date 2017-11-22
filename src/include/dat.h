# define DatImage		object "/usr/Asheron/dat/obj/DatImage"
# define DatItem		object "/usr/Asheron/dat/lib/DatItem"
# define DatReader		object "/usr/Asheron/dat/lib/DatReader"

# define CharGen		object "/usr/Asheron/dat/obj/CharGen"
# define DatPosition		object "/usr/Asheron/dat/lib/Position"
# define DatHeritage		object "/usr/Asheron/dat/lib/Heritage"
# define DatTemplate		object "/usr/Asheron/dat/lib/Template"
# define DatAppearance		object "/usr/Asheron/dat/lib/Appearance"
# define DatObjDesc		object "/usr/Asheron/dat/lib/ObjDesc"
# define DatHairStyle		object "/usr/Asheron/dat/lib/HairStyle"
# define DatEyeShape		object "/usr/Asheron/dat/lib/EyeShape"
# define DatFeature		object "/usr/Asheron/dat/lib/Feature"
# define DatGarment		object "/usr/Asheron/dat/lib/Garment"
# define XpTable		object "/usr/Asheron/dat/obj/XpTable"


# define DAT_SERVER		"/usr/Asheron/dat/sys/datserver"

# define DAT_ITERATION		0xffff0001

# define DAT_CELL_X(id)		(((id) >> 24) & 0xff)
# define DAT_CELL_Y(id)		(((id) >> 16) & 0xff)
# define DAT_CELL_MASK		0x0000ffff
# define DAT_CELL_LANDBLOCK	0x0000ffff
# define DAT_CELL_LANDINFO	0x0000fffe

# define DAT_PORTAL_TYPE(id)	(((id) >> 24) & 0xff)
# define DAT_PORTAL_MASK	0x00ffffff

# define DAT_CHARGEN		0x0e000002
# define DAT_XPTABLE		0x0e000018
