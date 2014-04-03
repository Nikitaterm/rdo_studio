/*!
  \copyright (c) RDO-Team, 2011
  \file      memory.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextMemory
// --------------------------------------------------------------------------------
ContextMemory::ContextMemory()
{
	m_pLocalVariableListStack = rdo::Factory<LocalVariableListStack>::create();
	ASSERT(m_pLocalVariableListStack);
}

LPLocalVariableListStack ContextMemory::getLocalMemory()
{
	return m_pLocalVariableListStack;
}

namespace
{

LPExpression contextGetLocalVariable(const LPLocalVariable& pLocalVariable, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		pLocalVariable->getExpression()->typeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcGetLocalVariable>::create(pLocalVariable->getName()),
		srcInfo
	);
}

LPExpression contextSetLocalVariable(const LPLocalVariable& pLocalVariable, const rdo::runtime::LPRDOCalc& rightValue, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		pLocalVariable->getTypeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable>::create(pLocalVariable->getName(), rightValue),
		srcInfo
	);
}

}

Context::FindResult ContextMemory::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	if (method == Context::METHOD_GET)
	{
		LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(params.identifier());
		if (pLocalVariable)
			return FindResult(CreateExpression(boost::bind(&contextGetLocalVariable, pLocalVariable, srcInfo)));
	}
	else if (method == Context::METHOD_SET)
	{
		LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(params.identifier());
		if (!pLocalVariable)
			return FindResult();

		using namespace rdo::runtime;

		const LPRDOCalc localVariableValue = FindResult(CreateExpression
			(boost::bind(&contextGetLocalVariable, pLocalVariable, srcInfo))
			).getCreateExpression()()->calc();

		const LPRDOCalc rightValue = params.exists(Expression::CONTEXT_PARAM_SET_EXPRESSION)
			? params.get<LPExpression>(Expression::CONTEXT_PARAM_SET_EXPRESSION)->calc()
			: params.get<LPRDOFUNArithm>(RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM)->createCalc(pLocalVariable->getTypeInfo());
		
		LPRDOCalc operationResult;

		switch (params.get<SetOperationType::Type>(Expression::CONTEXT_PARAM_SET_OPERATION_TYPE))
		{
		case SetOperationType::NOCHANGE:
			RDOParser::s_parser()->error().error(srcInfo, "Недопустимый тип операции: NOCHANGE");
			break;
		case SetOperationType::SET        : 
			operationResult = rightValue;
			break;
		case SetOperationType::ADDITION   : 
			operationResult =  rdo::Factory<RDOCalcPlus>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::SUBTRACTION: 
			operationResult =  rdo::Factory<RDOCalcMinus>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::MULTIPLY   : 
			operationResult =  rdo::Factory<RDOCalcMult>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::DIVIDE     : 
			operationResult =  rdo::Factory<RDOCalcDiv>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::INCREMENT  : 
			operationResult =  rdo::Factory<RDOCalcPlus>::create(localVariableValue, rdo::Factory<RDOCalcConst>::create(1));
			break;
		case SetOperationType::DECRIMENT  :
			operationResult =  rdo::Factory<RDOCalcMinus>::create(localVariableValue, rdo::Factory<RDOCalcConst>::create(1));
			break;
		}

		ASSERT(operationResult);
		return FindResult(CreateExpression(boost::bind(&contextSetLocalVariable, pLocalVariable, operationResult, srcInfo)));
	}

	return FindResult();
}

void ContextMemory::push()
{
	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);
	LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
	ASSERT(pContextMemory);

	LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
	ASSERT(pLocalVariableListStack);

	LPLocalVariableList pLocalVariableList = rdo::Factory<LocalVariableList>::create();
	ASSERT(pLocalVariableList);
	pLocalVariableListStack->push(pLocalVariableList);
}

void ContextMemory::pop()
{
	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);
	LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
	ASSERT(pContextMemory);

	LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
	ASSERT(pLocalVariableListStack);

	pLocalVariableListStack->pop();
}

CLOSE_RDO_PARSER_NAMESPACE