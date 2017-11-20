/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 *     found in "DSRCCN_20170918.asn"
 *     `asn1c -no-gen-example -no-gen-OER`
 */

#ifndef    _BrakeSystemStatus_H_
#define    _BrakeSystemStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include "BrakePedalStatus.h"
#include "BrakeAppliedStatus.h"
#include "TractionControlStatus.h"
#include "AntiLockBrakeStatus.h"
#include "StabilityControlStatus.h"
#include "BrakeBoostApplied.h"
#include "AuxiliaryBrakeStatus.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BrakeSystemStatus */
typedef struct BrakeSystemStatus {
    BrakePedalStatus_t    *brakePadel    /* OPTIONAL */;
    BrakeAppliedStatus_t    *wheelBrakes    /* OPTIONAL */;
    TractionControlStatus_t    *traction    /* OPTIONAL */;
    AntiLockBrakeStatus_t    *abs    /* OPTIONAL */;
    StabilityControlStatus_t    *scs    /* OPTIONAL */;
    BrakeBoostApplied_t    *brakeBoost    /* OPTIONAL */;
    AuxiliaryBrakeStatus_t    *auxBrakes    /* OPTIONAL */;
    
    /* Context for parsing across buffer boundaries */
    asn_struct_ctx_t _asn_ctx;
} BrakeSystemStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BrakeSystemStatus;
extern asn_SEQUENCE_specifics_t asn_SPC_BrakeSystemStatus_specs_1;
extern asn_TYPE_member_t asn_MBR_BrakeSystemStatus_1[7];

#ifdef __cplusplus
}
#endif

#endif    /* _BrakeSystemStatus_H_ */
#include <asn_internal.h>
