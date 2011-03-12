/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdodpt.y
 * author   : �������� ����, ������ ������, ����� �������
 * date     : 20.02.2003
 * bref     : ��������� ����� �������� ������� DPT
 * indent   : 4T
 */

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type
%token RDO_permanent
%token RDO_Parameters
%token RDO_integer
%token RDO_real
%token RDO_End
%token RDO_temporary
%token RDO_IDENTIF
%token RDO_INT_CONST
%token RDO_REAL_CONST
%token RDO_such_as
%token RDO_dblpoint
%token RDO_Resources
%token RDO_trace
%token RDO_no_trace
%token RDO_IDENTIF_COLON
%token RDO_Constant
%token RDO_Body
%token RDO_Function
%token RDO_Type
%token RDO_algorithmic
%token RDO_table
%token RDO_list
%token RDO_Exist
%token RDO_Not_Exist
%token RDO_For_All
%token RDO_Not_For_All
%token RDO_neq
%token RDO_leq
%token RDO_geq
%token RDO_NoCheck
%token RDO_Calculate_if
%token RDO_or
%token RDO_and
%token RDO_Sequence
%token RDO_uniform
%token RDO_exponential
%token RDO_normal
%token RDO_by_hist
%token RDO_enumerative

%token RDO_Pattern
%token RDO_operation
%token RDO_rule
%token RDO_keyboard
%token RDO_Relevant_resources
%token RDO_Keep
%token RDO_Create
%token RDO_Erase
%token RDO_NonExist
%token RDO_IDENTIF_NoChange
%token RDO_Time
%token RDO_Choice
%token RDO_from
%token RDO_first
%token RDO_Convert_begin
%token RDO_Convert_end
%token RDO_Convert_rule
%token RDO_Convert_event
%token RDO_with_max
%token RDO_with_min
%token RDO_set
%token RDO_IDENTIF_NoChange_NoChange
%token RDO_Operations

%token RDO_Results
%token RDO_watch_par
%token RDO_watch_state
%token RDO_watch_quant
%token RDO_watch_value
%token RDO_get_value

%token RDO_Show_mode
%token RDO_Frame_number
%token RDO_Show_rate
%token RDO_Run_StartTime
%token RDO_Trace_StartTime
%token RDO_Trace_EndTime
%token RDO_Terminate_if
%token RDO_Break_point
%token RDO_Seed
%token RDO_NoShow
%token RDO_Monitor
%token RDO_Animation
%token RDO_NoChange

%token RDO_Decision_point
%token RDO_search
%token RDO_trace_stat
%token RDO_trace_tops
%token RDO_trace_all
%token RDO_Condition
%token RDO_Term_condition
%token RDO_Evaluate_by
%token RDO_Compare_tops
%token RDO_NO
%token RDO_YES
%token RDO_Activities
%token RDO_value_before
%token RDO_value_after
%token RDO_some
%token RDO_Process
%token RDO_SEIZE
%token RDO_GENERATE
%token RDO_TERMINATE
%token RDO_ADVANCE
%token RDO_RELEASE
%token RDO_if
%token RDO_for
%token RDO_Return
%token RDO_Break
%token RDO_result
%token RDO_CF
%token RDO_Priority
%token RDO_prior
%token RDO_Parent
%token RDO_PlusEqual
%token RDO_MinusEqual
%token RDO_MultiplyEqual
%token RDO_DivideEqual
%token RDO_array
%token RDO_event
%token RDO_Planning
%token RDO_else
%token RDO_IncrEqual
%token RDO_DecrEqual
%token RDO_Stopping
%token RDO_Start
%token RDO_Stop

%token RDO_Frame
%token RDO_Show_if
%token RDO_Back_picture
%token RDO_Show
%token RDO_frm_cell
%token RDO_text
%token RDO_bitmap
%token RDO_s_bmp
%token RDO_rect
%token RDO_r_rect
%token RDO_line
%token RDO_ellipse
%token RDO_triang
%token RDO_active
%token RDO_ruler
%token RDO_space
%token RDO_color_transparent
%token RDO_color_last
%token RDO_color_white
%token RDO_color_black
%token RDO_color_red
%token RDO_color_green
%token RDO_color_blue
%token RDO_color_cyan
%token RDO_color_magenta
%token RDO_color_yellow
%token RDO_color_gray

%token RDO_IDENTIF_RELRES
%token RDO_typedef
%token RDO_enum

%token RDO_STRING_CONST
%token RDO_STRING_CONST_BAD
%token RDO_IDENTIF_BAD
%token RDO_Select
%token RDO_Size
%token RDO_Empty
%token RDO_not
%token RDO_UMINUS
%token RDO_string
%token RDO_bool
%token RDO_BOOL_CONST
%token RDO_Fuzzy
%token RDO_Fuzzy_Term
%token RDO_eq
%token RDO_External_Model
%token RDO_QUEUE
%token RDO_DEPART
%token RDO_ASSIGN

