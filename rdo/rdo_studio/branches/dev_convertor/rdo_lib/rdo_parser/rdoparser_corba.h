/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_corba.h
 * author   : ������� ������, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CORBA_H_
#define _RDOPARSER_CORBA_H_

#ifdef CORBA_ENABLE

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_rdo.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRTP);
private:
	RDOParserCorbaRTP()
		: RDOParserRDOItem(rdoModelObjects::RTP, NULL, NULL, NULL)
	{}
	virtual void parse(PTR(RDOParser) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRSS);
private:
	RDOParserCorbaRSS()
		: RDOParserRDOItem(rdoModelObjects::RSS, NULL, NULL, NULL)
	{}
	virtual void parse(PTR(RDOParser) pParser);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! CORBA_ENABLE

#endif //! _RDOPARSER_CORBA_H_
