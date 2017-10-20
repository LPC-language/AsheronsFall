# define DatImage		object "/usr/Asheron/dat/obj/DatImage"
# define DatItem		object "/usr/Asheron/dat/lib/DatItem"
# define DatReader		object "/usr/Asheron/dat/lib/DatReader"

# define CharGen		object "/usr/Asheron/dat/lib/CharGen"
# define DatPosition		object "/usr/Asheron/dat/lib/record/Position"
# define DatHeritage		object "/usr/Asheron/dat/lib/record/Heritage"
# define DatTemplate		object "/usr/Asheron/dat/lib/record/Template"
# define DatAppearance		object "/usr/Asheron/dat/lib/record/Appearance"
# define DatObjDesc		object "/usr/Asheron/dat/lib/record/ObjDesc"
# define DatHairStyle		object "/usr/Asheron/dat/lib/record/HairStyle"
# define DatEyeShape		object "/usr/Asheron/dat/lib/record/EyeShape"
# define DatFeature		object "/usr/Asheron/dat/lib/record/Feature"
# define DatGarment		object "/usr/Asheron/dat/lib/record/Garment"


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