%{
// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdorss.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start dpt_main

%%

// ----------------------------------------------------------------------------
// ---------- DPT
// ----------------------------------------------------------------------------
dpt_main
	: /* empty */
	| dpt_main dpt_search_end
	| dpt_main dpt_some_end
	| dpt_main dpt_prior_end
	| dpt_main dpt_free_end
	| error
	{
		PARSER->error().error(@1, _T("��������� �������� ����� ��� ���������� ����� �����������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- DPTSearch
// ----------------------------------------------------------------------------
dpt_search_trace
	: /* empty */
	{
		$$ = rdoRuntime::RDODPTSearchTrace::DPT_no_trace;
	}
	| RDO_no_trace
	{
		$$ = rdoRuntime::RDODPTSearchTrace::DPT_no_trace;
	}
	| RDO_trace
	{
		PARSER->error().error(@1, _T("������ ������� ����������� �� ������������ � ����� ���� search"));
	}
	| RDO_trace_stat
	{
		$$ = rdoRuntime::RDODPTSearchTrace::DPT_trace_stat;
	}
	| RDO_trace_tops
	{
		$$ = rdoRuntime::RDODPTSearchTrace::DPT_trace_tops;
	}
	| RDO_trace_all
	{
		$$ = rdoRuntime::RDODPTSearchTrace::DPT_trace_all;
	}
	;

dpt_search_parent
	: /* empty */
	{
		$$ = 0;
	}
	| RDO_Parent RDO_IDENTIF
	{
		$$ = $2;
	}
	| RDO_Parent error
	{
		PARSER->error().error(@1, _T("������ � ����� ������������ �����"));
	}
	;

dpt_search_begin
	: RDO_Decision_point RDO_IDENTIF_COLON RDO_search dpt_search_parent dpt_search_trace
	{
		PTR(RDOValue)  name        = P_RDOVALUE($2);
		PTR(RDOValue)  parent_name = P_RDOVALUE($4);
		LPRDODPTSearch pDPTSearch;
		if (parent_name)
		{
			LPRDODPTPrior  pParentDPTPrior  = PARSER->findDPTPrior (parent_name->value().getIdentificator());
			LPRDODPTSearch pParentDPTSearch = PARSER->findDPTSearch(parent_name->value().getIdentificator());
			LPRDODPTSome   pParentDPTSome   = PARSER->findDPTSome  (parent_name->value().getIdentificator());
			if (!pParentDPTPrior && !pParentDPTSearch && !pParentDPTSome)
			{
				PARSER->error().error(@1, rdo::format(_T("�� ������� ���������� ����� %s"), parent_name->value().getIdentificator().c_str()));
			}
			if (pParentDPTSearch)
			{
				PARSER->error().error(@1, _T("����� �������� ������� ���� search ����� ��������� ���� ���������� ���� rule � �� ����� ���� ������� � �������� ������������ �����"));
			}
			else if (pParentDPTPrior)
			{
				LPILogic pParent = pParentDPTPrior->getLogic();
				ASSERT(pParent);
				pDPTSearch = rdo::Factory<RDODPTSearch>::create(name->src_info(), *reinterpret_cast<PTR(rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag)>(&$5), pParent);
			}
			else if (pParentDPTSome)
			{
				LPILogic pParent = pParentDPTSome->getLogic();
				ASSERT(pParent);
				pDPTSearch = rdo::Factory<RDODPTSearch>::create(name->src_info(), *reinterpret_cast<PTR(rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag)>(&$5), pParent);
			}
		}
		else
		{
			pDPTSearch = rdo::Factory<RDODPTSearch>::create(name->src_info(), *reinterpret_cast<PTR(rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag)>(&$5));
		}
		ASSERT(pDPTSearch);
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| RDO_Decision_point RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("��������� ��� �����"));
	}
	| RDO_Decision_point RDO_IDENTIF error
	{
		PARSER->error().error(@2, _T("��������� ���������"));
	}
	| RDO_Decision_point error
	{
		PARSER->error().error(@1, @2, _T("����� ��������� ����� $Decision_point ��������� ��� �����"));
	}
	;

dpt_search_condition
	: dpt_search_begin RDO_Condition fun_logic
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_begin RDO_Condition RDO_NoCheck
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCondition();
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_begin RDO_Condition error
	{
		PARSER->error().error(@2, @3, _T("����� ��������� ����� $Condition ��������� ������� ������ ������ (��������� �������)"));
	}
	| dpt_search_begin error
	{
		PARSER->error().error(@2, _T("��������� �������� ����� $Condition"));
	}
	;

dpt_search_prior
	: dpt_search_condition
	| dpt_search_condition RDO_Priority fun_arithm
	{
		if (!PARSER->getLastDPTSearch()->setPrior(PARSER->stack().pop<RDOFUNArithm>($3)))
		{
			PARSER->error().error(@3, _T("����� �������� ������� ���� �� ����� ����� ���������"));
		}
	}
	| dpt_search_condition RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ���������� ����� �������� �������"))
	}
	| dpt_search_condition error
	{
		PARSER->error().error(@1, @2, _T("��������� �������� ����� $Priority"))
	}
	;

dpt_search_term
	: dpt_search_prior RDO_Term_condition fun_logic
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setTermCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_prior RDO_Term_condition RDO_NoCheck
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setTermCondition();
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_prior RDO_Term_condition error
	{
		PARSER->error().error(@2, @3, _T("����� ��������� ����� $Term_condition ��������� ������� ��������� ������ (�������� �������)"));
	}
	| dpt_search_prior error
	{
		PARSER->error().error(@2, _T("��������� �������� ����� $Term_condition"));
	}
	;

dpt_search_evaluate
	: dpt_search_term RDO_Evaluate_by fun_arithm
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setEvaluateBy(PARSER->stack().pop<RDOFUNArithm>($3));
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_term RDO_Evaluate_by error
	{
		PARSER->error().error(@2, @3, _T("����� ��������� ����� $Evaluate_by ��������� ��������� �������, ��������, 0 ��� ������ � ������"));
	}
	| dpt_search_term error
	{
		PARSER->error().error(@2, _T("��������� �������� ����� $Evaluate_by"));
	}
	;

dp_searcht_compare
	: dpt_search_evaluate RDO_Compare_tops '=' RDO_NO
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCompareTops(false);
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_evaluate RDO_Compare_tops '=' RDO_YES
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCompareTops(true);
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_evaluate RDO_Compare_tops '=' error
	{
		PARSER->error().error(@3, @4, _T("��������� ����� ����������� ���������� ������ (YES ��� NO)"));
	}
	| dpt_search_evaluate RDO_Compare_tops error
	{
		PARSER->error().error(@2, @3, _T("��������� ���� ���������"));
	}
	| dpt_search_evaluate error
	{
		PARSER->error().error(@2, _T("��������� �������� ����� $Compare_tops"));
	}
	;

dpt_search_descr_param
	: /* empty */
	| dpt_search_descr_param '*'
	{
		PARSER->getLastDPTSearch()->getLastActivity()->addParam(RDOValue(RDOParserSrcInfo(@2, _T("*"))));
	}
	| dpt_search_descr_param fun_arithm
	{
		PARSER->getLastDPTSearch()->getLastActivity()->addParam(PARSER->stack().pop<RDOFUNArithm>($2)->value());
	}
	| dpt_search_descr_param error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ��������� �������"))
	}
	;

dpt_search_descr_value
	: RDO_value_before fun_arithm
	{
		LPRDODPTSearch pDPTSearch = PARSER->getLastDPTSearch();
		ASSERT(pDPTSearch);
		pDPTSearch->getLastActivity()->setValue(IDPTSearchActivity::vt_before, PARSER->stack().pop<RDOFUNArithm>($2), @1);
	}
	| RDO_value_after fun_arithm
	{
		LPRDODPTSearch pDPTSearch = PARSER->getLastDPTSearch();
		ASSERT(pDPTSearch);
		pDPTSearch->getLastActivity()->setValue(IDPTSearchActivity::vt_after, PARSER->stack().pop<RDOFUNArithm>($2), @1);
	}
	| RDO_value_before error
	{
		PARSER->error().error(@1, @2, _T("������ � �������������� ���������"));
	}
	| RDO_value_after error
	{
		PARSER->error().error(@1, @2, _T("������ � �������������� ���������"));
	}
	;

dpt_search_name
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDODPTSearch pDPTSearch = PARSER->getLastDPTSearch();
		ASSERT(pDPTSearch);
		PTR(RDOValue) name    = P_RDOVALUE($1);
		PTR(RDOValue) pattern = P_RDOVALUE($2);
		LPRDODPTActivity pActivity = pDPTSearch->addNewActivity(name->src_info(), pattern->src_info());
		ASSERT(pActivity);
		$$ = PARSER->stack().push(pActivity);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("��������� ��� �������"));
	}
	| RDO_IDENTIF
	{
		PARSER->error().error(@1, _T("��������� ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� ��� ����������"));
	}
	;

