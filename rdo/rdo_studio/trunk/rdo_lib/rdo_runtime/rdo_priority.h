/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_priority.h
 * @authors   ����� �������
 * @date      04.11.09
 * @brief     �������� ���������� �����������. ������������ ��� ���������� ��.
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_PRIORITY_H_
#define _LIB_RUNTIME_PRIORITY_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_priority_i.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOPatternPrior
// ********************************************************************************
class RDOPatternPrior: public IPriority
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPriority)
QUERY_INTERFACE_END

protected:
	RDOPatternPrior()
	{}
	virtual ~RDOPatternPrior()
	{}

private:
	virtual LPRDOCalc getPrior()
	{
		return m_pPrior; 
	}
	virtual rbool setPrior(CREF(LPRDOCalc) pPrior)
	{
		m_pPrior = pPrior;
		return true;
	}

	LPRDOCalc m_pPrior;
};

// ********************************************************************************
// ******************** RDODPTActivityCompare
// ********************************************************************************
class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare(CREF(LPRDORuntime) pRuntime)
		: m_pRuntime(pRuntime)
	{}
	rbool operator() (CREF(LPIBaseOperation) opr1, CREF(LPIBaseOperation) opr2)
	{
		LPIPriority pattern1 = opr1;
		LPIPriority pattern2 = opr2;
		if (pattern1 && pattern2)
		{
			LPRDOCalc prior1 = pattern1->getPrior();
			LPRDOCalc prior2 = pattern2->getPrior();
			RDOValue value1 = prior1 ? prior1->calcValue(m_pRuntime) : RDOValue(0.0);
			RDOValue value2 = prior2 ? prior2->calcValue(m_pRuntime) : RDOValue(0.0);
			return value1 > value2;
		}
		return false;
	}

private:
	LPRDORuntime m_pRuntime;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_priority.inl"

#endif // _LIB_RUNTIME_PRIORITY_H_
