/*
	Copyright (C) 2019 Viktor Lindbäck

	This software is licensed under the European Union Public License (EUPL)
	The license is available in the 22 official languages of the EU.

	The English version is included here.
	Please see https://joinup.ec.europa.eu/community/eupl/og_page/eupl for official translations of the other languages.

	This license applies to the whole project.

*/

#pragma once

#ifndef C_GUUID_H
#define C_GUUID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* In its canonical textual representation, the 16 octets of a UUID are represented as 32 hexadecimal (base-16) digits,
 *   displayed in 5 groups separated by hyphens, in the form 8-4-4-4-12 for a total of 36 characters 
 *  (32 alphanumeric characters and 4 hyphens).
 *  
 *  For example:
 *  123e4567-e89b-12d3-a456-426655440000
 *  xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
*/

	typedef struct guuid
	{
		uint8_t data[16]; /* 128 bits */
	} guuid_t;


	guuid_t guuid_create(void);
	bool	guuid_create_n(guuid_t* buffer_ptr, size_t buffer_count);
	bool	guuid_valid(const guuid_t* ptr);

	/* GUUID to text */

	typedef struct guuid_string
	{
		char str[38]; /* 37 + 1 for null terminator */
	} guuid_string_t;

	void guuid_to_text(const guuid_t* guuid, guuid_string_t* string_output);


#ifdef __cplusplus
}
#endif

#endif