dpt_searcht_activity
	: /* empty */
	| dpt_searcht_activity dpt_search_name dpt_search_descr_param dpt_search_descr_value
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTActivity>($2);
		ASSERT(pActivity)
		pActivity->endParam(@3);
	}
	| dpt_searcht_activity dpt_search_name dpt_search_descr_param error
	{
		PARSER->error().error(@3, @4, _T("��������� �������� ����� value before ��� value after � ��������� ���������� �������"));
	}
	;

dpt_search_header
	: dp_searcht_compare RDO_Activities dpt_searcht_activity
	| dp_searcht_compare error
	{
		PARSER->error().error(@1, @2, _T("����� ������ ����������� ���������� ������ ��������� �������� ����� $Activities"));
	}
	;

dpt_search_end
	: dpt_search_header RDO_End
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->end();
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_header
	{
		PARSER->error().error(@1, _T("��������� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- DPTSome
// ----------------------------------------------------------------------------
dpt_some_trace
	: /* empty */
	{
		$$ = 1;
	}
	| RDO_no_trace
	{
		$$ = 1;
	}
	| RDO_trace
	{
		$$ = 2;
	}
	| RDO_trace_stat
	{
		PARSER->error().error(@1, _T("������ ������� ����������� �� ������������ � ����� ���� some"));
	}
	| RDO_trace_tops
	{
		PARSER->error().error(@1, _T("������ ������� ����������� �� ������������ � ����� ���� some"));
	}
	| RDO_trace_all
	{
		PARSER->error().error(@1, _T("������ ������� ����������� �� ������������ � ����� ���� some"));
	}
	;

dpt_some_parent
	: /* empty */
	{
		$$ = 0;
	}
	| RDO_Parent RDO_IDENTIF
	{
		$$ = $2;
	}
	| RDO_Parent error
	{
		PARSER->error().error(@1, _T("������ � ����� ������������ �����"));
	}
	;

dpt_some_begin
	: RDO_Decision_point RDO_IDENTIF_COLON RDO_some dpt_some_parent dpt_some_trace
	{
		// TODO: � ��� ������� ����������� ��� some ?
		PTR(RDOValue) name        = P_RDOVALUE($2);
		PTR(RDOValue) parent_name = P_RDOVALUE($4);
		LPRDODPTSome  pDPTSome;
		if (parent_name)
		{
			LPRDODPTPrior  pParentDPTPrior  = PARSER->findDPTPrior (parent_name->value().getIdentificator());
			LPRDODPTSearch pParentDPTSearch = PARSER->findDPTSearch(parent_name->value().getIdentificator());
			LPRDODPTSome   pParentDPTSome   = PARSER->findDPTSome  (parent_name->value().getIdentificator());
			if (!pParentDPTPrior && !pParentDPTSearch && !pParentDPTSome)
			{
				PARSER->error().error(@1, rdo::format(_T("�� ������� ���������� ����� %s"), parent_name->value().getIdentificator().c_str()));
			}
			if (pParentDPTSearch)
			{
				PARSER->error().error(@5, @1, _T("����� �������� ������� ���� search ����� ��������� ���� ���������� ���� rule � �� ����� ���� ������� � �������� ������������ �����"));
			}
			else if (pParentDPTPrior)
			{
				LPILogic pParentLogic = pParentDPTPrior->getLogic();
				ASSERT(pParentLogic);
				pDPTSome = rdo::Factory<RDODPTSome>::create(name->src_info(), pParentLogic);
			}
			else if (pParentDPTSome)
			{
				LPILogic pParentLogic = pParentDPTSome->getLogic();
				ASSERT(pParentLogic);
				pDPTSome = rdo::Factory<RDODPTSome>::create(name->src_info(), pParentLogic);
			}
		}
		else
		{
			pDPTSome = rdo::Factory<RDODPTSome>::create(name->src_info());
		}
		ASSERT(pDPTSome);
		$$ = PARSER->stack().push(pDPTSome);
	}
	;

dpt_some_condition
	: dpt_some_begin RDO_Condition fun_logic
	{
		LPRDODPTSome pDPTSome = PARSER->stack().pop<RDODPTSome>($1);
		ASSERT(pDPTSome);
		pDPTSome->setCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pDPTSome);
	}
	| dpt_some_begin RDO_Condition RDO_NoCheck
	{
		LPRDODPTSome pDPTSome = PARSER->stack().pop<RDODPTSome>($1);
		ASSERT(pDPTSome);
		pDPTSome->setCondition();
		$$ = PARSER->stack().push(pDPTSome);
	}
	| dpt_some_begin RDO_Condition error
	{
		PARSER->error().error(@2, @3, _T("����� ��������� ����� $Condition ��������� ������� ������� �����"));
	}
	| dpt_some_begin
	{
		LPRDODPTSome pDPTSome = PARSER->stack().pop<RDODPTSome>($1);
		ASSERT(pDPTSome);
		pDPTSome->setCondition();
		$$ = PARSER->stack().push(pDPTSome);
	}
	;

dpt_some_prior
	: dpt_some_condition
	| dpt_some_condition RDO_Priority fun_arithm
	{
		if (!PARSER->getLastDPTSome()->setPrior(PARSER->stack().pop<RDOFUNArithm>($3)))
		{
			PARSER->error().error(@3, _T("����� �������� ������� ���� �� ����� ����� ���������"));
		}
	}
	| dpt_some_condition RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ���������� ����� �������� �������"))
	}
	| dpt_some_condition error
	{
		PARSER->error().error(@1, @2, _T("��������� �������� ����� $Priority"))
	}
	;

dpt_some_name
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDODPTSome pDPTSome = PARSER->getLastDPTSome();
		ASSERT(pDPTSome);
		PTR(RDOValue) name    = P_RDOVALUE($1);
		PTR(RDOValue) pattern = P_RDOVALUE($2);
		LPRDODPTActivity pActivity = pDPTSome->addNewActivity(name->src_info(), pattern->src_info());
		ASSERT(pActivity);
		$$ = PARSER->stack().push(pActivity);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("��������� ��� �������"));
	}
	| RDO_IDENTIF
	{
		PARSER->error().error(@1, _T("��������� ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� ��� ����������"));
	}
	;

