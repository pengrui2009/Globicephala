/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRCCN_20170918.asn"
 * 	`asn1c -no-gen-example -no-gen-OER`
 */

#ifndef	_PathHistory_H_
#define	_PathHistory_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GNSSstatus.h"
#include "PathHistoryPointList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct FullPositionVector;

/* PathHistory */
typedef struct PathHistory {
	struct FullPositionVector	*initialPosition	/* OPTIONAL */;
	GNSSstatus_t	*currGNSSstatus	/* OPTIONAL */;
	PathHistoryPointList_t	 crumbData;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PathHistory_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PathHistory;
extern asn_SEQUENCE_specifics_t asn_SPC_PathHistory_specs_1;
extern asn_TYPE_member_t asn_MBR_PathHistory_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "FullPositionVector.h"

#endif	/* _PathHistory_H_ */
#include <asn_internal.h>
