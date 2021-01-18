/*************************************************************************************************/
/**
	asmexception.h


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

#ifndef ASMEXCEPTION_H_
#define ASMEXCEPTION_H_


#include <string>
#include <vector>


/*************************************************************************************************/
/**
	@class		AsmException

	Base class for Asm6502 exceptions
*/
/*************************************************************************************************/
class AsmException
{
public:

	AsmException() {}
	virtual ~AsmException() {}

	virtual void Print() const = 0;
};


/*************************************************************************************************/
/**
	@class		AsmException_FileError

	Exception class used for all I/O errors
*/
/*************************************************************************************************/
class AsmException_FileError : public AsmException
{
public:

	explicit AsmException_FileError( const std::string& filename )
		:	m_filename( filename )
	{
	}

	virtual ~AsmException_FileError() {}

	virtual void Print() const;

	virtual const char* Message() const
	{
		return "Unspecified file error.";
	}

protected:

	std::string			m_filename;
};


#define DEFINE_FILE_EXCEPTION( a, msg )										\
class AsmException_FileError_##a : public AsmException_FileError			\
{																			\
public:																		\
	explicit AsmException_FileError_##a( const std::string filename )			\
		:	AsmException_FileError( filename ) {}							\
																			\
	virtual ~AsmException_FileError_##a() {}								\
																			\
	virtual const char* Message() const { return msg; }						\
}


DEFINE_FILE_EXCEPTION( OpenSourceFile, "Could not open source file for reading." );
DEFINE_FILE_EXCEPTION( ReadSourceFile, "Problem reading from source file." );
DEFINE_FILE_EXCEPTION( OpenDiscSource, "Could not open disc image for reading." );
DEFINE_FILE_EXCEPTION( ReadDiscSource, "Problem reading from disc image." );
DEFINE_FILE_EXCEPTION( OpenDiscDest, "Could not create new disc image." );
DEFINE_FILE_EXCEPTION( WriteDiscDest, "Could not write to disc image." );
DEFINE_FILE_EXCEPTION( OpenObj, "Could not open object file for writing." );
DEFINE_FILE_EXCEPTION( WriteObj, "Problem writing to object file." );
DEFINE_FILE_EXCEPTION( DiscFull, "No room on DFS disc image full." );
DEFINE_FILE_EXCEPTION( BadName, "Bad DFS filename." );
DEFINE_FILE_EXCEPTION( TooManyFiles, "Too many files on DFS disc image (max 31)." );
DEFINE_FILE_EXCEPTION( FileExists, "File already exists on DFS disc image." );


/*************************************************************************************************/
/**
	@class		AsmException_SyntaxError

	Base exception class used for all syntax errors
*/
/*************************************************************************************************/
class AsmException_SyntaxError : public AsmException
{
public:

	AsmException_SyntaxError()
		:	m_column( 0 )
	{
	}

	AsmException_SyntaxError( std::string line, int column )
		:	m_line( line ),
			m_column( column )
	{
	}

	AsmException_SyntaxError( std::string line, int column, std::string extra )
		:	m_line( line ),
			m_column( column ),
			m_extra( extra )
	{
	}

	virtual ~AsmException_SyntaxError() {}

	void SetFilename( const std::string& filename )	{ m_filename.push_back( filename ); }
	void SetLineNumber( int lineNumber )		{ m_lineNumber.push_back( lineNumber ); }

	virtual void Print() const;
	virtual const char* Message() const
	{
		return "Unspecified syntax error.";
	}


protected:

	std::string	ErrorLocation( size_t i ) const;

	std::string			m_line;
	int				m_column;
	std::string			m_extra;
	std::vector<std::string>	m_filename;
	std::vector<int>                m_lineNumber;
};


#define DEFINE_SYNTAX_EXCEPTION( a, msg )									\
class AsmException_SyntaxError_##a : public AsmException_SyntaxError		\
{																			\
public:																		\
	AsmException_SyntaxError_##a( std::string line, int column )			\
		:	AsmException_SyntaxError( line, column ) {}						\
																			\
	virtual ~AsmException_SyntaxError_##a() {}								\
																			\
	virtual const char* Message() const { return msg; }						\
}


#define DEFINE_SYNTAX_EXCEPTION_EXTRA( a, msg )								\
class AsmException_SyntaxError_##a : public AsmException_SyntaxError		\
{																			\
public:																		\
	AsmException_SyntaxError_##a( std::string line, int column, std::string extra )			\
		:	AsmException_SyntaxError( line, column, extra ) {}				\
																			\
	virtual ~AsmException_SyntaxError_##a() {}								\
																			\
	virtual const char* Message() const { return msg; }						\
}


