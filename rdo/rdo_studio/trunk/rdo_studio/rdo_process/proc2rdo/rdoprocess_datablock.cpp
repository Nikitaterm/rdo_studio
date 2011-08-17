/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoprocess_datablock.cpp
  \author    ������� ������
  \date      04.05.2011
  \brief     
  \indent    4T
*/

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_datablock.h"
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlock
// ----------------------------------------------------------------------------
RPShapeDataBlock::RPShapeDataBlock(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name)
	: m_zakon(zakon)
	, m_name (name )
{}

void RPShapeDataBlock::setDisp(double disp)
{
	m_disp=disp;
}

void RPShapeDataBlock::setExp(double exp)
{
	m_exp=exp;
}

void RPShapeDataBlock::setBase(int base)
{
	m_base=base;
}

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockCreate
// ----------------------------------------------------------------------------
RPShapeDataBlockCreate::RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name)
	: RPShapeDataBlock(zakon, name)
{}

RPShapeDataBlockCreate::~RPShapeDataBlockCreate()
{}

void RPShapeDataBlockCreate::setAmount(int amount)
{
	m_amount=amount;
}

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockTerminate
// ----------------------------------------------------------------------------
RPShapeDataBlockTerminate::RPShapeDataBlockTerminate(CREF(tstring) name)
	:m_name(name)
{}

RPShapeDataBlockTerminate::~RPShapeDataBlockTerminate()
{}

void RPShapeDataBlockTerminate::setTermInc(int term_inc)
{
	m_term_inc = term_inc;
}

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockProcess
// ----------------------------------------------------------------------------
RPShapeDataBlockProcess::RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name)
	: RPShapeDataBlock(zakon,name)
{}

RPShapeDataBlockProcess::~RPShapeDataBlockProcess()
{}

void RPShapeDataBlockProcess::addAction(RPShapeDataBlockProcess::Action action)
{
	m_actionList.push_back(action);
}

void RPShapeDataBlockProcess::addRes(CREF(tstring) resName)
{
	ASSERT(std::find(m_resNameList.begin(), m_resNameList.end(), resName) == m_resNameList.end());

	m_resNameList.push_back(resName);
}

CREF(RPShapeDataBlockProcess::ActionList) RPShapeDataBlockProcess::getActionList() const
{
	return m_actionList;
}

CREF(RPShapeDataBlockProcess::ResNameList) RPShapeDataBlockProcess::getResNameList() const
{
	return m_resNameList;
}
