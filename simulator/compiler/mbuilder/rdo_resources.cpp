/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resources.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/mbuilder/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/rdo_resources.h"

#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/type/range.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResType
// --------------------------------------------------------------------------------
// ---- Инициализация типа ресурса по существующему в памяти
// ---- Собирает все параметры существующего в памяти типа ресурса
// --------------------------------------------------------------------------------
RDOResType::RDOResType(CREF(parser::LPRDORTPResType) rtp)
	: parser::RDOParserSrcInfo(rtp->src_info()                )
	, m_name (rtp->name()                                     )
	, m_exist(true                                            )
	, m_type (rtp->isPermanent() ? rt_permanent : rt_temporary)
	, m_id   (rtp->getNumber()                                )
{
	STL_FOR_ALL_CONST(rtp->getParams(), param_it)
	{
		Param param(*param_it);
		param.m_id = m_params.size();
		m_params.append(param);
	}
}

RDOResType::Param::Param(CREF(parser::LPRDORTPParam) param)
	: parser::RDOParserSrcInfo(param->src_info())
	, m_name    (param->name()                  )
	, m_exist   (true                           )
	, m_pType   (param->getTypeInfo()           )
	, m_pDefault(param->getDefault()            )
	, m_id      (-1                             )
{
	switch (typeID())
	{
		case rdo::runtime::RDOType::t_int:
		{
			parser::LPRDOTypeIntRange pRange = param->getTypeInfo()->type().object_dynamic_cast<parser::RDOTypeIntRange>();
			if (pRange)
			{
				m_pMin = pRange->range()->getMin();
				m_pMax = pRange->range()->getMax();
			}
			break;
		}
		case rdo::runtime::RDOType::t_real:
		{
			parser::LPRDOTypeRealRange pRange = param->getTypeInfo()->type().object_dynamic_cast<parser::RDOTypeRealRange>();
			if (pRange)
			{
				m_pMin = pRange->range()->getMin();
				m_pMax = pRange->range()->getMax();
			}
			break;
		}
		default:
			break;
	}
}

// --------------------------------------------------------------------------------
// ---- Инициализация *нового* типа ресурса
// --------------------------------------------------------------------------------
RDOResType::RDOResType(CREF(tstring) name, Type type)
	: m_name (name     )
	, m_exist(false    )
	, m_type (type     )
	, m_id   (ruint(~0))
{}

rbool RDOResType::ParamList::append(REF(Param) param)
{
	if (std::find_if(begin(), end(), parser::compareNameRef<Param>(param.name())) != end())
	{
		return false;
	}

	m_list.push_back(param);
	return true;
}

