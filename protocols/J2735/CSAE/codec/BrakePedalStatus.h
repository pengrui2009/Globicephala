/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 *     found in "DSRCCN_20170918.asn"
 *     `asn1c -no-gen-example -no-gen-OER`
 */

#ifndef    _BrakePedalStatus_H_
#define    _BrakePedalStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BrakePedalStatus {
    BrakePedalStatus_unavailable    = 0,
    BrakePedalStatus_off    = 1,
    BrakePedalStatus_on    = 2
} e_BrakePedalStatus;

/* BrakePedalStatus */
typedef long     BrakePedalStatus_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_BrakePedalStatus_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_BrakePedalStatus;
extern const asn_INTEGER_specifics_t asn_SPC_BrakePedalStatus_specs_1;
asn_struct_free_f BrakePedalStatus_free;
asn_struct_print_f BrakePedalStatus_print;
asn_constr_check_f BrakePedalStatus_constraint;
ber_type_decoder_f BrakePedalStatus_decode_ber;
der_type_encoder_f BrakePedalStatus_encode_der;
xer_type_decoder_f BrakePedalStatus_decode_xer;
xer_type_encoder_f BrakePedalStatus_encode_xer;
per_type_decoder_f BrakePedalStatus_decode_uper;
per_type_encoder_f BrakePedalStatus_encode_uper;

#ifdef __cplusplus
}
#endif

#endif    /* _BrakePedalStatus_H_ */
#include <asn_internal.h>