dpt_some_descr_keyb
	: /* empty */
	| dpt_some_descr_keyb '+' RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTSome()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = P_RDOVALUE($3)->value().getString();
		pActivityHotKey->addHotKey(key, @3);
	}
	| RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTSome()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = P_RDOVALUE($1)->value().getString();
		pActivityHotKey->addHotKey(key, @1);
	}
	;

dpt_some_descr_param
	: /* empty */
	| dpt_some_descr_param '*'
	{
		PARSER->getLastDPTSome()->getLastActivity()->addParam(RDOValue(RDOParserSrcInfo(@2, _T("*"))));
	}
	| dpt_some_descr_param fun_arithm
	{
		PARSER->getLastDPTSome()->getLastActivity()->addParam(PARSER->stack().pop<RDOFUNArithm>($2)->value());
	}
	| dpt_some_descr_param error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ��������� �������"))
	}
	;

dpt_some_activity
	: /* empty */
	| dpt_some_activity dpt_some_name dpt_some_descr_keyb dpt_some_descr_param
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTSomeActivity>($2);
		ASSERT(pActivity);
		pActivity->endParam(@3);
		$$ = PARSER->stack().push(pActivity);
	}
	;

dpt_some_header
	: dpt_some_prior RDO_Activities dpt_some_activity
	| dpt_some_prior error
	{
		PARSER->error().error(@1, @2, _T("��������� �������� ����� $Activities"));
	}
	;