// high-level file parsing exceptions
DEFINE_SYNTAX_EXCEPTION( UnrecognisedToken, "Unrecognised token." );

// expression parsing exceptions
DEFINE_SYNTAX_EXCEPTION( NumberTooBig, "Number too big." );
DEFINE_SYNTAX_EXCEPTION( SymbolNotDefined, "Symbol not defined." );
DEFINE_SYNTAX_EXCEPTION( BadHex, "Bad hex." );
DEFINE_SYNTAX_EXCEPTION( BadBin, "Bad binary expression." );
DEFINE_SYNTAX_EXCEPTION( MissingValue, "Missing value in expression." );
DEFINE_SYNTAX_EXCEPTION( InvalidCharacter, "Bad expression." );
DEFINE_SYNTAX_EXCEPTION( ExpressionTooComplex, "Expression too complex." );
DEFINE_SYNTAX_EXCEPTION( MismatchedParentheses, "Mismatched parentheses." );
DEFINE_SYNTAX_EXCEPTION( EmptyExpression, "Expression not found." );
DEFINE_SYNTAX_EXCEPTION( DivisionByZero, "Division by zero." );
DEFINE_SYNTAX_EXCEPTION( MissingQuote, "Unterminated string." );
DEFINE_SYNTAX_EXCEPTION( MissingComma, "Missing comma." );
DEFINE_SYNTAX_EXCEPTION( IllegalOperation, "Operation attempted with invalid or out of range values." );
DEFINE_SYNTAX_EXCEPTION( TimeResultTooBig, "TIME$() result too big." );

// assembler parsing exceptions
DEFINE_SYNTAX_EXCEPTION( NoImplied, "Implied mode not allowed for this instruction." );
DEFINE_SYNTAX_EXCEPTION( ImmTooLarge, "Immediate constants cannot be greater than 255." );
DEFINE_SYNTAX_EXCEPTION( ImmNegative, "Constant cannot be negative." );
DEFINE_SYNTAX_EXCEPTION( UnexpectedComma, "Unexpected comma enountered." );
DEFINE_SYNTAX_EXCEPTION( NoImmediate, "Immediate mode not allowed for this instruction." );
DEFINE_SYNTAX_EXCEPTION( NoIndirect, "Indirect mode not allowed for this instruction." );
DEFINE_SYNTAX_EXCEPTION( 6502Bug, "JMP (addr) will not execute as intended due to the 6502 bug (addr = &xxFF)." );
DEFINE_SYNTAX_EXCEPTION( BadIndirect, "Incorrectly formed indirect instruction." );
DEFINE_SYNTAX_EXCEPTION( NotZeroPage, "Address is not in zero-page." );
DEFINE_SYNTAX_EXCEPTION_EXTRA( BranchOutOfRange, "Branch out of range." );
DEFINE_SYNTAX_EXCEPTION( NoAbsolute, "Absolute addressing mode not allowed for this instruction." );
DEFINE_SYNTAX_EXCEPTION( BadAbsolute, "Syntax error in absolute instruction." );
DEFINE_SYNTAX_EXCEPTION( BadAddress, "Out of range address." );
DEFINE_SYNTAX_EXCEPTION( BadIndexed, "Syntax error in indexed instruction." );
DEFINE_SYNTAX_EXCEPTION( NoIndexedX, "X indexed mode does not exist for this instruction." );
DEFINE_SYNTAX_EXCEPTION( NoIndexedY, "Y indexed mode does not exist for this instruction." );
DEFINE_SYNTAX_EXCEPTION( LabelAlreadyDefined, "Symbol already defined." );
DEFINE_SYNTAX_EXCEPTION( InvalidSymbolName, "Invalid symbol name; must start with a letter and contain only letters, numbers and underscore." );
DEFINE_SYNTAX_EXCEPTION( SymbolScopeOutsideMacro, "Symbol scope cannot promote outside current macro." );
DEFINE_SYNTAX_EXCEPTION( SymbolScopeOutsideFor, "Symbol scope cannot promote outside current FOR loop." );
DEFINE_SYNTAX_EXCEPTION( SecondPassProblem, "Fatal error: the second assembler pass has generated different code to the first." );
DEFINE_SYNTAX_EXCEPTION( InvalidMacroName, "Invalid macro name; must start with a letter and contain only letters, numbers and underscore." );
DEFINE_SYNTAX_EXCEPTION( NoNestedMacros, "Cannot define one macro inside another." );
DEFINE_SYNTAX_EXCEPTION( EndMacroUnexpected, "ENDMACRO encountered without a matching MACRO directive." );
DEFINE_SYNTAX_EXCEPTION( NoEndMacro, "Unterminated macro (ENDMACRO not found)." );
DEFINE_SYNTAX_EXCEPTION( DuplicateMacroName, "Macro name already defined." );
DEFINE_SYNTAX_EXCEPTION( AssertionFailed, "Assertion failed." );

