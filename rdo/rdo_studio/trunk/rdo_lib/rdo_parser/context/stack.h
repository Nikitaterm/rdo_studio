/*!
  \copyright (c) RDO-Team, 2011
  \file      stack.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_STACK_H_
#define _RDOPARSER_CONTEXT_STACK_H_

// ====================================================================== INCLUDES
#include <list>
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"

#include "rdo_lib/rdo_parser/namespace.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextStack
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(Context);

OBJECT(ContextStack)
{
DECLARE_FACTORY(ContextStack);
public:
	void      push  (LPContext pContext);
	void      pop   ();

	LPContext top   () const;
	LPContext global() const;
	LPContext prev  (CREF(LPContext) pContext) const;

private:
	ContextStack();
	virtual ~ContextStack();

	typedef std::list<LPContext> Container;

	Container m_container;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_STACK_H_