dpt_some_end
	: dpt_some_header RDO_End
	{
		LPRDODPTSome pDPTSome = PARSER->stack().pop<RDODPTSome>($1);
		ASSERT(pDPTSome);
		pDPTSome->end();
		$$ = PARSER->stack().push(pDPTSome);
	}
	| dpt_some_header
	{
		PARSER->error().error(@1, _T("��������� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- DPT Prior
// ----------------------------------------------------------------------------
dpt_prior_trace
	: /* empty */
	{
		$$ = 1;
	}
	| RDO_no_trace
	{
		$$ = 1;
	}
	| RDO_trace
	{
		$$ = 2;
	}
	| RDO_trace_stat
	{
		PARSER->error().error(@1, _T("������ ������� ����������� �� ������������ � ����� ���� prior"));
	}
	| RDO_trace_tops
	{
		PARSER->error().error(@1, _T("������ ������� ����������� �� ������������ � ����� ���� prior"));
	}
	| RDO_trace_all
	{
		PARSER->error().error(@1, _T("������ ������� ����������� �� ������������ � ����� ���� prior"));
	}
	;

dpt_prior_parent
	: /* empty */
	{
		$$ = 0;
	}
	| RDO_Parent RDO_IDENTIF
	{
		$$ = $2;
	}
	| RDO_Parent error
	{
		PARSER->error().error(@1, _T("������ � ����� ������������ �����"));
	}
	;

dpt_prior_begin
	: RDO_Decision_point RDO_IDENTIF_COLON RDO_prior dpt_prior_parent dpt_prior_trace
	{
		// TODO: � ��� ������� ����������� ��� prior ?
		PTR(RDOValue) name        = P_RDOVALUE($2);
		PTR(RDOValue) parent_name = P_RDOVALUE($4);
		LPRDODPTPrior pDPTPrior;
		if (parent_name)
		{
			LPRDODPTPrior  pParentDPTPrior  = PARSER->findDPTPrior (parent_name->value().getIdentificator());
			LPRDODPTSearch pParentDPTSearch = PARSER->findDPTSearch(parent_name->value().getIdentificator());
			LPRDODPTSome   pParentDPTSome   = PARSER->findDPTSome  (parent_name->value().getIdentificator());
			if (!pParentDPTPrior && !pParentDPTSome && !pParentDPTSearch)
			{
				PARSER->error().error(@1, rdo::format(_T("�� ������� ���������� ����� %s"), parent_name->value().getIdentificator().c_str()));
			}
			if (pParentDPTSearch)
			{
				PARSER->error().error(@5, @1, _T("����� �������� ������� ���� search ����� ��������� ���� ���������� ���� rule � �� ����� ���� ������� � �������� ������������ �����"));
			}
			else if (pParentDPTPrior)
			{
				LPILogic pParentLogic = pParentDPTPrior->getLogic();
				ASSERT(pParentLogic);
				pDPTPrior = rdo::Factory<RDODPTPrior>::create(name->src_info(), pParentLogic);
			}
			else if (pParentDPTSome)
			{
				LPILogic pParentLogic = pParentDPTSome->getLogic();
				ASSERT(pParentLogic);
				pDPTPrior = rdo::Factory<RDODPTPrior>::create(name->src_info(), pParentLogic);
			}
		}
		else
		{
			pDPTPrior = rdo::Factory<RDODPTPrior>::create(name->src_info());
		}
		ASSERT(pDPTPrior);
		$$ = PARSER->stack().push(pDPTPrior);
	}
	;

dpt_prior_condition
	: dpt_prior_begin RDO_Condition fun_logic
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->setCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pDPTPrior);
	}
	| dpt_prior_begin RDO_Condition RDO_NoCheck
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->setCondition();
		$$ = PARSER->stack().push(pDPTPrior);
	}
	| dpt_prior_begin RDO_Condition error
	{
		PARSER->error().error(@2, @3, _T("����� ��������� ����� $Condition ��������� ������� ������� �����"));
	}
	| dpt_prior_begin
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->setCondition();
		$$ = PARSER->stack().push(pDPTPrior);
	}
	;

