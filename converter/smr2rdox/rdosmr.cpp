/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosmr.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdosmr.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdoparser_rdo.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_smr_file_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_smr_file_error(const char* message)
{
	UNUSED(message);
}

int cnv_smr_sim_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_smr_sim_error(const char* message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOSMR
// --------------------------------------------------------------------------------
RDOSMR::RDOSMR(CREF(tstring) modelName)
	: m_showMode      (rdo::service::simulation::SM_NoShow)
	, m_frameNumber   (1 )
	, m_showRate      (60)
	, m_runStartTime  (0 )
	, m_traceStartTime(rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME)
	, m_traceEndTime  (rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME)
{
	setFile("Model_name", modelName);
	Converter::s_converter()->setSMR(this);
}

void RDOSMR::setShowMode(rdo::service::simulation::ShowMode showMode)
{
	m_showMode = showMode;
}

void RDOSMR::setFrameNumber(int value, CREF(YYLTYPE) pos)
{
	if (value <= 0)
	{
		Converter::s_converter()->error().error(pos, "Номер кадра должен быть больше нуля");
	}
	if (Converter::s_converter()->getNumberFrame() < (ruint)value)
	{
		Converter::s_converter()->error().error(pos, rdo::format("Несуществующий кадр: %d", value));
	}
	m_frameNumber = value;
}

void RDOSMR::setShowRate(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		Converter::s_converter()->error().error(pos, "Масштаб должен быть больше нуля");
	}
	m_showRate = value;
}

void RDOSMR::setRunStartTime(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		Converter::s_converter()->error().error(pos, "Начальное модельное время должно быть больше нуля");
	}
	m_runStartTime = value;
}

void RDOSMR::setTraceStartTime(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		Converter::s_converter()->error().error(pos, "Начальное время трассировки должно быть больше нуля");
	}
	if (getTraceEndTime() != rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceEndTime() <= value)
	{
		Converter::s_converter()->error().push_only(pos, "Начальное время трассировки должно быть меньше конечного");
		Converter::s_converter()->error().push_only(m_traceEndTime_pos, "См. конечное время трассировки");
		Converter::s_converter()->error().push_done();
	}
	m_traceStartTime     = value;
	m_traceStartTime_pos = pos;
}

void RDOSMR::setTraceEndTime(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		Converter::s_converter()->error().error(pos, "Конечное время трассировки должно быть больше нуля");
	}
	if (getTraceStartTime() != rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceStartTime() >= value)
	{
		Converter::s_converter()->error().push_only(pos, "Конечное время трассировки должно быть больше начального");
		Converter::s_converter()->error().push_only(m_traceStartTime_pos, "См. начальное время трассировки");
		Converter::s_converter()->error().push_done();
	}
	m_traceEndTime     = value;
	m_traceEndTime_pos = pos;
}

void RDOSMR::setTerminateIf(REF(LPRDOFUNLogic) pLogic)
{
	if (m_pTerminateIf)
	{
		Converter::s_converter()->error().push_only(pLogic->src_info(), "Terminate_if уже определен");
		Converter::s_converter()->error().push_only(m_pTerminateIf->src_info(), "См. первое определение");
		Converter::s_converter()->error().push_done();
	}
	m_pTerminateIf = pLogic;
	Converter::s_converter()->runtime()->setTerminateIf(pLogic->getCalc());
}

void RDOSMR::setConstValue(CREF(RDOParserSrcInfo) const_info, REF(LPRDOFUNArithm) pArithm)
{
	LPRDOFUNConstant pConstant = Converter::s_converter()->findFUNConstant(const_info.src_text());
	if (!pConstant)
	{
		Converter::s_converter()->error().error(const_info, rdo::format("Константа '%s' не найдена", const_info.src_text().c_str()));
	}
	ASSERT(pArithm);
	pArithm->checkParamType(pConstant->getType());
	rdo::runtime::LPRDOCalc pCalc = pArithm->createCalc(pConstant->getType());
	Converter::s_converter()->runtime()->addInitCalc(rdo::Factory<rdo::runtime::RDOCalcSetConst>::create(pConstant->getNumber(), pCalc));
	Converter::s_converter()->insertChanges(pConstant->src_text(), pArithm->src_text());
}

void RDOSMR::setResParValue(CREF(RDOParserSrcInfo) res_info, CREF(RDOParserSrcInfo) par_info, REF(LPRDOFUNArithm) pArithm)
{
	LPRDORSSResource pResource = Converter::s_converter()->findRSSResource(res_info.src_text());
	if (!pResource)
	{
		Converter::s_converter()->error().error(res_info.src_info(), rdo::format("Ресурс '%s' не найден", res_info.src_text().c_str()));
	}
	LPRDORTPParam pParam = pResource->getType()->findRTPParam(par_info.src_text());
	if (!pParam)
	{
		Converter::s_converter()->error().push_only(par_info.src_info(), rdo::format("Параметр '%s' не найден", par_info.src_text().c_str()));
		Converter::s_converter()->error().push_only(pResource->src_info(), "См. ресурс");
		Converter::s_converter()->error().push_only(pResource->getType()->src_info(), "См. тип ресурса");
		Converter::s_converter()->error().push_done();
	}
	ASSERT(pArithm);
	pArithm->checkParamType(pParam->getType());
	ruint                 parNumb = pResource->getType()->getRTPParamNumber(par_info.src_text());
	rdo::runtime::LPRDOCalc pCalc   = pArithm->createCalc(pParam->getType());
	Converter::s_converter()->runtime()->addInitCalc(rdo::Factory<rdo::runtime::RDOSetResourceParamCalc>::create(pResource->getID(), parNumb, pCalc));
	Converter::s_converter()->insertChanges(res_info.src_text() + "." + par_info.src_text(), pArithm->src_text());
}

void RDOSMR::setSeed(CREF(RDOParserSrcInfo) seq_info, int base)
{
	LPRDOFUNSequence pSequence = Converter::s_converter()->findFUNSequence(seq_info.src_text());
	if (!pSequence)
	{
		Converter::s_converter()->error().error(seq_info, rdo::format("Последовательность '%s' не найдена", seq_info.src_text().c_str()));
	}
	pSequence->getInitCalc()->setBase(base);
	Converter::s_converter()->insertChanges(pSequence->src_text() + ".Seed", rdo::format("%d", base));
}

void RDOSMR::insertBreakPoint(CREF(RDOParserSrcInfo) src_info, REF(LPRDOFUNLogic) pLogic)
{
	STL_FOR_ALL_CONST(m_breakPointList, it)
	{
		if ((*it)->src_text() == src_info.src_text())
		{
			Converter::s_converter()->error().push_only(src_info, rdo::format("Точка останова с именем '%s' уже существует", src_info.src_text().c_str()));
			Converter::s_converter()->error().push_only((*it)->src_info(), "См. первое определение");
			Converter::s_converter()->error().push_done();
		}
	}
	LPBreakPoint pBreakPoint = rdo::Factory<BreakPoint>::create(src_info, pLogic);
	ASSERT(pBreakPoint);
	m_breakPointList.push_back(pBreakPoint);
}

RDOSMR::BreakPoint::BreakPoint(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic)
	: RDOParserSrcInfo(src_info)
{
	ASSERT(pLogic);
	Converter::s_converter()->runtime()->insertBreakPoint(src_text(), pLogic->getCalc());
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
