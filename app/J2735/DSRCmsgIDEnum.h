/*
 * Generated by asn1c-0.9.27 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735_WXW.ASN"
 */

#ifndef	_DSRCmsgIDEnum_H_
#define	_DSRCmsgIDEnum_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRCmsgIDEnum {
	DSRCmsgIDEnum_reservedMessageId_D	= 0,
	DSRCmsgIDEnum_alaCarteMessage_D	= 1,
	DSRCmsgIDEnum_basicSafetyMessage_D	= 2,
	DSRCmsgIDEnum_basicSafetyMessageVerbose_D	= 3,
	DSRCmsgIDEnum_commonSafetyRequest_D	= 4,
	DSRCmsgIDEnum_emergencyVehicleAlert_D	= 5,
	DSRCmsgIDEnum_intersectionCollision_D	= 6,
	DSRCmsgIDEnum_mapData_D	= 7,
	DSRCmsgIDEnum_nmeaCorrections_D	= 8,
	DSRCmsgIDEnum_probeDataManagement_D	= 9,
	DSRCmsgIDEnum_probeVehicleData_D	= 10,
	DSRCmsgIDEnum_roadSideAlert_D	= 11,
	DSRCmsgIDEnum_rtcmCorrections_D	= 12,
	DSRCmsgIDEnum_signalPhaseAndTimingMessage_D	= 13,
	DSRCmsgIDEnum_signalRequestMessage_D	= 14,
	DSRCmsgIDEnum_signalStatusMessage_D	= 15,
	DSRCmsgIDEnum_travelerInformation_D	= 16,
	DSRCmsgIDEnum_uperFrame_D	= 17,
	DSRCmsgIDEnum_mapData	= 18,
	DSRCmsgIDEnum_signalPhaseAndTimingMessage	= 19,
	DSRCmsgIDEnum_basicSafetyMessage	= 20,
	DSRCmsgIDEnum_commonSafetyRequest	= 21,
	DSRCmsgIDEnum_emergencyVehicleAlert	= 22,
	DSRCmsgIDEnum_intersectionCollision	= 23,
	DSRCmsgIDEnum_nmeaCorrections	= 24,
	DSRCmsgIDEnum_probeDataManagement	= 25,
	DSRCmsgIDEnum_probeVehicleData	= 26,
	DSRCmsgIDEnum_roadSideAlert	= 27,
	DSRCmsgIDEnum_rtcmCorrections	= 28,
	DSRCmsgIDEnum_signalRequestMessage	= 29,
	DSRCmsgIDEnum_signalStatusMessage	= 30,
	DSRCmsgIDEnum_travelerInformation	= 31,
	DSRCmsgIDEnum_personalSafetyMessage	= 32,
	DSRCmsgIDEnum_testMessage00	= 240,
	DSRCmsgIDEnum_testMessage01	= 241,
	DSRCmsgIDEnum_testMessage02	= 242,
	DSRCmsgIDEnum_testMessage03	= 243,
	DSRCmsgIDEnum_testMessage04	= 244,
	DSRCmsgIDEnum_testMessage05	= 245,
	DSRCmsgIDEnum_testMessage06	= 246,
	DSRCmsgIDEnum_testMessage07	= 247,
	DSRCmsgIDEnum_testMessage08	= 248,
	DSRCmsgIDEnum_testMessage09	= 249,
	DSRCmsgIDEnum_testMessage10	= 250,
	DSRCmsgIDEnum_testMessage11	= 251,
	DSRCmsgIDEnum_testMessage12	= 252,
	DSRCmsgIDEnum_testMessage13	= 253,
	DSRCmsgIDEnum_testMessage14	= 254,
	DSRCmsgIDEnum_testMessage15	= 255
} e_DSRCmsgIDEnum;

/* DSRCmsgIDEnum */
typedef long	 DSRCmsgIDEnum_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRCmsgIDEnum;
asn_struct_free_f DSRCmsgIDEnum_free;
asn_struct_print_f DSRCmsgIDEnum_print;
asn_constr_check_f DSRCmsgIDEnum_constraint;
ber_type_decoder_f DSRCmsgIDEnum_decode_ber;
der_type_encoder_f DSRCmsgIDEnum_encode_der;
xer_type_decoder_f DSRCmsgIDEnum_decode_xer;
xer_type_encoder_f DSRCmsgIDEnum_encode_xer;
per_type_decoder_f DSRCmsgIDEnum_decode_uper;
per_type_encoder_f DSRCmsgIDEnum_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRCmsgIDEnum_H_ */
#include <asn_internal.h>
