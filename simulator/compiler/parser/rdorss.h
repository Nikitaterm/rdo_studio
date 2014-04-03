/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Романов Ярослав (robot.xet@gmail.com)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDORSS_RSS_H_
#define _RDORSS_RSS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
CLASS(RDORSSResource):
	    INSTANCE_OF      (RDOParserSrcInfo  )
	AND INSTANCE_OF      (boost::noncopyable)
	AND INSTANCE_OF      (Context           )
	AND IMPLEMENTATION_OF(IContextFind      )
{
DECLARE_FACTORY(RDORSSResource);
public:
	static const std::string GET_RESOURCE;

	class Param
	{
	public:
		explicit Param(CREF(LPRDOValue) pValue)
			: m_pValue(pValue)
		{}

		CREF(LPRDOValue) param() const
		{
			return m_pValue;
		}

		REF(LPRDOValue) param()
		{
			return m_pValue;
		}

	private:
		//! \todo использовать RDOCalc вместо RDOValue
		LPRDOValue m_pValue;
	};
	typedef std::vector<Param> ParamList;
	static const ruint UNDEFINED_ID = ruint(~0);

	virtual std::vector<rdo::runtime::LPRDOCalc> createCalc() const;

	CREF(tstring)    name   () const { return src_info().src_text(); }
	LPRDORTPResType  getType() const { return m_pResType;            }

	ruint            getID  () const { return m_id;                  }

	CREF(ParamList)  params () const { return m_paramList;           }

	void  addParam(CREF(LPRDOValue) pParam);
	rbool getTrace() const      { return trace;  }
	void  setTrace(rbool value) { trace = value; }
	rbool defined () const;
	void  end     ();

	void writeModelStructure(std::ostream& stream) const;

	LPExpression  createGetResourceExpression(const RDOParserSrcInfo& srcInfo) const;

	void setTraceCalc (const rdo::runtime::LPRDOCalc& pTraceCalc)
	{
		m_traceCalc = pTraceCalc;
	}

protected:
	RDORSSResource(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, ruint id = UNDEFINED_ID);
	virtual ~RDORSSResource();

	LPRDORTPResType m_pResType;
	ruint           m_id;        //! in system
	ParamList       m_paramList;
	rbool           trace;

private:
	RDORTPResType::ParamList::const_iterator m_currParam;
	rdo::runtime::LPRDOCalc m_traceCalc;

	virtual Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDORSSResource);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDORSS_RSS_H_
