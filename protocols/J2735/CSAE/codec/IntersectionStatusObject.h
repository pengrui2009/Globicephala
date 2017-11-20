/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRCCN_20170918.asn"
 * 	`asn1c -no-gen-example -no-gen-OER`
 */

#ifndef	_IntersectionStatusObject_H_
#define	_IntersectionStatusObject_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum IntersectionStatusObject {
	IntersectionStatusObject_manualControlIsEnabled	= 0,
	IntersectionStatusObject_stopTimeIsActivated	= 1,
	IntersectionStatusObject_failureFlash	= 2,
	IntersectionStatusObject_preemptIsActive	= 3,
	IntersectionStatusObject_signalPriorityIsActive	= 4,
	IntersectionStatusObject_fixedTimeOperation	= 5,
	IntersectionStatusObject_trafficDependentOperation	= 6,
	IntersectionStatusObject_standbyOperation	= 7,
	IntersectionStatusObject_failureMode	= 8,
	IntersectionStatusObject_off	= 9,
	IntersectionStatusObject_recentMAPmessageUpdate	= 10,
	IntersectionStatusObject_recentChangeInMAPassignedLanesIDsUsed	= 11,
	IntersectionStatusObject_noValidMAPisAvailableAtThisTime	= 12,
	IntersectionStatusObject_noValidSPATisAvailableAtThisTime	= 13
} e_IntersectionStatusObject;

/* IntersectionStatusObject */
typedef BIT_STRING_t	 IntersectionStatusObject_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_IntersectionStatusObject_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_IntersectionStatusObject;
asn_struct_free_f IntersectionStatusObject_free;
asn_struct_print_f IntersectionStatusObject_print;
asn_constr_check_f IntersectionStatusObject_constraint;
ber_type_decoder_f IntersectionStatusObject_decode_ber;
der_type_encoder_f IntersectionStatusObject_encode_der;
xer_type_decoder_f IntersectionStatusObject_decode_xer;
xer_type_encoder_f IntersectionStatusObject_encode_xer;
per_type_decoder_f IntersectionStatusObject_decode_uper;
per_type_encoder_f IntersectionStatusObject_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _IntersectionStatusObject_H_ */
#include <asn_internal.h>
