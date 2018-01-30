# define Object			object "/usr/Asheron/lib/Object"
# define ObjDesc		object "/usr/Asheron/lib/ObjDesc"
# define PhysicalObject		object "/usr/Asheron/lib/PhysicalObject"
# define GameObject		object "/usr/Asheron/lib/GameObject"
# define Container		object "/usr/Asheron/lib/Container"

# define OBJECT_ID_TYPE_MASK		0x03
# define OBJECT_ID_TYPE_PERSISTENT	0x00
# define OBJECT_ID_TYPE_CONTAINED	0x01
# define OBJECT_ID_TYPE_ENVIRONMENTAL	0x02

# define OBJECT_ID_PERSISTENT_MASK	0xfffff000
# define OBJECT_ID_PERSISTENT_SHIFT	12
# define OBJECT_ID_LOCAL_MASK		0x00000ffc
# define OBJECT_ID_LOCAL_SHIFT		4