dpt_prior_prior
	: dpt_prior_condition
	| dpt_prior_condition RDO_Priority fun_arithm
	{
		if (!PARSER->getLastDPTPrior()->setPrior(PARSER->stack().pop<RDOFUNArithm>($3)))
		{
			PARSER->error().error(@3, _T("����� �������� ������� ���� �� ����� ����� ���������"));
		}
	}
	| dpt_prior_condition RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ���������� ����� �������� �������"))
	}
	| dpt_some_condition error
	{
		PARSER->error().error(@1, @2, _T("��������� �������� ����� $Priority"))
	}
	;

dpt_prior_name
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDODPTPrior pDPTPrior = PARSER->getLastDPTPrior();
		ASSERT(pDPTPrior);
		PTR(RDOValue) name    = P_RDOVALUE($1);
		PTR(RDOValue) pattern = P_RDOVALUE($2);
		LPRDODPTActivity pActivity = pDPTPrior->addNewActivity(name->src_info(), pattern->src_info());
		ASSERT(pActivity);
		$$ = PARSER->stack().push(pActivity);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("��������� ��� �������"));
	}
	| RDO_IDENTIF
	{
		PARSER->error().error(@1, _T("��������� ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� ��� ����������"));
	}
	;

dpt_prior_descr_keyb
	: /* empty */
	| dpt_prior_descr_keyb '+' RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTPrior()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = P_RDOVALUE($3)->value().getString();
		pActivityHotKey->addHotKey(key, @3);
	}
	| RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTPrior()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = P_RDOVALUE($1)->value().getString();
		pActivityHotKey->addHotKey(key, @1);
	}
	;

dpt_prior_descr_param
	: /* empty */
	| dpt_prior_descr_param '*'
	{
		PARSER->getLastDPTPrior()->getLastActivity()->addParam(RDOValue(RDOParserSrcInfo(@2, _T("*"))));
	}
	| dpt_prior_descr_param fun_arithm
	{
		PARSER->getLastDPTPrior()->getLastActivity()->addParam(PARSER->stack().pop<RDOFUNArithm>($2)->value());
	}
	| dpt_prior_descr_param error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ��������� �������"))
	}
	;

dpt_prior_activ_prior
	: /* empty */
	| RDO_CF '=' fun_arithm
	{
		if (!PARSER->getLastDPTPrior()->getLastActivity()->setPrior(PARSER->stack().pop<RDOFUNArithm>($3)))
		{
			PARSER->error().error(@3, _T("���������� �� ����� ����� ���������"));
		}
	}
	| RDO_CF '=' error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ���������� ����������"))
	}
	| RDO_CF error
	{
		PARSER->error().error(@1, @2, _T("������: ��������� ���� ���������"))
	}
	;

dpt_prior_activity
	: /* empty */
	| dpt_prior_activity dpt_prior_name dpt_prior_descr_keyb dpt_prior_descr_param dpt_prior_activ_prior
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTSomeActivity>($2);
		ASSERT(pActivity);
		pActivity->endParam(@3);
		$$ = PARSER->stack().push(pActivity);
	}
	;

