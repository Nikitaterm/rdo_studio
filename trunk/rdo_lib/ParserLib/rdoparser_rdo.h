#ifndef RDOPARSER_RDO_H
#define RDOPARSER_RDO_H

#include "rdoparser_base.h"
#include "rdoStdFuncs.h"
#include <iostream.h>

namespace rdoParse
{

class RDOFlexLexer;

// ----------------------------------------------------------------------------
// ---------- RDOParserRDO
// ----------------------------------------------------------------------------
class RDOParserRDO: public RDOParserBase
{
protected:
	RDOFlexLexer*        m_lexer;
	std::list< YYLTYPE > m_loc_list;

public:
	RDOParserRDO( rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun ): RDOParserBase( _type, _parser_fun, _error_fun, _lexer_fun ), m_lexer( NULL ) {};
	virtual ~RDOParserRDO();

	virtual void parse();
	virtual void parse( std::istream& in_stream );

	virtual void lexer_setvalue( int value );
	virtual void lexer_loc_init();
	virtual void lexer_loc_action();
	virtual void lexer_loc_push( bool erase = false );
	virtual void lexer_loc_push( void* data, bool erase = false );
	virtual void lexer_loc_pop();
	virtual int  lexer_loc_lineno();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserBase
{
public:
	RDOParserRSSPost(): RDOParserBase( rdoModelObjects::RSS, NULL, NULL, NULL ) {};

	virtual void parse();
	virtual void parse( std::istream& in_stream ) { parse(); }
};

// ----------------------------------------------------------------------------
// ---------- RDOParserFUN
// ----------------------------------------------------------------------------
class RDOParserFUN: public RDOParserRDO
{
public:
	RDOParserFUN( t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun ): RDOParserRDO( rdoModelObjects::FUN, _parser_fun, _error_fun, _lexer_fun ) {};

	virtual void parse( std::istream& in_stream );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSMR1
// ----------------------------------------------------------------------------
struct RDOSMR1OkException: public RDOException
{
	virtual std::string getType() const { return ""; }
	RDOSMR1OkException( const char* str ): RDOException( str ) {};
};

class RDOParserSMR1: public RDOParserRDO
{
public:
	RDOParserSMR1( t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun ): RDOParserRDO( rdoModelObjects::SMR, _parser_fun, _error_fun, _lexer_fun ) {};

	virtual void parse( std::istream& in_stream );
};

} // namespace rdoParse

#endif // RDOPARSER_RDO_H
