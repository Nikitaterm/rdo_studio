/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopat.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/compiler/parser/local_variable.h"
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int patlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void paterror(PTR(char) message)
{
	UNUSED(message);
}

int evnlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void evnerror(PTR(char) message)
{
	UNUSED(message);
}

int evn_preparse_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void evn_preparse_error(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPATPattern
// --------------------------------------------------------------------------------
RDOPATPattern::RDOPATPattern(CREF(RDOParserSrcInfo) name_src_info)
	: RDOParserSrcInfo    (name_src_info)
	, m_pCommonChoice     (NULL         )
	, m_pPatRuntime       (NULL         )
	, m_pCurrRelRes       (NULL         )
	, m_currentRelResIndex(0            )
	, m_useCommonChoice   (false        )
{
	LPRDOPATPattern pPatternExist = RDOParser::s_parser()->findPATPattern(src_text());
	if (pPatternExist)
	{
		rdoParser::g_error().push_only(src_info(), rdo::format(_T("������� '%s' ��� ����������"), name().c_str()));
		rdoParser::g_error().push_only(pPatternExist->src_info(), _T("��. ������ �����������"));
		rdoParser::g_error().push_done();
	}
	RDOParser::s_parser()->insertPATPattern(this);
	RDOParser::s_parser()->contextStack()->push(this);

	m_pContextMemory = rdo::Factory<ContextMemory>::create();
	ASSERT(m_pContextMemory);
	RDOParser::s_parser()->contextStack()->push(m_pContextMemory);

	ContextMemory::push();
}

RDOPATPattern::~RDOPATPattern()
{}

tstring RDOPATPattern::typeToString(PatType type) const
{
	switch (type)
	{
	case PT_Event    : return _T("�������");
	case PT_Rule     : return _T("������������� �������");
	case PT_Operation: return _T("��������");
	case PT_Keyboard : return _T("������������ ��������");
	default          : return _T("�����������");
	}
}

LPRDORelevantResource RDOPATPattern::findRelRes(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	//! ����������� �������
	LPRDORelevantResource pRelevantResource = findRelevantResource(pValue->value().getIdentificator());
	if (pRelevantResource)
	{
		if (!m_pCurrRelRes)
		{
			//! ������ with_min-common-choice ��� $Time
			if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_NonExist || pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Create)
			{
				RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ �� ����� ���� �����������, �.�. �� ��� �� ����������: %s"), pRelevantResource->name().c_str()));
			}
		}
		else
		{
			//! ������ $Body
			//! ��������� ������������� ��������������������� ���.������� (pRelevantResource) � Choice from ������ ���.������� (m_pCurrRelRes)
			if (m_pCurrRelRes->isChoiceFromState())
			{
				if (!pRelevantResource->m_alreadyHaveConverter && !pRelevantResource->isDirect())
				{
					RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ �� ���������: %s. ��� ������ ������������ � �������� ������ ������ �������� �� ��� ������������ Choice from"), pRelevantResource->name().c_str()));
				}
				if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_NonExist)
				{
					RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ � ������ �������� �� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
				}
				if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Create)
				{
					RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����� ����� �������� (Create) ����������� ������ '%s' ����� ������������ ������ � �����������, �� �� � ������� ������"), pRelevantResource->name().c_str()));
				}
			}
			//! ��������� ������������� ���������� ���.������� ������ ���������� ������� ���.�������
			if (pRelevantResource->getType()->isTemporary())
			{
				//! � ���������� ������
				if (m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin)
				{
					if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ ������ ������������ �� ��� �������� (Create): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ ������ ������������ ����� �������� (Erase): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_NonExist)
					{
						RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ �� ���������� � ���� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
					}
				}
				//! � ���������� �����
				if (m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd)
				{
					if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ ������ ������������ �� ��� �������� (Create): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ ������ ������������ ����� �������� (Erase): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::CS_NonExist)
					{
						RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� ������ �� ���������� � ���� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
					}
				}
			}
		}
	}
	return pRelevantResource;
}

Context::FindResult RDOPATPattern::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	Context::FindResult result = m_pContextMemory->onFindContext(pValue);
	if (result.m_pContext)
	{
		return result;
	}

	//! ����������� �������
	LPRDORelevantResource pRelevantResource = findRelRes(pValue);
	if (pRelevantResource)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				pRelevantResource->getType(),
				pValue->src_info()
			),
			rdo::Factory<rdo::runtime::RDOGetResourceByRelevantResourceID>::create(pRelevantResource->m_relResID),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOPATPattern)>(this), pExpression, pValue, pRelevantResource);
	}

	//! ���������
	LPRDOParam pParam = findPATPatternParam(pValue->value().getIdentificator());
	if (pParam)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pParam->getTypeInfo(),
			rdo::Factory<rdo::runtime::RDOCalcPatParam>::create(findPATPatternParamNum(pValue->value().getIdentificator())),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOPATPattern)>(this), pExpression, pValue);
	}

	return Context::FindResult();
}

