/*************************************************************************************************/
/**
	globaldata.h


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

#ifndef GLOBALDATA_H_
#define GLOBALDATA_H_

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <string>


class DiscImage;

class GlobalData
{
public:

	static void Create();
	static void Destroy();
	static inline GlobalData& Instance() { assert( m_gInstance != NULL ); return *m_gInstance; }

	inline void SetPass( int i )				{ m_pass = i; }
	inline void SetBootFile( const char* p )	{ m_pBootFile = p; }
	inline void SetVerbose( bool b )			{ m_bVerbose = b; }
	inline void SetUseDiscImage( bool b )		{ m_bUseDiscImage = b; }
	inline void SetDiscImage( DiscImage* d )	{ m_pDiscImage = d; }
	inline void ResetForId()					{ m_forId = 0; }
	inline void SetSaved()						{ m_bSaved = true; }
	inline void SetOutputFile( const char* p )	{ m_pOutputFile = p; }
	inline void IncNumAnonSaves()				{ m_numAnonSaves++; }
	inline void SetDiscOption( int opt )		{ m_discOption = opt; }
	inline void SetDiscTitle( const std::string& t )  
												{ m_discTitle = t; }
	inline void SetRequireDistinctOpcodes ( bool b )
												{ m_bRequireDistinctOpcodes = b; }
	inline void SetUseVisualCppErrorFormat( bool b )
												{ m_bUseVisualCppErrorFormat = b; }

	inline int GetPass() const					{ return m_pass; }
	inline bool IsFirstPass() const				{ return ( m_pass == 0 ); }
	inline bool IsSecondPass() const			{ return ( m_pass == 1 ); }
	inline bool ShouldOutputAsm() const			{ return ( m_pass == 1 && m_bVerbose ); }
	inline const char* GetBootFile() const		{ return m_pBootFile; }
	inline bool UsesDiscImage() const			{ return m_bUseDiscImage; }
	inline DiscImage* GetDiscImage() const		{ return m_pDiscImage; }
	inline int GetNextForId()					{ return m_forId++; }
	inline bool IsSaved() const					{ return m_bSaved; }
	inline const char* GetOutputFile() const	{ return m_pOutputFile; }
	inline int GetNumAnonSaves() const			{ return m_numAnonSaves; }
	inline int GetDiscOption() const			{ return m_discOption; }
	inline const std::string& GetDiscTitle() const
												{ return m_discTitle; }
	inline time_t GetAssemblyTime() const		{ return m_assemblyTime; }
	inline bool RequireDistinctOpcodes() const  { return m_bRequireDistinctOpcodes; }
	inline bool UseVisualCppErrorFormat() const { return m_bUseVisualCppErrorFormat; }

private:

	GlobalData();
	~GlobalData();

	static GlobalData*			m_gInstance;

	int							m_pass;
	const char*					m_pBootFile;
	bool						m_bVerbose;
	bool						m_bUseDiscImage;
	DiscImage*					m_pDiscImage;
	int							m_forId;
	bool						m_bSaved;
	const char*					m_pOutputFile;
	int							m_numAnonSaves;
	int							m_discOption;
	std::string					m_discTitle;
	time_t						m_assemblyTime;
	bool						m_bRequireDistinctOpcodes;
	bool						m_bUseVisualCppErrorFormat;
};



#endif // GLOBALDATA_H_
