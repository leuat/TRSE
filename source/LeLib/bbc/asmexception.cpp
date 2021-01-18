/*************************************************************************************************/
/**
	asmexception.cpp

	Exception handling for the app


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

#include <iostream>
#include <cassert>
#include <sstream>

#include "asmexception.h"
#include "globaldata.h"
#include "stringutils.h"

using namespace std;



/*************************************************************************************************/
/**
	AsmException_FileAccessError::Print()

	Outputs to stderr an error message relating to an I/O exception
*/
/*************************************************************************************************/
void AsmException_FileError::Print() const
{
	cerr << "Error: " << m_filename << ": " << Message() << endl;
}



/*************************************************************************************************/
/**
	AsmException_SyntaxError::Print()

	Outputs to stderr an error message regarding a syntax error
*/
/*************************************************************************************************/
void AsmException_SyntaxError::Print() const
{
	assert( !m_filename.empty() );
	assert( !m_lineNumber.empty() );
	assert( m_filename.size() == m_lineNumber.size() ) ;

	cerr << ErrorLocation(0);
	cerr << ": error: ";
	cerr << Message() << m_extra << endl << endl;
	cerr << m_line << endl;
	cerr << string( m_column, ' ' ) << "^" << endl;

	if ( m_filename.size() > 1 )
	{
		cerr << endl;
		cerr << "Call stack:" << endl;
		for (size_t i = 1; i < m_filename.size(); i++)
		{
			cerr << ErrorLocation(i) << endl;
		}
	}
}



/*************************************************************************************************/
/**
	AsmException_SyntaxError::ErrorLocation()

	Formats filename and line number as a string in the appropriate format
*/
/*************************************************************************************************/
std::string AsmException_SyntaxError::ErrorLocation( size_t i ) const
{
	std::stringstream s;
	if ( GlobalData::Instance().UseVisualCppErrorFormat() )
	{
		s << m_filename[ i ] << "(" << m_lineNumber[ i ] << ")";
	}
	else
	{
		s << m_filename[ i ] << ":" << m_lineNumber[ i ];
	}
	return s.str();
}
