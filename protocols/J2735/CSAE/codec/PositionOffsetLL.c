/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 *     found in "DSRCCN_20170918.asn"
 *     `asn1c -no-gen-example -no-gen-OER`
 */

#include "PositionOffsetLL.h"

asn_per_constraints_t asn_PER_type_PositionOffsetLL_constr_1 CC_NOTUSED = {
    { APC_CONSTRAINED,     3,  3,  0,  6 }    /* (0..6) */,
    { APC_UNCONSTRAINED,    -1, -1,  0,  0 },
    0, 0    /* No PER value map */
};
asn_TYPE_member_t asn_MBR_PositionOffsetLL_1[] = {
    { ATF_NOFLAGS, 0, offsetof(struct PositionOffsetLL, choice.position_LL1),
        (ASN_TAG_CLASS_CONTEXT | (0 << 2)),
        -1,    /* IMPLICIT tag at current level */
        &asn_DEF_Position_LL_24B,
        0,
        { 0, 0, 0 },
        0, 0, /* No default value */
        "position-LL1"
        },
    { ATF_NOFLAGS, 0, offsetof(struct PositionOffsetLL, choice.position_LL2),
        (ASN_TAG_CLASS_CONTEXT | (1 << 2)),
        -1,    /* IMPLICIT tag at current level */
        &asn_DEF_Position_LL_28B,
        0,
        { 0, 0, 0 },
        0, 0, /* No default value */
        "position-LL2"
        },
    { ATF_NOFLAGS, 0, offsetof(struct PositionOffsetLL, choice.position_LL3),
        (ASN_TAG_CLASS_CONTEXT | (2 << 2)),
        -1,    /* IMPLICIT tag at current level */
        &asn_DEF_Position_LL_32B,
        0,
        { 0, 0, 0 },
        0, 0, /* No default value */
        "position-LL3"
        },
    { ATF_NOFLAGS, 0, offsetof(struct PositionOffsetLL, choice.position_LL4),
        (ASN_TAG_CLASS_CONTEXT | (3 << 2)),
        -1,    /* IMPLICIT tag at current level */
        &asn_DEF_Position_LL_36B,
        0,
        { 0, 0, 0 },
        0, 0, /* No default value */
        "position-LL4"
        },
    { ATF_NOFLAGS, 0, offsetof(struct PositionOffsetLL, choice.position_LL5),
        (ASN_TAG_CLASS_CONTEXT | (4 << 2)),
        -1,    /* IMPLICIT tag at current level */
        &asn_DEF_Position_LL_44B,
        0,
        { 0, 0, 0 },
        0, 0, /* No default value */
        "position-LL5"
        },
    { ATF_NOFLAGS, 0, offsetof(struct PositionOffsetLL, choice.position_LL6),
        (ASN_TAG_CLASS_CONTEXT | (5 << 2)),
        -1,    /* IMPLICIT tag at current level */
        &asn_DEF_Position_LL_48B,
        0,
        { 0, 0, 0 },
        0, 0, /* No default value */
        "position-LL6"
        },
    { ATF_NOFLAGS, 0, offsetof(struct PositionOffsetLL, choice.position_LatLon),
        (ASN_TAG_CLASS_CONTEXT | (6 << 2)),
        -1,    /* IMPLICIT tag at current level */
        &asn_DEF_Position_LLmD_64b,
        0,
        { 0, 0, 0 },
        0, 0, /* No default value */
        "position-LatLon"
        },
};
static const asn_TYPE_tag2member_t asn_MAP_PositionOffsetLL_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* position-LL1 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* position-LL2 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* position-LL3 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* position-LL4 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* position-LL5 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* position-LL6 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* position-LatLon */
};
asn_CHOICE_specifics_t asn_SPC_PositionOffsetLL_specs_1 = {
    sizeof(struct PositionOffsetLL),
    offsetof(struct PositionOffsetLL, _asn_ctx),
    offsetof(struct PositionOffsetLL, present),
    sizeof(((struct PositionOffsetLL *)0)->present),
    asn_MAP_PositionOffsetLL_tag2el_1,
    7,    /* Count of tags in the map */
    0, 0,
    -1    /* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_PositionOffsetLL = {
    "PositionOffsetLL",
    "PositionOffsetLL",
    &asn_OP_CHOICE,
    0,    /* No effective tags (pointer) */
    0,    /* No effective tags (count) */
    0,    /* No tags (pointer) */
    0,    /* No tags (count) */
    { 0, &asn_PER_type_PositionOffsetLL_constr_1, CHOICE_constraint },
    asn_MBR_PositionOffsetLL_1,
    7,    /* Elements count */
    &asn_SPC_PositionOffsetLL_specs_1    /* Additional specs */
};