RDOResType::Param::Param(CREF(tstring) name, CREF(parser::LPTypeInfo) pType, CREF(parser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_exist   (true    )
	, m_pType   (pType   )
	, m_pDefault(pDefault)
	, m_id      (-1      )
{}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<parser::RDOType__int>) pType, CREF(parser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_exist   (true    )
	, m_pDefault(pDefault)
	, m_id      (-1      )
{
	initType(pType);
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<parser::RDOType__real>) pType, CREF(parser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_exist   (true    )
	, m_pDefault(pDefault)
	, m_id      (-1      )
{
	initType(pType);
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::runtime::RDOEnumType::Enums) enums, CREF(parser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_exist   (true    )
	, m_pDefault(pDefault)
	, m_id      (-1      )
{
	initType(enums);
}

void RDOResType::Param::setRange(CREF(parser::LPRDOValue) pMin, CREF(parser::LPRDOValue) pMax)
{
	ASSERT(pMin);
	ASSERT(pMax);

	m_pMin = pMin;
	m_pMax = pMax;
}

void RDOResType::Param::setDefault(CREF(parser::LPRDOValue) pDefault)
{
	ASSERT(pDefault);

	m_pDefault = pDefault;
}

rbool RDOResType::Param::operator== (CREF(Param) param) const
{
	UNUSED(param);
	NEVER_REACH_HERE;
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOResTypeList
// --------------------------------------------------------------------------------
// ---- Собирает все типы ресурсов, которые есть у парсера
// --------------------------------------------------------------------------------
RDOResTypeList::RDOResTypeList(CREF(parser::LPRDOParser) pParser)
	: RDOList<RDOResType>(pParser)
{
	STL_FOR_ALL_CONST(m_pParser->getRTPResTypes(), rtp_it)
	{
		RDOResType rtp(*rtp_it);
		m_list.push_back(rtp);
	}
}

// --------------------------------------------------------------------------------
// ---- Добавление *нового* типа ресурса
// --------------------------------------------------------------------------------
parser::LPRDORTPResType RDOResTypeList::appendBefore(REF(RDOResType) rtp)
{
	if (std::find_if(begin(), end(), parser::compareNameRef<RDOResType>(rtp.name())) != end())
	{
		return parser::LPRDORTPResType(NULL);
	}

	parser::LPRDORTPResType pResourceType = rdo::Factory<parser::RDORTPResType>::create(m_pParser, parser::RDOParserSrcInfo(rtp.name()), rtp.isPermanent());
	ASSERT(pResourceType);
	return pResourceType;
}

rbool RDOResTypeList::appendAfter(REF(RDOResType) rtp, CREF(parser::LPRDORTPResType) pResourceType)
{
	ASSERT(pResourceType);

	STL_FOR_ALL_CONST(rtp.m_params, param)
	{
		parser::LPTypeInfo pParamType;
		parser::LPRDOValue pDefault = param->hasDefault() ? param->getDefault() : rdo::Factory<parser::RDOValue>::create();
		ASSERT(pDefault);
		switch (param->typeID())
		{
			case rdo::runtime::RDOType::t_int:
			{
				if (param->hasRange())
				{
					parser::LPRDOTypeRangeRange pRange    = rdo::Factory<parser::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), parser::RDOParserSrcInfo());
					parser::LPRDOTypeIntRange   pIntRange = rdo::Factory<parser::RDOTypeIntRange>::create(pRange);
					pParamType = rdo::Factory<parser::TypeInfo>::create(pIntRange, parser::RDOParserSrcInfo());
				}
				else
				{
					pParamType = param->type();
				}
				break;
			}
			case rdo::runtime::RDOType::t_real:
			{
				if (param->hasRange())
				{
					parser::LPRDOTypeRangeRange pRange     = rdo::Factory<parser::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), parser::RDOParserSrcInfo());
					parser::LPRDOTypeRealRange  pRealRange = rdo::Factory<parser::RDOTypeRealRange>::create(pRange);
					pParamType = rdo::Factory<parser::TypeInfo>::create(pRealRange, parser::RDOParserSrcInfo());
				}
				else
				{
					pParamType = param->type();
				}
				break;
			}
			case rdo::runtime::RDOType::t_enum:
			{
				pParamType = param->type();
				break;
			}
			default:
			{
				m_pParser->removeRTPResType(pResourceType);
				return false;
			}
		}
		parser::LPRDORTPParam pParam = rdo::Factory<parser::RDORTPParam>::create(pParamType, pDefault, parser::RDOParserSrcInfo(param->name()));
		ASSERT(pParam);
		pResourceType->addParam(pParam);
	}
	rtp.m_exist = true;
	rtp.m_id    = pResourceType->getNumber();
	m_list.push_back(rtp);
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
// ---- Инициализация ресурса по существующему в памяти
// ---- Собирает все параметры существующего в памяти ресурса
// --------------------------------------------------------------------------------
RDOResource::RDOResource(CREF(parser::LPRDORSSResource) rss)
	: parser::RDOParserSrcInfo(rss->src_info())
	, m_name (rss->name()                     )
	, m_exist(true                            )
	, m_rtp  (rss->getType()                  )
	, m_id   (rss->getID()                    )
{
	if (m_rtp.m_params.size() == rss->params().size())
	{
		ruint index = 0;
		STL_FOR_ALL_CONST(m_rtp.m_params, param_it)
		{
			m_params[param_it->name()] = rss->params()[index].param();
			index++;
		}
	}
}

RDOResource::Params::const_iterator RDOResource::operator[] (CREF(tstring) param) const
{
	return m_params.find(param);
}

REF(RDOResource::Params::mapped_type) RDOResource::operator[] (CREF(tstring) param)
{
	RDOResource::Params::iterator param_it = m_params.find(param);
	if (param_it != m_params.end())
	{
		return param_it->second;
	}
	else
	{
		static Params::mapped_type tmpValue;
		return tmpValue;
	}
}

parser::LPRDORSSResource RDOResource::getParserResource(CREF(parser::LPRDOParser) pParser) const
{
	if (!exist())
	{
		return parser::LPRDORSSResource(NULL);
	}

	return pParser->findRSSResource(name());
}

rbool RDOResource::fillParserResourceParams(REF(parser::LPRDORSSResource) pToParserRSS) const
{
	STL_FOR_ALL_CONST(getType().m_params, param_it)
	{
		RDOResource::Params::const_iterator value_it = operator[](param_it->name());
		if (value_it == end())
			return false;

		parser::LPRDOValue pValue = rdo::Factory<parser::RDOValue>::create(value_it->second);
		ASSERT(pValue);
		pValue = param_it->type()->value_cast(pValue);
		/// @todo а почему тут toParserRSS->src_info(), а не value_it->src_info() ?
		pValue->setSrcInfo(pToParserRSS->src_info());
		pToParserRSS->addParam(pValue);
	}
	return true;
}

// --------------------------------------------------------------------------------
// ---- Инициализация *нового* ресурса
// --------------------------------------------------------------------------------
RDOResource::RDOResource(CREF(RDOResType) rtp, CREF(tstring) name)
	: m_name (name                                )
	, m_exist(false                               )
	, m_rtp  (rtp                                 )
	, m_id   (parser::RDORSSResource::UNDEFINED_ID)
{
	STL_FOR_ALL_CONST(m_rtp.m_params, param_it)
	{
		parser::LPRDOValue pValue = rdo::Factory<parser::RDOValue>::create(param_it->type(), param_it->src_info());
		ASSERT(pValue);
		if (param_it->hasDefault())
		{
			pValue = param_it->getDefault();
		}
		else if (param_it->hasRange())
		{
			pValue = param_it->getMin();
		}
		m_params[param_it->name()] = pValue;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOResourceList
// --------------------------------------------------------------------------------
// ---- Собирает все ресурсы, которые есть у парсера
// --------------------------------------------------------------------------------
RDOResourceList::RDOResourceList(CREF(parser::LPRDOParser) pParser)
	: RDOList<RDOResource>(pParser)
{
	STL_FOR_ALL_CONST(m_pParser->getRSSResources(), rss_it)
	{
		RDOResource rss(*rss_it);
		m_list.push_back(rss);
	}
}

CLOSE_COMPILER_MBUILDER_NAMESPACE
