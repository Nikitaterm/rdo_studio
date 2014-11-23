/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_internal.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Стандартные функции языка
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function_internal.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

double GetParam<double>::getParam(const LPRDORuntime& pRuntime, std::size_t paramNumber)
{
	return pRuntime->getFuncArgument(paramNumber).getDouble();
}

int GetParam<int>::getParam(const LPRDORuntime& pRuntime, std::size_t paramNumber)
{
	return pRuntime->getFuncArgument(paramNumber).getInt();
}


CLOSE_RDO_RUNTIME_NAMESPACE