// meta-language parsing exceptions
DEFINE_SYNTAX_EXCEPTION( NextWithoutFor, "NEXT without FOR." );
DEFINE_SYNTAX_EXCEPTION( ForWithoutNext, "FOR without NEXT." );
DEFINE_SYNTAX_EXCEPTION( BadStep, "Step value cannot be zero." );
DEFINE_SYNTAX_EXCEPTION( TooManyFORs, "Too many nested FORs or braces." );
DEFINE_SYNTAX_EXCEPTION( MismatchedBraces, "Mismatched braces." );
DEFINE_SYNTAX_EXCEPTION( CantInclude, "Cannot include a source file within a FOR loop or braced block." );
DEFINE_SYNTAX_EXCEPTION( ElseWithoutIf, "ELSE without IF." );
DEFINE_SYNTAX_EXCEPTION( ElifWithoutIf, "ELIF without IF." );
DEFINE_SYNTAX_EXCEPTION( EndifWithoutIf, "ENDIF without IF." );
DEFINE_SYNTAX_EXCEPTION( IfWithoutEndif, "IF without ENDIF." );
DEFINE_SYNTAX_EXCEPTION( TooManyIFs, "Too many nested IFs." );
DEFINE_SYNTAX_EXCEPTION( BadAlignment, "Bad alignment." );
DEFINE_SYNTAX_EXCEPTION( OutOfRange, "Out of range." );
DEFINE_SYNTAX_EXCEPTION( BackwardsSkip, "Attempted to skip backwards to an address." );
DEFINE_SYNTAX_EXCEPTION( NoAnonSave, "Cannot specify SAVE without a filename if no default output filename has been specified." );
DEFINE_SYNTAX_EXCEPTION( OnlyOneAnonSave, "Can only use SAVE without a filename once per project." );



/*************************************************************************************************/
/**
	@class		AsmException_AssembleError

	Base exception class used for all assembling errors
*/
/*************************************************************************************************/
class AsmException_AssembleError : public AsmException_SyntaxError
{
public:

	AsmException_AssembleError() {}

	virtual ~AsmException_AssembleError() {}

	void SetString( std::string line )			{ m_line = line; }
	void SetColumn( int column )				{ m_column = column; }

	virtual const char* Message() const
	{
		return "Unspecified assemble error.";
	}
};


#define DEFINE_ASSEMBLE_EXCEPTION( a, msg )									\
class AsmException_AssembleError_##a : public AsmException_AssembleError	\
{																			\
public:																		\
	AsmException_AssembleError_##a() {}										\
	virtual ~AsmException_AssembleError_##a() {}							\
	virtual const char* Message() const { return msg; }						\
}


DEFINE_ASSEMBLE_EXCEPTION( OutOfMemory, "Out of memory." );
DEFINE_ASSEMBLE_EXCEPTION( GuardHit, "Guard point hit." );
DEFINE_ASSEMBLE_EXCEPTION( Overlap, "Trying to assemble over existing code." );
DEFINE_ASSEMBLE_EXCEPTION( InconsistentCode, "Assembled object code has changed between 1st and 2nd pass. Has a zero-page symbol been forward-declared?" );
DEFINE_ASSEMBLE_EXCEPTION( FileOpen, "Error opening file." );
DEFINE_ASSEMBLE_EXCEPTION( FileRead, "Error reading file." );


/*************************************************************************************************/
/**
	@class		AsmException_UserError

	Exception class used for user generated errors
*/
/*************************************************************************************************/
class AsmException_UserError : public AsmException_SyntaxError
{
public:

	AsmException_UserError( std::string line, int column, std::string message )
		:	AsmException_SyntaxError( line, column ),
			m_message( message )
	{
	}

	virtual const char* Message() const
	{
		return m_message.c_str();
	}


protected:

	std::string		m_message;
};



#endif // ASMEXCEPTION_H_