dpt_prior_header
	: dpt_prior_prior RDO_Activities dpt_prior_activity
	| dpt_prior_prior error
	{
		PARSER->error().error(@1, @2, _T("��������� �������� ����� $Activities"));
	}
	;

dpt_prior_end
	: dpt_prior_header RDO_End
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->end();
		$$ = PARSER->stack().push(pDPTPrior);
	}
	| dpt_prior_header
	{
		PARSER->error().error(@1, _T("��������� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- DPT Free
// ----------------------------------------------------------------------------
dpt_free_prior
	: dpt_free_header
	| RDO_Priority fun_arithm dpt_free_header
	{
		if (!PARSER->getLastDPTFree()->setPrior(PARSER->stack().pop<RDOFUNArithm>($2)))
		{
			PARSER->error().error(@3, _T("����� �������� ������� ���� �� ����� ����� ���������"));
		}
	}
	| RDO_Priority error dpt_free_header
	{
		PARSER->error().error(@1, @2, _T("������ �������� ���������� ����� �������� �������"))
	}
	| error dpt_free_header
	{
		PARSER->error().error(@1, @2, _T("��������� �������� ����� $Priority"))
	}
	;

dpt_free_header
	: RDO_Activities
	{
		LPRDODPTFree pDPTFree = rdo::Factory<RDODPTFree>::create(@1);
		ASSERT(pDPTFree);
		$$ = PARSER->stack().push(pDPTFree);
	}
	;

dpt_free_activity
	: /* empty */
	| dpt_free_activity dpt_free_activity_name dpt_free_activity_param dpt_free_activity_keys
	;

dpt_free_activity_name
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDODPTFree pDPTFree = PARSER->getLastDPTFree();
		ASSERT(pDPTFree);
		PTR(RDOValue) name    = P_RDOVALUE($1);
		PTR(RDOValue) pattern = P_RDOVALUE($2);
		LPRDODPTActivity pActivity = pDPTFree->addNewActivity(name->src_info(), pattern->src_info());
		ASSERT(pActivity);
		$$ = PARSER->stack().push(pActivity);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("��������� ��� �������"));
	}
	| RDO_IDENTIF
	{
		PARSER->error().error(@1, _T("��������� ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� ��� ����������"));
	}
	;

dpt_free_activity_param
	: /* empty */
	| dpt_free_activity_param '*'
	{
		PARSER->getLastDPTFree()->getLastActivity()->addParam(RDOValue(RDOParserSrcInfo(@2, _T("*"))));
	}
	| dpt_free_activity_param fun_arithm
	{
		PARSER->getLastDPTFree()->getLastActivity()->addParam(PARSER->stack().pop<RDOFUNArithm>($2)->value());
	}
	| dpt_free_activity_param error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ��������� �������"))
	}
	;

dpt_free_activity_keys
	: /* empty */
	| dpt_free_activity_keys RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTFree()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = P_RDOVALUE($2)->value().getString();
		pActivityHotKey->addHotKey(key, @2);
	}
	| dpt_free_activity_keys '+' RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTFree()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = P_RDOVALUE($3)->value().getString();
		pActivityHotKey->addHotKey(key, @3);
	}
	;

dpt_free_end
	: dpt_free_prior dpt_free_activity RDO_End
	| dpt_free_header error
	{
		PARSER->error().error(@1, _T("��������� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ����� ��������� ������ ��� ���� �������� ���
// ----------------------------------------------------------------------------
// ---------- ���������� ���������
// ----------------------------------------------------------------------------
fun_logic_eq
	: RDO_eq { $$ = RDO_eq; }
	;

fun_logic
	: fun_arithm fun_logic_eq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator ==(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_neq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator !=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '<' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '>' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_leq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_geq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_and fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator &&(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_or fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator ||(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		LPRDOFUNLogic pResult = rdo::Factory<RDOFUNLogic>::create(pArithm);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_group
	| fun_select_logic
	| '[' fun_logic ']'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("[") + pLogic->src_text() + _T("]"));
		$$ = PARSER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("(") + pLogic->src_text() + _T(")"));
		$$ = PARSER->stack().push(pLogic);
	}
	| RDO_not fun_logic
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not();
		ASSERT(pLogicNot);
		pLogicNot->setSrcPos (@1, @2);
		pLogicNot->setSrcText(_T("not ") + pLogic->src_text());
		$$ = PARSER->stack().push(pLogicNot);
	}
	| '[' fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| '(' fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������������� ���������
// ----------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| param_array_value					 { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| fun_arithm '+' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator +(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '-' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator -(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '*' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator *(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '/' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator /(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm_func_call
	| fun_select_arithm
	| '(' fun_arithm ')'
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		pArithm->setSrcPos (@1, @3);
		pArithm->setSrcText(_T("(") + pArithm->src_text() + _T(")"));
		$$ = PARSER->stack().push(pArithm);
	}
	| '-' fun_arithm %prec RDO_UMINUS
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		RDOParserSrcInfo info;
		info.setSrcPos (@1, @2);
		info.setSrcText(_T("-") + pArithm->src_text());
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOValue(pArithm->type(), info), rdo::Factory<rdoRuntime::RDOCalcUMinus>::create(pArithm->createCalc())));
	}
	;

