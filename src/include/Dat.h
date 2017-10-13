# define DatImage		object "/usr/Asheron/dat/obj/DatImage"
# define DatItem		object "/usr/Asheron/dat/lib/DatItem"

# define DAT_ITERATION		0xffff0001

# define DAT_CELL_X(id)		(((id) >> 24) & 0xff)
# define DAT_CELL_Y(id)		(((id) >> 16) & 0xff)
# define DAT_CELL_MASK		0x0000ffff
# define DAT_CELL_LANDBLOCK	0x0000ffff
# define DAT_CELL_LANDINFO	0x0000fffe

# define DAT_PORTAL_TYPE(id)	(((id) >> 24) & 0xff)
# define DAT_PORTAL_MASK	0x00ffffff