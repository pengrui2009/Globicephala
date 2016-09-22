/*
 * Generated by asn1c-0.9.27 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735_WXW.ASN"
 */

#include "BSMpartIIExtension.h"

static asn_per_constraints_t asn_PER_type_BSMpartIIExtension_constr_1 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 6,  6,  0,  52 }	/* (0..52) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_BSMpartIIExtension_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleSafetyExt),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleSafetyExtensions,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleSafetyExt"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.specialVehicleExt),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SpecialVehicleExtensions,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"specialVehicleExt"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt1),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt1"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt2),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt2"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt3),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt3"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt4),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt4"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt5),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt5"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt6),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt6"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt7),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt7"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt8),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt8"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt9),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt9"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt10),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt10"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt11),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt11"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt12),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt12"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt13),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt13"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt14),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt14"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt15),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt15"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt16),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt16"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt17),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt17"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt18),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt18"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt19),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt19"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt20),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt20"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt21),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt21"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt22),
		(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt22"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt23),
		(ASN_TAG_CLASS_CONTEXT | (24 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt23"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt24),
		(ASN_TAG_CLASS_CONTEXT | (25 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt24"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt25),
		(ASN_TAG_CLASS_CONTEXT | (26 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt25"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt26),
		(ASN_TAG_CLASS_CONTEXT | (27 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt26"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt27),
		(ASN_TAG_CLASS_CONTEXT | (28 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt27"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt28),
		(ASN_TAG_CLASS_CONTEXT | (29 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt28"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt29),
		(ASN_TAG_CLASS_CONTEXT | (30 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt29"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt30),
		(ASN_TAG_CLASS_CONTEXT | (31 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt30"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt31),
		(ASN_TAG_CLASS_CONTEXT | (32 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt31"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt32),
		(ASN_TAG_CLASS_CONTEXT | (33 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt32"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt33),
		(ASN_TAG_CLASS_CONTEXT | (34 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt33"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt34),
		(ASN_TAG_CLASS_CONTEXT | (35 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt34"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt35),
		(ASN_TAG_CLASS_CONTEXT | (36 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt35"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt36),
		(ASN_TAG_CLASS_CONTEXT | (37 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt36"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt37),
		(ASN_TAG_CLASS_CONTEXT | (38 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt37"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt38),
		(ASN_TAG_CLASS_CONTEXT | (39 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt38"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt39),
		(ASN_TAG_CLASS_CONTEXT | (40 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt39"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt40),
		(ASN_TAG_CLASS_CONTEXT | (41 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt40"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt51),
		(ASN_TAG_CLASS_CONTEXT | (42 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt51"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt52),
		(ASN_TAG_CLASS_CONTEXT | (43 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt52"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt53),
		(ASN_TAG_CLASS_CONTEXT | (44 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt53"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt54),
		(ASN_TAG_CLASS_CONTEXT | (45 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt54"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt55),
		(ASN_TAG_CLASS_CONTEXT | (46 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt55"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt56),
		(ASN_TAG_CLASS_CONTEXT | (47 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt56"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt57),
		(ASN_TAG_CLASS_CONTEXT | (48 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt57"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt58),
		(ASN_TAG_CLASS_CONTEXT | (49 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt58"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt59),
		(ASN_TAG_CLASS_CONTEXT | (50 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt59"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt60),
		(ASN_TAG_CLASS_CONTEXT | (51 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt60"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BSMpartIIExtension, choice.vehicleDummySt61),
		(ASN_TAG_CLASS_CONTEXT | (52 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleDummyStruct,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vehicleDummySt61"
		},
};
static asn_TYPE_tag2member_t asn_MAP_BSMpartIIExtension_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* vehicleSafetyExt */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* specialVehicleExt */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* vehicleDummySt1 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* vehicleDummySt2 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* vehicleDummySt3 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* vehicleDummySt4 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* vehicleDummySt5 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* vehicleDummySt6 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* vehicleDummySt7 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* vehicleDummySt8 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* vehicleDummySt9 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* vehicleDummySt10 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* vehicleDummySt11 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* vehicleDummySt12 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* vehicleDummySt13 */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* vehicleDummySt14 */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* vehicleDummySt15 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* vehicleDummySt16 */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* vehicleDummySt17 */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 }, /* vehicleDummySt18 */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 20, 0, 0 }, /* vehicleDummySt19 */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 21, 0, 0 }, /* vehicleDummySt20 */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 22, 0, 0 }, /* vehicleDummySt21 */
    { (ASN_TAG_CLASS_CONTEXT | (23 << 2)), 23, 0, 0 }, /* vehicleDummySt22 */
    { (ASN_TAG_CLASS_CONTEXT | (24 << 2)), 24, 0, 0 }, /* vehicleDummySt23 */
    { (ASN_TAG_CLASS_CONTEXT | (25 << 2)), 25, 0, 0 }, /* vehicleDummySt24 */
    { (ASN_TAG_CLASS_CONTEXT | (26 << 2)), 26, 0, 0 }, /* vehicleDummySt25 */
    { (ASN_TAG_CLASS_CONTEXT | (27 << 2)), 27, 0, 0 }, /* vehicleDummySt26 */
    { (ASN_TAG_CLASS_CONTEXT | (28 << 2)), 28, 0, 0 }, /* vehicleDummySt27 */
    { (ASN_TAG_CLASS_CONTEXT | (29 << 2)), 29, 0, 0 }, /* vehicleDummySt28 */
    { (ASN_TAG_CLASS_CONTEXT | (30 << 2)), 30, 0, 0 }, /* vehicleDummySt29 */
    { (ASN_TAG_CLASS_CONTEXT | (31 << 2)), 31, 0, 0 }, /* vehicleDummySt30 */
    { (ASN_TAG_CLASS_CONTEXT | (32 << 2)), 32, 0, 0 }, /* vehicleDummySt31 */
    { (ASN_TAG_CLASS_CONTEXT | (33 << 2)), 33, 0, 0 }, /* vehicleDummySt32 */
    { (ASN_TAG_CLASS_CONTEXT | (34 << 2)), 34, 0, 0 }, /* vehicleDummySt33 */
    { (ASN_TAG_CLASS_CONTEXT | (35 << 2)), 35, 0, 0 }, /* vehicleDummySt34 */
    { (ASN_TAG_CLASS_CONTEXT | (36 << 2)), 36, 0, 0 }, /* vehicleDummySt35 */
    { (ASN_TAG_CLASS_CONTEXT | (37 << 2)), 37, 0, 0 }, /* vehicleDummySt36 */
    { (ASN_TAG_CLASS_CONTEXT | (38 << 2)), 38, 0, 0 }, /* vehicleDummySt37 */
    { (ASN_TAG_CLASS_CONTEXT | (39 << 2)), 39, 0, 0 }, /* vehicleDummySt38 */
    { (ASN_TAG_CLASS_CONTEXT | (40 << 2)), 40, 0, 0 }, /* vehicleDummySt39 */
    { (ASN_TAG_CLASS_CONTEXT | (41 << 2)), 41, 0, 0 }, /* vehicleDummySt40 */
    { (ASN_TAG_CLASS_CONTEXT | (42 << 2)), 42, 0, 0 }, /* vehicleDummySt51 */
    { (ASN_TAG_CLASS_CONTEXT | (43 << 2)), 43, 0, 0 }, /* vehicleDummySt52 */
    { (ASN_TAG_CLASS_CONTEXT | (44 << 2)), 44, 0, 0 }, /* vehicleDummySt53 */
    { (ASN_TAG_CLASS_CONTEXT | (45 << 2)), 45, 0, 0 }, /* vehicleDummySt54 */
    { (ASN_TAG_CLASS_CONTEXT | (46 << 2)), 46, 0, 0 }, /* vehicleDummySt55 */
    { (ASN_TAG_CLASS_CONTEXT | (47 << 2)), 47, 0, 0 }, /* vehicleDummySt56 */
    { (ASN_TAG_CLASS_CONTEXT | (48 << 2)), 48, 0, 0 }, /* vehicleDummySt57 */
    { (ASN_TAG_CLASS_CONTEXT | (49 << 2)), 49, 0, 0 }, /* vehicleDummySt58 */
    { (ASN_TAG_CLASS_CONTEXT | (50 << 2)), 50, 0, 0 }, /* vehicleDummySt59 */
    { (ASN_TAG_CLASS_CONTEXT | (51 << 2)), 51, 0, 0 }, /* vehicleDummySt60 */
    { (ASN_TAG_CLASS_CONTEXT | (52 << 2)), 52, 0, 0 } /* vehicleDummySt61 */
};
static asn_CHOICE_specifics_t asn_SPC_BSMpartIIExtension_specs_1 = {
	sizeof(struct BSMpartIIExtension),
	offsetof(struct BSMpartIIExtension, _asn_ctx),
	offsetof(struct BSMpartIIExtension, present),
	sizeof(((struct BSMpartIIExtension *)0)->present),
	asn_MAP_BSMpartIIExtension_tag2el_1,
	53,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_BSMpartIIExtension = {
	"BSMpartIIExtension",
	"BSMpartIIExtension",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&asn_PER_type_BSMpartIIExtension_constr_1,
	asn_MBR_BSMpartIIExtension_1,
	53,	/* Elements count */
	&asn_SPC_BSMpartIIExtension_specs_1	/* Additional specs */
};

