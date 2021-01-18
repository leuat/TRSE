/*************************************************************************************************/
/**
	stringutils.h


	Copyright (C) Rich Talbot-Watkins 2007 - 2012

	This file is part of BeebAsm.

	BeebAsm is free software: you can redistribute it and/or modify it under the terms of the GNU
	General Public License as published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	BeebAsm is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
	even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with BeebAsm, as
	COPYING.txt.  If not, see <http://www.gnu.org/licenses/>.
*/
/*************************************************************************************************/

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <string>


namespace StringUtils
{
	void ExpandTabsToSpaces( std::string& line, size_t tabWidth );
	bool EatWhitespace( const std::string& line, size_t& column );
}


#endif // STRINGUTILS_H_
