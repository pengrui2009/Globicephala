/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 *     found in "DSRCCN_20170918.asn"
 *     `asn1c -no-gen-example -no-gen-OER`
 */

#ifndef    _Connection_H_
#define    _Connection_H_


#include <asn_application.h>

/* Including external dependencies */
#include "NodeReferenceID.h"
#include "PhaseID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ConnectingLane;

/* Connection */
typedef struct Connection {
    NodeReferenceID_t     remoteIntersection;
    struct ConnectingLane    *connectingLane    /* OPTIONAL */;
    PhaseID_t    *phaseId    /* OPTIONAL */;
    
    /* Context for parsing across buffer boundaries */
    asn_struct_ctx_t _asn_ctx;
} Connection_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Connection;
extern asn_SEQUENCE_specifics_t asn_SPC_Connection_specs_1;
extern asn_TYPE_member_t asn_MBR_Connection_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ConnectingLane.h"

#endif    /* _Connection_H_ */
#include <asn_internal.h>