tstring RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::ConvertStatus value)
{
	switch (value)
	{
	case rdo::runtime::RDOResource::CS_Keep    : return _T("Keep");
	case rdo::runtime::RDOResource::CS_Create  : return _T("Create");
	case rdo::runtime::RDOResource::CS_Erase   : return _T("Erase");
	case rdo::runtime::RDOResource::CS_NonExist: return _T("NonExist");
	case rdo::runtime::RDOResource::CS_NoChange: return _T("NoChange");
	default                                    : NEVER_REACH_HERE;
	}
	return tstring();
}

rdo::runtime::RDOResource::ConvertStatus RDOPATPattern::StrToStatus(CREF(tstring) value, CREF(YYLTYPE) convertor_pos)
{
	if (value == _T("Keep") || value == _T("keep"))
	{
		return rdo::runtime::RDOResource::CS_Keep;
	}
	else if (value == _T("Create") || value == _T("create"))
	{
		return rdo::runtime::RDOResource::CS_Create;
	}
	else if (value == _T("Erase") || value == _T("erase"))
	{
		return rdo::runtime::RDOResource::CS_Erase;
	}
	else if (value == _T("NonExist") || value == _T("nonexist"))
	{
		return rdo::runtime::RDOResource::CS_NonExist;
	}
	else if (value == _T("NoChange") || value == _T("nochange"))
	{
		return rdo::runtime::RDOResource::CS_NoChange;
	}
	rdoParser::g_error().error(convertor_pos, rdo::format(_T("�������� ������ ����������: %s"), value.c_str()));
	return rdo::runtime::RDOResource::CS_Keep;
}

void RDOPATPattern::beforeRelRensert(CREF(RDOParserSrcInfo) rel_info)
{
	if (findRelevantResource(rel_info.src_text()))
	{
		rdoParser::g_error().error(rel_info, rdo::format(_T("����������� ������ '%s' ��� ���������"), rel_info.src_text().c_str()));
	}
}

void RDOPATPattern::rel_res_insert(CREF(LPRDORelevantResource) pRelevantResource)
{
	ASSERT(pRelevantResource);
	switch (getType())
	{
	case PT_Event    : getPatRuntime<rdo::runtime::RDOPatternEvent    >()->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Rule     : getPatRuntime<rdo::runtime::RDOPatternRule     >()->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Operation: getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	case PT_Keyboard : getPatRuntime<rdo::runtime::RDOPatternKeyboard >()->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	default          : rdoParser::g_error().error(src_info(), _T("����������� ��� �������"));
	}
	m_relResList.push_back(pRelevantResource);
}

void RDOPATPattern::addRelResConvert(rbool trace, CREF(LPConvertCmdList) commands, CREF(YYLTYPE) convertor_pos, CREF(YYLTYPE) trace_pos, rdo::runtime::RDOResource::ConvertStatus status)
{
	if (status == rdo::runtime::RDOResource::CS_NoChange || status == rdo::runtime::RDOResource::CS_NonExist)
	{
		rdoParser::g_error().error(convertor_pos, getErrorMessage_NotNeedConvertor(m_pCurrRelRes->name(), status));
	}

	if (status == rdo::runtime::RDOResource::CS_Create)
	{
		addParamSetCalc(createRelRes(trace));
	}
	else
	{
		if (trace)
		{
			rdoParser::g_error().error(trace_pos, _T("������� ����������� � ������ ����� �������� ������ ��� ������������ �������"));
		}
	}

	if (commands->commands().empty() && status == rdo::runtime::RDOResource::CS_Keep)
	{
		rdoParser::g_error().warning(convertor_pos, getWarningMessage_EmptyConvertor(m_pCurrRelRes->name(), status));
	}

	STL_FOR_ALL_CONST(commands->commands(), cmdIt)
		addParamSetCalc(*cmdIt);

	ASSERT(m_pCurrRelRes);
}

void RDOPATPattern::addParamSetCalc(CREF(rdo::runtime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_Event    : getPatRuntime<rdo::runtime::RDOPatternEvent>()->addConvertorCalc(pCalc); break;
	case PT_Rule     : getPatRuntime<rdo::runtime::RDOPatternRule >()->addConvertorCalc(pCalc); break;
	case PT_Operation: NEVER_REACH_HERE;
	case PT_Keyboard : NEVER_REACH_HERE;
	default          : rdoParser::g_error().error(src_info(), _T("����������� ��� �������"));
	}
}

