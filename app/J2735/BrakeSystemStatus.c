/*
 * Generated by asn1c-0.9.27 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735_WXW.ASN"
 */

#include "BrakeSystemStatus.h"

static asn_TYPE_member_t asn_MBR_BrakeSystemStatus_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BrakeSystemStatus, wheelBrakes),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BrakeAppliedStatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"wheelBrakes"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BrakeSystemStatus, traction),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TractionControlStatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"traction"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BrakeSystemStatus, abs),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AntiLockBrakeStatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"abs"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BrakeSystemStatus, scs),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_StabilityControlStatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"scs"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BrakeSystemStatus, brakeBoost),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BrakeBoostApplied,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"brakeBoost"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BrakeSystemStatus, auxBrakes),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AuxiliaryBrakeStatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"auxBrakes"
		},
};
static ber_tlv_tag_t asn_DEF_BrakeSystemStatus_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_BrakeSystemStatus_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* wheelBrakes */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* traction */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* abs */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* scs */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* brakeBoost */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* auxBrakes */
};
static asn_SEQUENCE_specifics_t asn_SPC_BrakeSystemStatus_specs_1 = {
	sizeof(struct BrakeSystemStatus),
	offsetof(struct BrakeSystemStatus, _asn_ctx),
	asn_MAP_BrakeSystemStatus_tag2el_1,
	6,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_BrakeSystemStatus = {
	"BrakeSystemStatus",
	"BrakeSystemStatus",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_BrakeSystemStatus_tags_1,
	sizeof(asn_DEF_BrakeSystemStatus_tags_1)
		/sizeof(asn_DEF_BrakeSystemStatus_tags_1[0]), /* 1 */
	asn_DEF_BrakeSystemStatus_tags_1,	/* Same as above */
	sizeof(asn_DEF_BrakeSystemStatus_tags_1)
		/sizeof(asn_DEF_BrakeSystemStatus_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_BrakeSystemStatus_1,
	6,	/* Elements count */
	&asn_SPC_BrakeSystemStatus_specs_1	/* Additional specs */
};

