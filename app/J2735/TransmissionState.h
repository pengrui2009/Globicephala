/*
 * Generated by asn1c-0.9.27 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735_WXW.ASN"
 */

#ifndef	_TransmissionState_H_
#define	_TransmissionState_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TransmissionState {
	TransmissionState_neutral	= 0,
	TransmissionState_park	= 1,
	TransmissionState_forwardGears	= 2,
	TransmissionState_reverseGears	= 3,
	TransmissionState_reserved1	= 4,
	TransmissionState_reserved2	= 5,
	TransmissionState_reserved3	= 6,
	TransmissionState_unavailable	= 7
} e_TransmissionState;

/* TransmissionState */
typedef long	 TransmissionState_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TransmissionState;
asn_struct_free_f TransmissionState_free;
asn_struct_print_f TransmissionState_print;
asn_constr_check_f TransmissionState_constraint;
ber_type_decoder_f TransmissionState_decode_ber;
der_type_encoder_f TransmissionState_encode_der;
xer_type_decoder_f TransmissionState_decode_xer;
xer_type_encoder_f TransmissionState_encode_xer;
per_type_decoder_f TransmissionState_decode_uper;
per_type_encoder_f TransmissionState_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _TransmissionState_H_ */
#include <asn_internal.h>
