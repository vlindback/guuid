/*
	Copyright (C) 2019 Viktor Lindbäck

	This software is licensed under the European Union Public License (EUPL)
	The license is available in the 22 official languages of the EU.

	The English version is included here.
	Please see https://joinup.ec.europa.eu/community/eupl/og_page/eupl for official translations of the other languages.

	This license applies to the whole project.

*/


#pragma once

#include "guuid.h"

#include <stdexcept>
#include <memory>


class guuid
{
public:

	using value_type = guuid_t;

	guuid()
		: m_guuid(guuid_create())
	{
		constexpr const value_type zero_uuid = {};
		if (std::memcmp(&m_guuid, &zero_uuid, sizeof(value_type)) == 0) {
			throw std::runtime_error("Failed to generate UUID!");
			/* TODO: platform specific system_error? */
		}
	}

private:

	value_type m_guuid;

};