/*
	Copyright (C) 2019 Viktor Lindbäck

	This software is licensed under the European Union Public License (EUPL)
	The license is available in the 22 official languages of the EU.

	The English version is included here.
	Please see https://joinup.ec.europa.eu/community/eupl/og_page/eupl for official translations of the other languages.

	This license applies to the whole project.

*/

#include "guuid.h"

#if defined __linux__

#include <stdio.h>

guuid_t guuid_create()
{
	/* Open a file to /dev/urandom and read enough bytes (16) to make up a GUID/UUID. */
	guuid_t guid = { 0 };
	FILE* dev_urandom_fp = fopen("dev/urandom", "w");
	if (dev_urandom_fp != NULL) {
		const size_t bytes_read = fread(&guid, sizeof(guid), 1, &dev_urandom_fp);
		if (bytes_read != 16) {
			/* error */
		}
		fclose(dev_urandom_fp);
	}
	return guid;
}

bool guuid_create_n(guuid_t* buffer_ptr, size_t buffer_count)
{
	/* OK, so here we try to do everything in 1 read then read bit for bit until success. */
	bool result = true;
	FILE* dev_urandom_fp = fopen("dev/urandom", "r");
	if (dev_urandom_fp != NULL) {

		const size_t total_bytes = sizeof(guuid_t) * buffer_count;
		size_t read_result = fread(buffer_ptr, total_bytes, 1, &dev_urandom_fp);
		
		if (read_result != total_bytes) {
			if (read_result == -1) {
				result = false;
			} else {
				size_t remaining_bytes = total_bytes - read_result;
				char* read_ptr = buffer_ptr;
				read_ptr += read_result;

				while (remaining_bytes != 0) {
					read_result = fread(read_ptr, remaining_bytes, 1, &dev_urandom_fp);
					if (read_result == -1) {
						result = false;
						break;
					}
				}
			}
		}

		fclose(dev_urandom_fp);
	} else {
		result = false;
	}

	return result;
}

#elif defined _WIN32

#include <Windows.h>
#include <bcrypt.h>
#include <ntstatus.h>

guuid_t guuid_create(void)
{
	/* To use a specific algorithm available in Windows we have to use BCryptOpenProvider etc... */
	guuid_t guuid = { 0 };

	/*	Return value is ignored here, because if it fails we just return a zero guuid.
		Heavily assuming this function doesn't screw up midway and leaves us with corrupt data.
	*/
	BCryptGenRandom(NULL, &guuid, sizeof(guuid), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	return guuid;
}

bool guuid_create_n(guuid_t* buffer_ptr, size_t buffer_count)
{
	const size_t total_bytes = sizeof(guuid_t) * buffer_count;
	return BCryptGenRandom(NULL, buffer_ptr, total_bytes, BCRYPT_USE_SYSTEM_PREFERRED_RNG) == STATUS_SUCCESS;
}


#endif

#include <memory.h>
#include <assert.h>


/* Cross platform implementations */

bool guuid_valid(const guuid_t* ptr)
{
	assert(ptr);
	guuid_t zero = { 0 };
	return memcmp(ptr, &zero, sizeof(zero)) != 0;
}

void guuid_to_text(const guuid_t* guuid, guuid_string_t* string_output)
{
	/* Assuming no invalid pointers */
	assert(guuid);
	assert(string_output);

	/* 8-4-4-4-12 */

	memcpy(string_output->str, guuid->data, sizeof(char) * 8);
	string_output->str[8] = '-';
	memcpy(string_output->str + 9, guuid->data + 8, sizeof(char) * 4);
	string_output->str[13] = '-';
	memcpy(string_output->str + 14, guuid->data + 12, sizeof(char) * 4);
	string_output->str[18] = '-';
}