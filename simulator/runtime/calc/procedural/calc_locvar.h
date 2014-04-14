/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_locvar.h
  \author    Чирков Михаил
  \date      03.12.2010
  \brief     Локальные переменные
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Создание локальной переменной
PREDECLARE_POINTER(RDOCalcCreateLocalVariable);
class RDOCalcCreateLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcCreateLocalVariable)
private:
	RDOCalcCreateLocalVariable(CREF(std::string) name, CREF(LPRDOCalc) pValueCalc);

	std::string m_name;
	LPRDOCalc  m_pValueCalc;

	DECLARE_ICalc;
};

//! Получение значения локальной переменной
PREDECLARE_POINTER(RDOCalcGetLocalVariable);
class RDOCalcGetLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetLocalVariable)
private:
	RDOCalcGetLocalVariable(CREF(std::string) name);

	std::string m_name;

	DECLARE_ICalc;
};

//! Установка значения локальной переменной
class RDOCalcSetLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetLocalVariable)
private:
	RDOCalcSetLocalVariable(CREF(std::string) name, LPRDOCalc pCalc = NULL);
	virtual ~RDOCalcSetLocalVariable();

	std::string m_name;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

//! Список локальных переменных
PREDECLARE_POINTER(RDOCalcLocalVariableList);
class RDOCalcLocalVariableList: public RDOCalc
{
DECLARE_FACTORY(RDOCalcLocalVariableList)
public:
	typedef std::vector<LPRDOCalc> CalcLocalVariableList;

	void addCalcLocalVariable(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcLocalVariableList();

	CalcLocalVariableList m_calcLocalVariableList;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_
