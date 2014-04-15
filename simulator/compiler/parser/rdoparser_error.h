/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_error.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.02.2010
  \brief     основан на rdoparser.h/cpp
  \indent    4T
*/

#ifndef _RDOPARSER_ERROR_H_
#define _RDOPARSER_ERROR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/rdo_exception.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSyntaxException
// --------------------------------------------------------------------------------
class RDOSyntaxException: public rdo::runtime::RDOException
{
public:
	RDOSyntaxException(CREF(std::string) message)
	   : RDOException(message)
	{}
	virtual std::string getType() const { return "RDO Syntax Error"; }
};

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
class Error
{
public:
	typedef std::vector<rdo::simulation::report::FileMessage> ErrorList;

	Error();

	//! 1
	void error(CREF(RDOParserSrcInfo) src_info, CREF(std::string) message);
	void warning(CREF(RDOParserSrcInfo) src_info, CREF(std::string) message);
	void push_only(CREF(RDOParserSrcInfo) src_info, CREF(std::string) message);

	//! 2
	void error(CREF(RDOParserSrcInfo) src_info1, CREF(RDOParserSrcInfo) src_info2, CREF(std::string) message);

	//! misc
	void push_done();
	void modify(CREF(std::string) message);
	void clear();
	CREF(ErrorList) getList () const;
	void block();
	void unblock();
	bool blocked() const;

private:
	ErrorList m_errorList;
	bool m_blocked;
};

Error& g_error();

class ErrorBlockMonicker
{
public:
	ErrorBlockMonicker()
	{
		g_error().block();
	}
	~ErrorBlockMonicker()
	{
		g_error().unblock();
	}
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_ERROR_H_
