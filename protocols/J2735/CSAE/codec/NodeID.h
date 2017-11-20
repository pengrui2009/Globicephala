/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRCCN_20170918.asn"
 * 	`asn1c -no-gen-example -no-gen-OER`
 */

#ifndef	_NodeID_H_
#define	_NodeID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NodeID */
typedef long	 NodeID_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_NodeID_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_NodeID;
asn_struct_free_f NodeID_free;
asn_struct_print_f NodeID_print;
asn_constr_check_f NodeID_constraint;
ber_type_decoder_f NodeID_decode_ber;
der_type_encoder_f NodeID_encode_der;
xer_type_decoder_f NodeID_decode_xer;
xer_type_encoder_f NodeID_encode_xer;
per_type_decoder_f NodeID_decode_uper;
per_type_encoder_f NodeID_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _NodeID_H_ */
#include <asn_internal.h>