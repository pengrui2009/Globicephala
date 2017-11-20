/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 *     found in "DSRCCN_20170918.asn"
 *     `asn1c -no-gen-example -no-gen-OER`
 */

#ifndef    _PositionOffsetLL_H_
#define    _PositionOffsetLL_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Position-LL-24B.h"
#include "Position-LL-28B.h"
#include "Position-LL-32B.h"
#include "Position-LL-36B.h"
#include "Position-LL-44B.h"
#include "Position-LL-48B.h"
#include "Position-LLmD-64b.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PositionOffsetLL_PR {
    PositionOffsetLL_PR_NOTHING,    /* No components present */
    PositionOffsetLL_PR_position_LL1,
    PositionOffsetLL_PR_position_LL2,
    PositionOffsetLL_PR_position_LL3,
    PositionOffsetLL_PR_position_LL4,
    PositionOffsetLL_PR_position_LL5,
    PositionOffsetLL_PR_position_LL6,
    PositionOffsetLL_PR_position_LatLon
} PositionOffsetLL_PR;

/* PositionOffsetLL */
typedef struct PositionOffsetLL {
    PositionOffsetLL_PR present;
    union PositionOffsetLL_u {
        Position_LL_24B_t     position_LL1;
        Position_LL_28B_t     position_LL2;
        Position_LL_32B_t     position_LL3;
        Position_LL_36B_t     position_LL4;
        Position_LL_44B_t     position_LL5;
        Position_LL_48B_t     position_LL6;
        Position_LLmD_64b_t     position_LatLon;
    } choice;
    
    /* Context for parsing across buffer boundaries */
    asn_struct_ctx_t _asn_ctx;
} PositionOffsetLL_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PositionOffsetLL;
extern asn_CHOICE_specifics_t asn_SPC_PositionOffsetLL_specs_1;
extern asn_TYPE_member_t asn_MBR_PositionOffsetLL_1[7];
extern asn_per_constraints_t asn_PER_type_PositionOffsetLL_constr_1;

#ifdef __cplusplus
}
#endif

#endif    /* _PositionOffsetLL_H_ */
#include <asn_internal.h>
