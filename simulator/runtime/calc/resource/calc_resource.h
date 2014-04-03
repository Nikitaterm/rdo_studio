/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_resource.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      19.12.2011
  \brief     Калки для ресурсов
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_H_
#define _LIB_RUNTIME_CALC_RESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Вспомогательный класс для вытаскивания ресурса по ID и оборачивания его в RDOValue
class RDOCalcGetResourceHelper
{
public:
	static rbool getResource(CREF(LPRDORuntime) pRuntime, ruint resourceID, REF(RDOValue) result);
};

//! Получение ресурса по ID
CALC(RDOCalcGetResourceByID)
{
DECLARE_FACTORY(RDOCalcGetResourceByID)
private:
	RDOCalcGetResourceByID(CREF(ruint) resourceID);

	ruint m_resourceID;

	DECLARE_ICalc;
};

//! Получение параметр ресурса по калку ресурса и ID параметра
CALC(RDOCalcGetResourceParam)
{
DECLARE_FACTORY(RDOCalcGetResourceParam)
private:
	RDOCalcGetResourceParam(CREF(LPRDOCalc) pResource, ruint paramID);

	LPRDOCalc m_pResource;
	int       m_paramID;

	DECLARE_ICalc;
};

//! Параметры несуществующего ресурса
CALC(RDOCalcGetUnknowResParam)
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName);

	tstring m_resName;
	tstring m_parName;

	DECLARE_ICalc;
};

//! Установка значения параметра ресурса перед моделированием из SMR
CALC(RDOSetResourceParamCalc)
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(ruint resourceID, ruint paramID, CREF(LPRDOCalc) pCalc);

	ruint      m_resourceID;
	ruint      m_paramID;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

CALC(RDOCalcSetResourceTrace)
{
DECLARE_FACTORY(RDOCalcSetResourceTrace)
private:
	RDOCalcSetResourceTrace(const LPRDOCalc& getResource, bool traceValue);

	LPRDOCalc m_getResource;
	bool  m_traceValue;

	DECLARE_ICalc;
};

template <SetOperationType::Type setOperationType>
class RDOSetResourceParam: public RDOCalc
{
public:
	RDOSetResourceParam(const LPRDOCalc& getResource, const ruint paramID, const LPRDOCalc& pCalc = NULL);
	virtual ~RDOSetResourceParam();

protected:
	LPRDOCalc m_getResource;
	ruint     m_paramID;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/resource/calc_resource.inl"

#endif // _LIB_RUNTIME_CALC_RESOURCE_H_
