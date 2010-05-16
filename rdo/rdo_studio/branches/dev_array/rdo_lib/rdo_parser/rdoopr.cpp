#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdoopr.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_runtime/rdo_logic_opr.h"

namespace rdoParse 
{

int oprlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void oprerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
RDOOPROperation::RDOOPROperation( LPIBaseOperationContainer dpt, const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( dpt, parent, _src_info, _pattern_src_info )
{
	parser()->insertOPROperation( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
RDOOperations::RDOOperations( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOLogicActivity<rdoRuntime::RDOOperations, RDOOPROperation>( _parser, _src_info )
{
	m_rt_logic = F(rdoRuntime::RDOOperations)::create(parser()->runtime());
	m_rt_logic->init(parser()->runtime());
	parser()->insertOperations( this );
}

} // namespace rdoParse 