tstring RDOPATPattern::getPatternId() const
{ 
	return m_pPatRuntime->traceId(); 
}

void RDOPATPattern::writeModelStructure(REF(std::ostream) stream) const
{
	stream << getPatternId() << " " << name() << " " << getModelStructureLetter() << " " << m_relResList.size();
	STL_FOR_ALL_CONST(m_relResList, it)
		stream << " " << (*it)->getType()->getNumber();

	stream << std::endl;
}

LPRDOParam RDOPATPattern::findPATPatternParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? (*it) : LPRDOParam(NULL);
}

LPRDORelevantResource RDOPATPattern::findRelevantResource(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? (*it) : LPRDORelevantResource(NULL);
}

int RDOPATPattern::findPATPatternParamNum(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

ruint RDOPATPattern::findRelevantResourceNum(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? it - m_relResList.begin() : ruint(~0);
}

void RDOPATPattern::add(CREF(LPRDOParam) pParam)
{
	ASSERT(pParam);

	LPRDOParam pParamExist = findPATPatternParam(pParam->name());
	if (pParamExist)
	{
		rdoParser::g_error().push_only(pParam->src_info(), rdo::format(_T("�������� '%s' ��� ���������"), pParam->src_text().c_str()));
		rdoParser::g_error().push_only(pParamExist->src_info(), _T("��. ������ �����������"));
		rdoParser::g_error().push_done();
	}
	m_paramList.push_back(pParam);
}

void RDOPATPattern::setCommonChoiceFirst()
{
	m_useCommonChoice = true;
	m_pCommonChoice   = NULL;
//	rdoParser::g_error().error(_T("�������� ������, �.�. � ���������� �� ������� ������������ m_pCommonChoice"));
}

void RDOPATPattern::setCommonChoiceWithMin(CREF(LPRDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = false;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setCommonChoiceWithMax(CREF(LPRDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = true;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setTime(REF(LPRDOFUNArithm) arithm)
{ 
	switch (getType())
	{
	case PT_Operation: getPatRuntime<rdo::runtime::RDOPatternOperation>()->setTime(arithm->createCalc(NULL)); break;
	case PT_Keyboard : getPatRuntime<rdo::runtime::RDOPatternKeyboard >()->setTime(arithm->createCalc(NULL)); break;
	default          : rdoParser::g_error().error(src_info(), rdo::format(_T("��� ������� ���� %s ����������� ������������� ��������� �������"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addChoiceFromCalc(CREF(rdo::runtime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_Event    : getPatRuntime<rdo::runtime::RDOPatternEvent    >()->addPreSelectRelRes(pCalc); break;
	case PT_Rule     : getPatRuntime<rdo::runtime::RDOPatternRule     >()->addChoiceFromCalc (pCalc); break;
	case PT_Operation: getPatRuntime<rdo::runtime::RDOPatternOperation>()->addChoiceFromCalc (pCalc); break;
	case PT_Keyboard : getPatRuntime<rdo::runtime::RDOPatternKeyboard >()->addChoiceFromCalc (pCalc); break;
	default          : rdoParser::g_error().error(src_info(), rdo::format(_T("��� ������� ���� %s ����������� ������������� �������� ������"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addRelResBody(CREF(RDOParserSrcInfo) body_name)
{ 
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(body_name.src_text()));
	if (it == m_relResList.end())
	{
		rdoParser::g_error().error(body_name.src_info(), rdo::format(_T("����������� ����������� ������: %s"), body_name.src_text().c_str()));
	}
	if (findRelevantResourceNum(body_name.src_text()) != m_currentRelResIndex)
	{
		tstring rel_res_waiting = m_currentRelResIndex < m_relResList.size() ? m_relResList[m_currentRelResIndex]->name().c_str() : _T("");
		rdoParser::g_error().error(body_name.src_info(), rdo::format(_T("��������� �������� ������������ ������� '%s', ������ ���� �������: %s"), rel_res_waiting.c_str(), body_name.src_text().c_str()));
	}
	if ((*it)->m_alreadyHaveConverter)
	{
		rdoParser::g_error().error(body_name.src_info(), rdo::format(_T("����������� ������ ��� ������������: %s"), body_name.src_text().c_str()));
	}
	m_pCurrRelRes = (*it);
	m_pCurrRelRes->m_bodySrcInfo = body_name;
	m_pCurrRelRes->m_alreadyHaveConverter = true;
	m_currentRelResIndex++;
}

void RDOPATPattern::addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder)
{
	if (!m_useCommonChoice)
	{
		if (pChoiceOrder->m_type == rdo::runtime::RDOSelectResourceCalc::order_empty)
		{
			if ((m_pCurrRelRes->m_statusBegin != rdo::runtime::RDOResource::CS_Create) && (m_pCurrRelRes->m_statusEnd != rdo::runtime::RDOResource::CS_Create))
			{
//				pChoiceOrder->m_type = rdo::runtime::RDOSelectResourceCalc::order_first;
			}
		}
		else if (m_pCurrRelRes->isDirect())
		{
			rdoParser::g_error().warning(pChoiceOrder->src_info(), rdo::format(_T("������� ������ '%s' ������������ ������� '%s' �� ����� ������, �.�. ����������� ������ ��������� ����� ���, � �� ���, � �� ����� ���� ������ � �����-���� ������ ��������"), pChoiceOrder->src_text().c_str(), m_pCurrRelRes->name().c_str()));
		}
	}
	else
	{
		if (pChoiceOrder->m_type != rdo::runtime::RDOSelectResourceCalc::order_empty)
		{
			rdoParser::g_error().push_only(pChoiceOrder->src_info(), _T("������ ������� ������ ������ ������������ �������, �.�. ������������ ������ ��� ���� ����������� �������� ������, ��������� �� ��������� ����� $Body"));
			if (m_pCommonChoice)
			{
				rdoParser::g_error().push_only(m_pCommonChoice->src_info(), rdo::format(_T("��. '%s'"), m_pCommonChoice->src_text().c_str()));
			}
			rdoParser::g_error().push_done();
		}
	}

	if ((m_pCurrRelRes->m_statusBegin == rdo::runtime::RDOResource::CS_Create) || (m_pCurrRelRes->m_statusEnd == rdo::runtime::RDOResource::CS_Create))
	{
		if (pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
		{
			rdoParser::g_error().error(pChoiceFrom->src_info(), _T("����������� ������ ���������, ��� ���� ������ ������������ Choice from ��� Choice NoCheck"));
		}
		if (pChoiceOrder->m_type != rdo::runtime::RDOSelectResourceCalc::order_empty)
		{
			rdoParser::g_error().error(pChoiceOrder->src_info(), rdo::format(_T("����������� ������ ���������, ��� ���� ������ ������������ ������� ������ '%s'"), pChoiceOrder->asString().c_str()));
		}
	}

	m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
	m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

void RDOPATPattern::end()
{
	int size = m_relResList.size();
	for (int i = 0; i < size; i++)
	{
		LPRDORelevantResource pCurrRelRes = m_relResList.at(i);
		if (!pCurrRelRes->m_alreadyHaveConverter)
		{
			/// @todo � ������ ������ ������� warning ? ��������, ���� ������� ���������� ����������� ������� ���. �������.
			rdoParser::g_error().error(pCurrRelRes->src_info(), rdo::format(_T("����������� ������ '%s' �� ������������ � ������� '%s'"), pCurrRelRes->name().c_str(), name().c_str()));
		}
		m_pPatRuntime->addPreSelectRelRes(pCurrRelRes->createPreSelectRelResCalc());
	}

	if (m_useCommonChoice)
	{
		//! first
		//! �������� �����������, � ��� ������������ first
		if (!m_pCommonChoice)
		{
			//! first
			std::vector<rdo::runtime::LPIRDOSelectResourceCommon> resSelectors;
			for (int i = 0; i < size; i++)
			{
				if (m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::CS_Keep || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::CS_Erase || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::CS_NoChange)
				{
					resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
				}
			}
			rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, rdo::runtime::LPRDOCalc(NULL));
			ASSERT(pCalc);
			rdo::runtime::RDOSrcInfo srcInfo(src_info());
			srcInfo.setSrcText(_T("first"));
			pCalc->setSrcInfo(srcInfo);
			addChoiceFromCalc(pCalc);
		}
		else
		{
			//! with_min/with_max
			std::vector<rdo::runtime::LPIRDOSelectResourceCommon> resSelectors;
			for (int i = 0; i < size; i++)
			{
				if (m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::CS_Keep || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::CS_Erase || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::CS_NoChange)
				{
					resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
				}
			}
			rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, m_pCommonChoice->createCalc());
			pCalc->setSrcInfo(m_pCommonChoice->src_info());
			addChoiceFromCalc(pCalc);
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			rdo::runtime::LPRDOCalc pCalc = m_relResList.at(i)->createSelectResourceChoiceCalc();
			addChoiceFromCalc(pCalc);
		}
	}
	ContextMemory::pop();
	RDOParser::s_parser()->contextStack()->pop();
	RDOParser::s_parser()->contextStack()->pop();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternEvent
// --------------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(name_src_info)
{
	m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternEvent>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

void RDOPatternEvent::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdo::runtime::RDOResource::CS_NonExist || beg == rdo::runtime::RDOResource::CS_NoChange)
	{
		rdoParser::g_error().error(convertor_pos, _T("������� ����������� NonExist � NoChange �� ����� ���� ������������ � �������"));
	}

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		switch (beg)
		{
		case rdo::runtime::RDOResource::CS_Create: rdoParser::g_error().error(type_info, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str())); break;
		case rdo::runtime::RDOResource::CS_Erase : rdoParser::g_error().error(convertor_pos, _T("������� ������� � ������� ������")); break;
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoParser::g_error().error(type_info, rdo::format(_T("����������� ��� �������: %s"), type_info.src_text().c_str()));
		}
		switch (beg)
		{
		case rdo::runtime::RDOResource::CS_Create: if (!pResType->isTemporary()) rdoParser::g_error().error(type_info, rdo::format(_T("��� ������� '%s' ����������. ����������� ��������� �� ���� ������� ������"), type_info.src_text().c_str())); break;
		case rdo::runtime::RDOResource::CS_Keep  :
		case rdo::runtime::RDOResource::CS_Erase : rdoParser::g_error().error(convertor_pos, _T("������� ����������� Keep � Erase ����� ���� ������������ � ������� � ���������� � ���� �������, �� �� ���� �������")); break;
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Erase)
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdo::runtime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdo::runtime::RDOPatternEvent>()->addEraseCalc(pCalc);
	}
}

void RDOPatternEvent::addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder)
{
	if (pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
	{
		rdoParser::g_error().error(pChoiceFrom->src_info(), _T("����������� ������� ������� ������ �������� � ������� Choice from ��� Choice NoCheck"));
	}
	if (pChoiceOrder->m_type != rdo::runtime::RDOSelectResourceCalc::order_empty)
	{
		rdoParser::g_error().error(pChoiceFrom->src_info(), rdo::format(_T("��� ����������� �������� ������� ������ ������������ ������� ������ '%s'"), pChoiceOrder->asString().c_str()));
	}
	m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
	m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

rdo::runtime::LPRDOCalc RDOPATPattern::createRelRes(rbool trace) const
{
	std::vector<rdo::runtime::RDOValue> params_default;
	STL_FOR_ALL_CONST(m_pCurrRelRes->getType()->getParams(), it)
	{
		if (!(*it)->getDefault()->defined())
		{
			params_default.push_back(rdo::runtime::RDOValue(0));
			if (!m_pCurrRelRes->getParamSetList().find((*it)->name()))
			{
				rdoParser::g_error().error(m_pCurrRelRes->src_info(), rdo::format(_T("��� �������� ������� ���������� ���������� ��� ��� ���������. �� ������� ����������� ���������: %s"), (*it)->name().c_str()));
			}
		}
		else
		{
			params_default.push_back((*it)->getDefault()->value());
		}
	}
	rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcCreateResource>::create(m_pCurrRelRes->getType()->getRuntimeResType(), params_default, trace, false/** @todo ���������, ��� ������ ��������� */, m_pCurrRelRes->m_relResID);
	ASSERT(pCalc);
	rdo::runtime::RDOSrcInfo srcInfo(m_pCurrRelRes->src_info());
	srcInfo.setSrcText(rdo::format(_T("�������� ���������� ������� %s"), m_pCurrRelRes->name().c_str()));
	pCalc->setSrcInfo(srcInfo);
	return pCalc;
}

tstring RDOPatternEvent::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� (Convert_event), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

tstring RDOPatternEvent::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� (Convert_event), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(name_src_info)
{
//	RDOParser::s_parser()->runtime()->addRuntimeRule((RDOPatternRule *)(m_pPatRuntime = new RDOPatternRule(RDOParser::s_parser()->runtime(), _trace))); 
	m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternRule>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

void RDOPatternRule::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdo::runtime::RDOResource::CS_NonExist)
	{
		rdoParser::g_error().error(convertor_pos, rdo::format(_T("������ ������������ ������ ���������� '%s' � ������������� �������"), RDOPATPattern::StatusToStr(beg).c_str()));
	}

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdo::runtime::RDOResource::CS_Create)
		{
			rdoParser::g_error().error(type_info, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdo::runtime::RDOResource::CS_Erase)
		{
			rdoParser::g_error().error(convertor_pos, rdo::format(_T("������������ ������ ���������� ��� �������: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoParser::g_error().error(type_info, rdo::format(_T("����������� ��� �������: %s"), type_info.src_text().c_str()));
		}
		if (!pResType->isTemporary() && (beg == rdo::runtime::RDOResource::CS_Create || beg == rdo::runtime::RDOResource::CS_Erase))
		{
			rdoParser::g_error().error(type_info, rdo::format(_T("��� �������� ��� �������� ������� ��� ��� ������ ���� ��������� (temporary), � �� ���������� (permanent), ��� � '%s'"), type_info.src_text().c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Erase)
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdo::runtime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdo::runtime::RDOPatternRule>()->addEraseCalc(pCalc);
	}
}

tstring RDOPatternRule::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� (Convert_rule), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

tstring RDOPatternRule::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� (Convert_rule), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern  (name_src_info )
	, m_convertorType(convert_unknow)
{
	m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternOperation>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

RDOPatternOperation::RDOPatternOperation(rbool trace, CREF(RDOParserSrcInfo) name_src_info)
	: RDOPATPattern  (name_src_info )
	, m_convertorType(convert_unknow)
{
	UNUSED(trace);
}

void RDOPatternOperation::rel_res_insert(CREF(LPRDORelevantResource) pRelevantResource)
{
	ASSERT(pRelevantResource);
	RDOPATPattern::rel_res_insert(pRelevantResource);
	getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorEndStatus(pRelevantResource->m_statusEnd);
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	UNUSED(rel_info );
	UNUSED(type_info);
	UNUSED(beg      );

	rdoParser::g_error().error(convertor_pos, _T("���������� ������ �������"));
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, rdo::runtime::RDOResource::ConvertStatus end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos)
{
	beforeRelRensert(rel_info);
	switch (beg)
	{
	case rdo::runtime::RDOResource::CS_Keep:
		if (end != rdo::runtime::RDOResource::CS_Keep && end != rdo::runtime::RDOResource::CS_Erase && end != rdo::runtime::RDOResource::CS_NoChange)
		{
			rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdo::runtime::RDOResource::CS_Create:
		if (end != rdo::runtime::RDOResource::CS_Keep && end != rdo::runtime::RDOResource::CS_Erase && end != rdo::runtime::RDOResource::CS_NoChange)
		{
			rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdo::runtime::RDOResource::CS_Erase:
		if (end != rdo::runtime::RDOResource::CS_NonExist)
		{
			rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdo::runtime::RDOResource::CS_NonExist:
		if (end != rdo::runtime::RDOResource::CS_Create)
		{
			rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdo::runtime::RDOResource::CS_NoChange:
		if (end != rdo::runtime::RDOResource::CS_Keep && end != rdo::runtime::RDOResource::CS_Erase && end != rdo::runtime::RDOResource::CS_NoChange)
		{
			rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	};

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdo::runtime::RDOResource::CS_Create)
		{
			rdoParser::g_error().error(convertor_begin_pos, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (end == rdo::runtime::RDOResource::CS_Create)
		{
			rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdo::runtime::RDOResource::CS_Erase || beg == rdo::runtime::RDOResource::CS_NonExist)
		{
			rdoParser::g_error().error(convertor_begin_pos, rdo::format(_T("������������ ������ ���������� ������ ��� �������: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		if (end == rdo::runtime::RDOResource::CS_Erase || end == rdo::runtime::RDOResource::CS_NonExist)
		{
			rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("������������ ������ ���������� ����� ��� �������: %s"), RDOPATPattern::StatusToStr(end).c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg, end);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoParser::g_error().error(type_info, rdo::format(_T("����������� ��� �������: %s"), type_info.src_text().c_str()));
		}
		if (pResType->isPermanent())
		{
			if (beg == rdo::runtime::RDOResource::CS_Create || beg == rdo::runtime::RDOResource::CS_Erase || beg == rdo::runtime::RDOResource::CS_NonExist)
			{
				rdoParser::g_error().error(convertor_begin_pos, rdo::format(_T("������������ ������ ���������� ������ ��� ����������� ����: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
			}
			if (end == rdo::runtime::RDOResource::CS_Create || end == rdo::runtime::RDOResource::CS_Erase || end == rdo::runtime::RDOResource::CS_NonExist)
			{
				rdoParser::g_error().error(convertor_end_pos, rdo::format(_T("������������ ������ ���������� ����� ��� ����������� ����: %s"), RDOPATPattern::StatusToStr(end).c_str()));
			}
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg, end);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Erase)
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdo::runtime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdo::runtime::RDOPatternOperation>()->addEraseBeginCalc(pCalc);
	}
	if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::CS_Erase)
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdo::runtime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdo::runtime::RDOPatternOperation>()->addEraseEndCalc(pCalc);
	}
}

void RDOPatternOperation::addRelResConvertBeginEnd(rbool trace_begin, CREF(LPConvertCmdList) cmd_begin, rbool trace_end, CREF(LPConvertCmdList) cmd_end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos, CREF(YYLTYPE) trace_begin_pos, CREF(YYLTYPE) trace_end_pos)
{
	if (cmd_begin)
	{
		m_convertorType = convert_begin;
		ASSERT(m_pCurrRelRes);
		addRelResConvert(trace_begin, cmd_begin, convertor_begin_pos, trace_begin_pos, m_pCurrRelRes->m_statusBegin);
		m_convertorType = convert_unknow;
	}
	if (cmd_end)
	{
		m_convertorType = convert_end;
		ASSERT(m_pCurrRelRes);
		addRelResConvert(trace_end, cmd_end, convertor_end_pos, trace_end_pos, m_pCurrRelRes->m_statusEnd);
		m_convertorType = convert_unknow;
	}
}

void RDOPatternOperation::addParamSetCalc(CREF(rdo::runtime::LPRDOCalc) pCalc)
{
	switch (m_convertorType)
	{
	case convert_begin: getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorBeginCalc(pCalc); break;
	case convert_end  : getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorEndCalc  (pCalc); break;
	default           : NEVER_REACH_HERE;
	}
}

tstring RDOPatternOperation::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status)
{
	switch (m_convertorType)
	{
	case convert_begin: return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� ������ (Convert_begin), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	case convert_end  : return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� ����� (Convert_end), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	default           : NEVER_REACH_HERE;
	}
	return tstring();
}

tstring RDOPatternOperation::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status)
{
	switch (m_convertorType)
	{
	case convert_begin: return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� ������ (Convert_begin), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	case convert_end  : return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� ����� (Convert_end), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	default           : NEVER_REACH_HERE;
	}
	return tstring();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternKeyboard
// --------------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPatternOperation(trace, name_src_info)
{
	m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternKeyboard>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResource
// --------------------------------------------------------------------------------
RDORelevantResource::RDORelevantResource(CREF(RDOParserSrcInfo) src_info, const int relResID, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd)
	: RDOParserSrcInfo      (src_info  )
	, m_relResID            (relResID   )
	, m_statusBegin         (statusBegin)
	, m_statusEnd           (statusEnd  )
	, m_alreadyHaveConverter(false      )
	, m_pChoiceFrom         (NULL       )
	, m_pChoiceOrder        (NULL       )
	, m_currentState        (stateNone  )
{}

RDORelevantResource::~RDORelevantResource()
{}

Context::FindResult RDORelevantResource::onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const
{
	ASSERT(pSwitchExpression);
	ASSERT(pValue           );

	ruint parNumb = getType()->getRTPParamNumber(pValue->value().getIdentificator());
	if (parNumb == RDORTPResType::UNDEFINED_PARAM)
	{
		RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� �������� �������: %s"), pValue->value().getIdentificator().c_str()));
	}

	//! ��������� ������������� ��� �� ������������������� (������ ��� Create) ��������� ���. ������� � ��� �� ����������
	LPRDORTPParam pParam = getType()->findRTPParam(pValue->value().getIdentificator());
	ASSERT(pParam);
	//! � ���������� ������
	if (m_currentState == RDORelevantResource::convertBegin && m_statusBegin == rdo::runtime::RDOResource::CS_Create)
	{
		if (!getParamSetList().find(pValue->value().getIdentificator()))
		{
			if (!pParam->getDefault()->defined())
			{
				RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� ��-��������� � ���� �������"), pValue->value().getIdentificator().c_str()));
			}
		}
	}
	//! � ���������� �����
	if (m_currentState == RDORelevantResource::convertEnd && m_statusEnd == rdo::runtime::RDOResource::CS_Create)
	{
		if (!getParamSetList().find(pValue->value().getIdentificator()))
		{
			if (!getParamSetList().find(pValue->value().getIdentificator()))
			{
				if (!pParam->getDefault()->defined())
				{
					RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� ��-��������� � ���� �������"), pValue->value().getIdentificator().c_str()));
				}
			}
		}
	}

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pParam->getTypeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcGetResourceParam>::create(pSwitchExpression->calc(), parNumb),
		pValue->src_info()
	);
	ASSERT(pExpression);

	return Context::FindResult(const_cast<PTR(RDORelevantResource)>(this), pExpression, pValue);
}

rdo::runtime::LPRDOCalc RDORelevantResource::getChoiceCalc()
{
	if (m_pChoiceFrom && m_pChoiceFrom->m_type == rdoParser::RDOPATChoiceFrom::ch_from)
	{
		return m_pChoiceFrom->m_pLogic->getCalc(rdo::runtime::RDOType::t_int);
	}
	return NULL;
}

rdo::runtime::LPRDOCalc RDORelevantResource::getSelectCalc()
{
	if (m_pChoiceOrder && m_pChoiceOrder->m_pArithm)
	{
		return m_pChoiceOrder->m_pArithm->createCalc(NULL);
	}
	return NULL;
}

rdo::runtime::RDOSelectResourceCalc::Type RDORelevantResource::getSelectType() const
{
	return m_pChoiceOrder ? m_pChoiceOrder->m_type : rdo::runtime::RDOSelectResourceCalc::order_empty;
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceDirect - �� ����� �������
// --------------------------------------------------------------------------------
rdo::runtime::LPRDOCalc RDORelevantResourceDirect::createPreSelectRelResCalc()
{
	rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID());
	ASSERT(pCalc);
	rdo::runtime::RDOSrcInfo srcInfo(src_info());
	srcInfo.setSrcText(rdo::format(_T("��������������� ����� ���. ������� %s"), src_text().c_str()));
	pCalc->setSrcInfo(srcInfo);
	return pCalc;
}

rdo::runtime::LPRDOCalc RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc(), getSelectCalc(), getSelectType());
	pCalc->setSrcInfo(m_pChoiceFrom->src_info());
	return pCalc;
}

rdo::runtime::LPRDOCalc RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return rdo::Factory<rdo::runtime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
}

rdo::runtime::LPIRDOSelectResourceCommon RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	rdo::runtime::LPRDOSelectResourceDirectCommonCalc pDirectCommonCalc = rdo::Factory<rdo::runtime::RDOSelectResourceDirectCommonCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
	rdo::runtime::LPIRDOSelectResourceCommon pSelectResourceCommon = pDirectCommonCalc.interface_cast<rdo::runtime::IRDOSelectResourceCommon>();
	ASSERT(pSelectResourceCommon);
	return pSelectResourceCommon;
}

LPRDORTPResType RDORelevantResourceDirect::getType() const 
{ 
	return m_pResource->getType(); 
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceByType - �� ����� ����
// --------------------------------------------------------------------------------
rdo::runtime::LPRDOCalc RDORelevantResourceByType::createPreSelectRelResCalc()
{
	if ((m_statusBegin != rdo::runtime::RDOResource::CS_Create) && (m_statusEnd != rdo::runtime::RDOResource::CS_Create))
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber());
		ASSERT(pCalc);
		rdo::runtime::RDOSrcInfo srcInfo(src_info());
		srcInfo.setSrcText(rdo::format(_T("��������������� ����� ���. ������� %s"), src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		return pCalc;
	}
	else
	{
		if (m_statusBegin == rdo::runtime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdo::runtime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
			ASSERT(pCalc);
			rdo::runtime::RDOSrcInfo srcInfo(src_info());
			srcInfo.setSrcText(rdo::format(_T("��������������� ����� ���. ������� ����� ��������� %s"), src_text().c_str()));
			pCalc->setSrcInfo(srcInfo);
			return pCalc;
		}
	}
}

rdo::runtime::LPRDOCalc RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ((m_statusBegin != rdo::runtime::RDOResource::CS_Create) && (m_statusEnd != rdo::runtime::RDOResource::CS_Create))
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType());
		pCalc->setSrcInfo(m_pChoiceFrom->src_info());
		return pCalc;
	}
	else
	{
		if (m_statusBegin == rdo::runtime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdo::runtime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
			ASSERT(pCalc);
			rdo::runtime::RDOSrcInfo srcInfo(src_info());
			srcInfo.setSrcText(rdo::format(_T("����� ��������� ���. ������� %s"), src_text().c_str()));
			pCalc->setSrcInfo(srcInfo);
			return pCalc;
		}
	}
}

rdo::runtime::LPRDOCalc RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ((m_statusBegin != rdo::runtime::RDOResource::CS_Create) && (m_statusEnd != rdo::runtime::RDOResource::CS_Create))
	{
		return rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
	}
	else
	{
		if (m_statusBegin == rdo::runtime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdo::runtime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			return rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
		}
	}
}

rdo::runtime::LPIRDOSelectResourceCommon RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	rdo::runtime::LPRDOSelectResourceByTypeCommonCalc pByTypeCommonCalc = rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCommonCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
	rdo::runtime::LPIRDOSelectResourceCommon pSelectResourceCommon = pByTypeCommonCalc.interface_cast<rdo::runtime::IRDOSelectResourceCommon>();
	ASSERT(pSelectResourceCommon);
	return pSelectResourceCommon;
}

CLOSE_RDO_PARSER_NAMESPACE
