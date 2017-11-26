# define Creature		object "/usr/Asheron/creature/lib/Creature"
# define Humanoid		object "/usr/Asheron/creature/lib/Humanoid"

# define CREATURE_SERVER	"/usr/Asheron/creature/sys/creatures"

# define ATTR_STRENGTH			1
# define ATTR_ENDURANCE			2
# define ATTR_QUICKNESS			3
# define ATTR_COORDINATION		4
# define ATTR_FOCUS			5
# define ATTR_SELF			6

# define VITAL_HEALTH			1
# define VITAL_STAMINA			2
# define VITAL_MANA			3

# define SKILL_STATINCR(stat, incr)	(((stat) << 16) + (incr))
# define SKILL_STAT(skill)		((skill) >> 16)
# define SKILL_INCR(skill)		((skill) & 0xffff)

# define SKILLSTAT_INACTIVE		0
# define SKILLSTAT_UNTRAINED		1
# define SKILLSTAT_TRAINED		2
# define SKILLSTAT_SPECIALIZED		3

# define SKILL_MELEE_DEFENSE		6
# define SKILL_MISSILE_DEFENSE		7
# define SKILL_ARCANE_LORE		14
# define SKILL_MAGIC_DEFENSE		15
# define SKILL_MANA_CONVERSION		16
# define SKILL_ITEM_TINKERING		18
# define SKILL_ASSESS_PERSON		19
# define SKILL_DECEPTION		20
# define SKILL_HEALING			21
# define SKILL_JUMP			22
# define SKILL_LOCKPICK			23
# define SKILL_RUN			24
# define SKILL_ASSESS_CREATURE		27
# define SKILL_WEAPON_TINKERING		28
# define SKILL_ARMOR_TINKERING		29
# define SKILL_MAGIC_ITEM_TINKERING	30
# define SKILL_CREATURE_ENCHANTMENT	31
# define SKILL_ITEM_ENCHANTMENT		32
# define SKILL_LIFE_MAGIC		33
# define SKILL_WAR_MAGIC		34
# define SKILL_LEADERSHIP		35
# define SKILL_LOYALTY			36
# define SKILL_FLETCHING		37
# define SKILL_ALCHEMY			38
# define SKILL_COOKING			39
# define SKILL_SALVAGING		40
# define SKILL_TWO_HANDED_COMBAT	41
# define SKILL_VOID_MAGIC		43
# define SKILL_HEAVY_WEAPONS		44
# define SKILL_LIGHT_WEAPONS		45
# define SKILL_FINESSE_WEAPONS		46
# define SKILL_MISSILE_WEAPONS		47
# define SKILL_SHIELD			48
# define SKILL_DUAL_WIELD		49
# define SKILL_RECKLESSNESS		50
# define SKILL_SNEAK_ATTACK		51
# define SKILL_DIRTY_FIGHTING		52
# define SKILL_SUMMONING		54
