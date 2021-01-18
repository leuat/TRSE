/*************************************************************************************************/
/**
	globaldata.cpp


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

#include "globaldata.h"
#include <iostream>

GlobalData* GlobalData::m_gInstance = NULL;



/*************************************************************************************************/
/**
	GlobalData::Create()

	Creates the GlobalData singleton
*/
/*************************************************************************************************/
void GlobalData::Create()
{
	assert( m_gInstance == NULL );

	m_gInstance = new GlobalData;
}



/*************************************************************************************************/
/**
	GlobalData::Destroy()

	Destroys the GlobalData singleton
*/
/*************************************************************************************************/
void GlobalData::Destroy()
{
	assert( m_gInstance != NULL );

	delete m_gInstance;
	m_gInstance = NULL;
}



/*************************************************************************************************/
/**
	GlobalData::GlobalData()

	GlobalData constructor
*/
/*************************************************************************************************/
GlobalData::GlobalData()
	:	m_pBootFile( NULL ),
		m_bVerbose( false ),
		m_bUseDiscImage( false ),
		m_pDiscImage( NULL ),
		m_bSaved( false ),
		m_pOutputFile( NULL ),
		m_numAnonSaves( 0 ),
		m_discOption( 0 ),
		m_assemblyTime( time( NULL ) ),
		m_bRequireDistinctOpcodes( false ),
		m_bUseVisualCppErrorFormat( false )
{
	// We populate m_assemblyTime with a time on startup so that all uses of TIME$ during 
	// assembly refer to the exact same time, however long we spend assembling.
	if ( m_assemblyTime == static_cast< time_t >( -1 ) )
	{
		std::cerr << "Unable to determine current time" << std::endl;
		exit( EXIT_FAILURE );
	}
}



/*************************************************************************************************/
/**
	GlobalData::~GlobalData()

	GlobalData destructor
*/
/*************************************************************************************************/
GlobalData::~GlobalData()
{
}
