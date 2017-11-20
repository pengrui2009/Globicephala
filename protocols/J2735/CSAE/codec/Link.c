/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRCCN_20170918.asn"
 * 	`asn1c -no-gen-example -no-gen-OER`
 */

#include "Link.h"

asn_TYPE_member_t asn_MBR_Link_1[] = {
	{ ATF_POINTER, 1, offsetof(struct Link, name),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DescriptiveName,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"name"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Link, upstreamNodeId),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NodeReferenceID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"upstreamNodeId"
		},
	{ ATF_POINTER, 1, offsetof(struct Link, speedLimits),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SpeedLimitList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"speedLimits"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Link, laneWidth),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LaneWidth,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"laneWidth"
		},
	{ ATF_POINTER, 2, offsetof(struct Link, points),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PointList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"points"
		},
	{ ATF_POINTER, 1, offsetof(struct Link, movements),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MovementList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"movements"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Link, lanes),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LaneList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"lanes"
		},
};
static const int asn_MAP_Link_oms_1[] = { 0, 2, 4, 5 };
static const ber_tlv_tag_t asn_DEF_Link_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Link_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* name */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* upstreamNodeId */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* speedLimits */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* laneWidth */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* points */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* movements */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* lanes */
};
asn_SEQUENCE_specifics_t asn_SPC_Link_specs_1 = {
	sizeof(struct Link),
	offsetof(struct Link, _asn_ctx),
	asn_MAP_Link_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_Link_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	7,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Link = {
	"Link",
	"Link",
	&asn_OP_SEQUENCE,
	asn_DEF_Link_tags_1,
	sizeof(asn_DEF_Link_tags_1)
		/sizeof(asn_DEF_Link_tags_1[0]), /* 1 */
	asn_DEF_Link_tags_1,	/* Same as above */
	sizeof(asn_DEF_Link_tags_1)
		/sizeof(asn_DEF_Link_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Link_1,
	7,	/* Elements count */
	&asn_SPC_Link_specs_1	/* Additional specs */
};

