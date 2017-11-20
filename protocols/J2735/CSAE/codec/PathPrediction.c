/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRCCN_20170918.asn"
 * 	`asn1c -no-gen-example -no-gen-OER`
 */

#include "PathPrediction.h"

asn_TYPE_member_t asn_MBR_PathPrediction_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PathPrediction, radiusOfCurve),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RadiusOfCurvature,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"radiusOfCurve"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PathPrediction, confidence),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Confidence,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"confidence"
		},
};
static const ber_tlv_tag_t asn_DEF_PathPrediction_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PathPrediction_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* radiusOfCurve */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* confidence */
};
asn_SEQUENCE_specifics_t asn_SPC_PathPrediction_specs_1 = {
	sizeof(struct PathPrediction),
	offsetof(struct PathPrediction, _asn_ctx),
	asn_MAP_PathPrediction_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PathPrediction = {
	"PathPrediction",
	"PathPrediction",
	&asn_OP_SEQUENCE,
	asn_DEF_PathPrediction_tags_1,
	sizeof(asn_DEF_PathPrediction_tags_1)
		/sizeof(asn_DEF_PathPrediction_tags_1[0]), /* 1 */
	asn_DEF_PathPrediction_tags_1,	/* Same as above */
	sizeof(asn_DEF_PathPrediction_tags_1)
		/sizeof(asn_DEF_PathPrediction_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PathPrediction_1,
	2,	/* Elements count */
	&asn_SPC_PathPrediction_specs_1	/* Additional specs */
};

