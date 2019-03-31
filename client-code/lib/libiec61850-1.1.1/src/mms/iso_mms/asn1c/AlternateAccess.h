/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "MMS"
 * 	found in "../mms-extended.asn"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_AlternateAccess_H_
#define	_AlternateAccess_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "Identifier.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AlternateAccess__Member_PR {
	AlternateAccess__Member_PR_NOTHING,	/* No components present */
	AlternateAccess__Member_PR_unnamed,
	AlternateAccess__Member_PR_named
} AlternateAccess__Member_PR;

/* Forward declarations */
struct AlternateAccessSelection;

struct AlternateAccess__Member {
		AlternateAccess__Member_PR present;
		union AlternateAccess__Member_u {
			struct AlternateAccessSelection	*unnamed;
			struct named {
				Identifier_t	 componentName;
				struct AlternateAccessSelection	*access;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} named;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	};

/* AlternateAccess */
typedef struct AlternateAccess {
	A_SEQUENCE_OF(struct AlternateAccess__Member) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AlternateAccess_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AlternateAccess;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AlternateAccessSelection.h"

#endif	/* _AlternateAccess_H_ */
