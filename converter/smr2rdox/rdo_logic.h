/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_LOGIC_H_
#define _CONVERTOR_RDOCONVERTER_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
#include "simulator/runtime/rdo_logic_i.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogicActivity
// --------------------------------------------------------------------------------
template<class RTLogic, class Activity>
class RDOLogicActivity: public RDOParserSrcInfo
{
public:
	typedef  rdo::intrusive_ptr<Activity> LPActivity;
	typedef  std::vector<LPActivity>      ActivityList;

	RDOLogicActivity(CREF(RDOParserSrcInfo) src_info)
		: RDOParserSrcInfo(src_info)
	{}
	virtual ~RDOLogicActivity()
	{}

	CREF(tstring) name() const { return src_info().src_text(); }

	LPActivity addNewActivity(CREF(RDOParserSrcInfo) activity_src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	{
		LPActivity pAactivity = rdo::Factory<Activity>::create(m_pRuntimeLogic, activity_src_info, pattern_src_info);
		ASSERT(pAactivity);
		m_activityList.push_back(pAactivity);
		return pAactivity;
	}

	LPActivity getLastActivity() const
	{
		return !m_activityList.empty() ? m_activityList.back() : LPActivity(NULL);
	}
	CREF(ActivityList) getActivities() const { return m_activityList; }

	rbool setPrior(REF(LPRDOFUNArithm) pPrior)
	{
		LPIPriority pPriority = m_pRuntimeLogic;
		if (pPriority)
		{
			return pPriority->setPrior(pPrior->createCalc());
		}
		return false;
	}

protected:
	LPILogic     m_pRuntimeLogic;

private:
	ActivityList m_activityList;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_LOGIC_H_
