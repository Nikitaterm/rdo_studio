/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_activity.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      13.04.2008
  \brief     Описание базового класса для событий и активностей -- RDOActivity
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch/stdpch.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOActivity
// --------------------------------------------------------------------------------
void RDOActivity::addParamCalc(CREF(LPRDOCalc) pCalc)
{
	m_paramsCalcs.push_back(pCalc);
}

int RDOActivity::getResByRelRes(ruint rel_res_id) const
{
	if (m_relResID.size() <= rel_res_id)
	{
		return 0;
	}
	return m_relResID.at(rel_res_id);
}

void RDOActivity::setRelRes(ruint rel_res_id, ruint res_id)
{
	if (m_relResID.size() <= rel_res_id)
	{
		m_relResID.resize(rel_res_id + 1);
	}
	m_relResID[rel_res_id] = res_id;
}

void RDOActivity::setPatternParameters(CREF(LPRDORuntime) pRuntime, const std::vector<LPRDOCalc>& params)
{
	std::vector<RDOValue> params_values;
	params_values.reserve(params.size());
	BOOST_FOREACH(const LPRDOCalc& param, params)
	{
		params_values.push_back(param->calcValue(pRuntime));
	}
	setPatternParameters(pRuntime, params_values);
}

void RDOActivity::setPatternParameters(CREF(LPRDORuntime) pRuntime, const std::vector<RDOValue>& params)
{
	for (size_t index = 0; index < params.size(); ++index)
	{
		pRuntime->setPatternParameter(index, params[index]);
	}
}

void RDOActivity::getRelevantResources(CREF(LPRDORuntime) pRuntime, std::list<LPRDOResource>& rel_res_list)
{
	rel_res_list.clear();
	int size = m_relResID.size();
	for (int i = 0; i < size; ++i)
	{
		rel_res_list.push_back(pRuntime->getResourceByID(m_relResID[i]));
	}
}

void RDOActivity::updateConvertStatus(CREF(LPRDORuntime) pRuntime, const std::vector<RDOResource::ConvertStatus>& status_list)
{
	updateRelRes(pRuntime);
	int i = 0;
	std::list<LPRDOResource>::iterator it = m_relevantResources.begin();
	while (it != m_relevantResources.end())
	{
		LPRDOResource res = *it;
		if (res)
		{
			res->setState(status_list[i]);
			switch (status_list[i])
			{
			case RDOResource::CS_Create:
				{
					res->makeTemporary(true);
					break;
				}
			default: break;
			}
		}
		++i;
		++it;
	}
}

tstring RDOActivity::traceResourcesList(char prefix, CREF(LPRDORuntime) pRuntime)
{
	tstring res;
	for (std::list<LPRDOResource>::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); ++i)
	{
		if (*i)
		{
			res += (*i)->traceResourceState(prefix, pRuntime);
		}
	}
	return res;
}

tstring RDOActivity::traceResourcesListNumbers(CREF(LPRDORuntime) pRuntime, rbool show_create_index)
{
	UNUSED(pRuntime);
#ifndef RDOSIM_COMPATIBLE
	UNUSED(show_create_index);
#endif
	std::ostringstream res;
	res << m_relevantResources.size() << " ";
	for (std::list<LPRDOResource>::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); ++i)
	{
#ifdef RDOSIM_COMPATIBLE
		if (*i && (show_create_index || (!show_create_index && (*i)->getState() != RDOResource::CS_Create)))
		{
#else
		if (*i)
		{
#endif
			res << " " << (*i)->traceId();
		}
		else
		{
			res << " 0";
		}
	}
	return res.str();
}

CLOSE_RDO_RUNTIME_NAMESPACE