param_array_value
	: '[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		$$ = (int)PARSER->addValue(new RDOValue(pArrayValue->getRArray(), pArrayValue->getArrayType(), RDOParserSrcInfo(@2)));
	}
	|'[' array_item error {
		PARSER->error().error(@2, _T("������ ������ ����������� �������"));
	}
	;

array_item
	: param_value
	{
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).type(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		pArrayValue->insertItem(RDOVALUE($1));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($3));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($2));
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), RDOVALUE($2)->getAsString().c_str()));
	}
	;

param_value
	: RDO_INT_CONST
	{
		$$ = $1;
	}
	| RDO_REAL_CONST {
		$$ = $1;
	}
	| RDO_STRING_CONST {
		$$ = $1;
	}
	| RDO_IDENTIF {
		$$ = $1;
	}
	| RDO_BOOL_CONST {
		$$ = $1;
	}
	| param_array_value {
		$$ = $1;
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')'
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @3);
		pFunParams->setSrcText(funName + _T("()"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')'
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($3);
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pFunParams->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("������ � ���������� �������"));
	}
	;

fun_arithm_func_call_pars
	: fun_arithm
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars ',' fun_arithm
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($1);
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pFunParams->src_text() + _T(", ") + pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars error
	{
		PARSER->error().error(@2, _T("������ � �������������� ���������"));
	}
	| fun_arithm_func_call_pars ',' error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ��������� ���������
// ----------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue) type_name = P_RDOVALUE($3);
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info()));
	}
	| fun_group_keyword '(' error
	{
		PARSER->error().error(@3, _T("��������� ��� ����"));
	}
	| fun_group_keyword error
	{
		PARSER->error().error(@1, _T("����� ����� ������� ��������� ������������� ������"));
	}
	;

fun_group
	: fun_group_header fun_logic ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		LPRDOFUNLogic      pLogic    = PARSER->stack().pop<RDOFUNLogic>     ($2);
		ASSERT(pGroupFun);
		ASSERT(pLogic   );
		pGroupFun->setSrcPos(@1, @3);
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pLogic));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		ASSERT(pGroupFun);
		pGroupFun->setSrcPos(@1, @3);
		LPRDOFUNLogic pTrueLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(1), false);
		ASSERT(pTrueLogic);
		pTrueLogic->setSrcPos (@2);
		pTrueLogic->setSrcText(_T("NoCheck"));
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
	}
	| fun_group_header fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_group_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_group_header error
	{
		PARSER->error().error(@1, @2, _T("������ � ���������� ���������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue)  type_name = P_RDOVALUE($3);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(type_name->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + type_name->value().getIdentificator() + _T(": "));
		$$ = PARSER->stack().push(pSelect);
	}
	| RDO_Select '(' error
	{
		PARSER->error().error(@3, _T("��������� ��� ����"));
	}
	| RDO_Select error
	{
		PARSER->error().error(@1, _T("��������� ������������� ������"));
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + _T(")"));
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo logicInfo(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + logicInfo.src_text() + _T(")"));
		rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		ASSERT(pCalc);
		LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pCalc, true);
		ASSERT(pLogic);
		pLogic->setSrcInfo(logicInfo);
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_select_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_select_header error
	{
		PARSER->error().error(@1, @2, _T("������ � ���������� ���������"));
	}
	;

fun_select_keyword
	: RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_select_logic
	: fun_select_body '.' fun_select_keyword '(' fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($5);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcPos(@1, @6);
		LPRDOFUNLogic pLogicSelect = pSelect->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, pLogic);
		ASSERT(pLogicSelect);
		$$ = PARSER->stack().push(pLogicSelect);
	}
	| fun_select_body '.' fun_select_keyword '(' error
	{
		PARSER->error().error(@4, @5, _T("������ � ���������� ���������"));
	}
	| fun_select_body '.' fun_select_keyword error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, _T("Empty()"));
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = PARSER->stack().push(pLogic);
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		PARSER->error().error(@4, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Empty error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' error
	{
		PARSER->error().error(@2, @3, _T("��������� ����� ������ ��������"));
	}
	| fun_select_body error
	{
		PARSER->error().error(@1, _T("��������� '.' (�����) ��� ������ ������ ������ ��������"));
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, _T("Size()"));
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Size error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		PARSER->error().error(@4, _T("��������� ������������� ������"));
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE
