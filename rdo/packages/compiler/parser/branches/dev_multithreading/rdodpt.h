/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDODPT_H_
#define _RDODPT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"

#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_logic.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/process/queue_depart.h"
#include "simulator/runtime/process/advance.h"
#include "simulator/runtime/process/seize_release.h"
#include "simulator/runtime/process/assign.h"
#include "simulator/runtime/process/terminate.h"
#include "simulator/runtime/process/generate.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
class RDODPTSome;
class RDODPTPrior;
class RDODPTSearchRuntime;
class RDOPROCProcess;
class RDOPROCGenerate;
class RDOPROCSeize;
class RDOPROCTerminate;
class RDOPROCAdvance;
class RDOPROCRelease;
struct parser_for_Seize;
struct parser_for_Queue;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

int  dptparse      (PTR(void) lexer);
int  dptlex        (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void dpterror      (PTR(char) message);

int  proc_rtp_parse(PTR(void) lexer);
int  proc_rtp_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_rtp_error(PTR(char) message);

int  proc_rss_parse(PTR(void) lexer);
int  proc_rss_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_rss_error(PTR(char) message);

int  proc_opr_parse(PTR(void) lexer);
int  proc_opr_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_opr_error(PTR(char) message);
/*
// --------------------------------------------------------------------------------
// -------------------- RDOParsEvent
// --------------------------------------------------------------------------------
CLASS(RDOParsEvent):
	INSTANCE_OF      (RDOParserSrcInfo)
AND INSTANCE_OF      (Context         )
AND IMPLEMENTATION_OF(IContextFind    )
{
	DECLARE_FACTORY(RDOParsEvent);
public:
	CREF(tstring)     name    () const { return src_info().src_text(); }
	CREF(LPIActivity) activity() const { return m_pActivity;           }
	LPRDOPATPattern   pattern () const { return m_pPattern;            }

	void addParam(CREF(RDOValue) param    );
	void endParam(CREF(YYLTYPE)  param_pos);

//	rbool setPrior(REF(LPRDOFUNArithm) pPrior);

protected:
	RDOParsEvent(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDOParsEvent();

	LPIActivity m_pActivity;

private:
	ruint           m_currParam;
	LPRDOPATPattern m_pPattern;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOParsEvent);
*/

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
CLASS(RDODPTActivity):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDODPTActivity);
public:
	CREF(tstring)     name    () const { return src_info().src_text(); }
	CREF(LPIActivity) activity() const { return m_pActivity;           }
	LPRDOPATPattern   pattern () const { return m_pPattern;            }

	void addParam(CREF(LPRDOValue) pParam   );
	void endParam(CREF(YYLTYPE)    param_pos);

	rbool setPrior(REF(LPRDOFUNArithm) pPrior);

protected:
	RDODPTActivity(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDODPTActivity();

	LPIActivity m_pActivity;

private:
	ruint           m_currParam;
	LPRDOPATPattern m_pPattern;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
public:
	void addHotKey(CREF(tstring) hotKey, CREF(YYLTYPE) hotkey_pos);

protected:
	RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
};
DECLARE_POINTER(RDODPTActivityHotKey);


// --------------------------------------------------------------------------------
// -------------------- RDODPTSomeActivity
// --------------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTSomeActivity);
private:
	RDODPTSomeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
class RDODPTPriorActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTPriorActivity);
private:
	RDODPTPriorActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
CLASS(RDODPTSome): public RDOLogic<rdoRuntime::RDODPTSome, RDODPTSomeActivity>
	AND INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(IContextFind)
{
DECLARE_FACTORY(RDODPTSome);

private:
	RDODPTSome(CREF(RDOParserSrcInfo) src_info, LPILogic pParent = NULL);

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTSome);

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
CLASS(RDODPTPrior): public RDOLogic<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>
	AND INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(IContextFind)
{
DECLARE_FACTORY(RDODPTPrior);

private:
	RDODPTPrior(CREF(RDOParserSrcInfo) src_info, LPILogic pParent = NULL);

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTPrior);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
DECLARE_FACTORY(RDODPTSearchActivity);
public:
	IDPTSearchActivity::ValueTime getValue() const { return m_value; }
	void                          setValue(IDPTSearchActivity::ValueTime value,
	                                       CREF(LPRDOFUNArithm)          pRuleCost);

	LPRDOFUNArithm getRuleCost() const { return m_pRuleCost; }

private:
	RDODPTSearchActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);

	IDPTSearchActivity::ValueTime  m_value;
	LPRDOFUNArithm                 m_pRuleCost;
};
DECLARE_POINTER(RDODPTSearchActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
CLASS(RDODPTSearch): public RDOLogic<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>
	AND INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(IContextFind)
{
DECLARE_FACTORY(RDODPTSearch);
public:
	void  setTermCondition(CREF(LPRDOFUNLogic)  pTermConditon = NULL) { m_pTermConditon = pTermConditon; }
	void  setEvaluateBy   (CREF(LPRDOFUNArithm) pEvalBy             ) { m_pEvalBy       = pEvalBy;       }
	void  setCompareTops  (rbool compTops                           ) { m_compTops      = compTops;      }
	void  end             ();
	rbool closed          () const                                    { return m_closed;                 }

private:
	RDODPTSearch(CREF(RDOParserSrcInfo) src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace = rdoRuntime::RDODPTSearchTrace::DPT_no_trace, LPILogic pParent = NULL);

	LPRDOFUNLogic                                 m_pTermConditon;
	LPRDOFUNArithm                                m_pEvalBy;
	LPILogic                                      m_pParent;
	rbool                                         m_compTops;
	rbool                                         m_closed;
	rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag  m_trace;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTSearch);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCOperator);

OBJECT(RDOPROCProcess) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPROCProcess);
public:
	typedef  std::list<LPRDOPROCProcess >  ProcessList;
	typedef  std::list<LPRDOPROCOperator>  BlockList;

	static tstring s_name_prefix;
	static tstring s_name_sufix;

	tstring name() const { return m_name; }

	void  end   ();
	rbool closed() const { return m_closed; }

	void            setCondition(CREF(LPRDOFUNLogic) pConditon = NULL) { m_pConditon =   pConditon;      }
	LPRDOFUNLogic   getConditon () const                               { return        m_pConditon;      }
	CREF(BlockList) getBlockList() const                               { return        m_blockList;      }
	LPRDORTPResType getTransacType()                                   { return m_transactType;   }

	rbool checkTransactType(CREF(tstring) name) const;

	rbool    setPrior   (REF(LPRDOFUNArithm)     pPrior  );
	void     insertBlock(CREF(LPRDOPROCOperator) pBlock  );
	void     insertChild(REF(LPRDOPROCProcess)   pProcess);

	LPILogic getRunTime () const { return m_pRuntime; }

protected:
	rbool             m_closed;
	LPRDOPROCProcess  m_pParentProcess;
	ProcessList       m_childProcessList;
	BlockList         m_blockList;
	LPILogic          m_pRuntime;

private:
	RDOPROCProcess(CREF(RDOParserSrcInfo) info, CREF(tstring) name, LPRDORTPResType transactType);

	LPRDOFUNLogic   m_pConditon;
	tstring         m_name;
	LPRDORTPResType m_transactType;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCOperator
// --------------------------------------------------------------------------------
OBJECT(RDOPROCOperator)
{
DECLARE_FACTORY(RDOPROCOperator);
public:
	virtual LPIPROCBlock getRuntimeBlock() const = 0;
protected:
	tstring          m_name;
	LPRDOPROCProcess m_pProcess;

	RDOPROCOperator(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
	virtual ~RDOPROCOperator();
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCGenerate);
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCGenerate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc, CREF(rdoRuntime::LPRDOCalc) pCreateAndGoOnTransactCalc);
};
DECLARE_POINTER(RDOPROCGenerate);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
class RDOPROCBlockForQueue: public RDOPROCOperator
{
protected:
	RDOPROCBlockForQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);

	//! m_parserForRuntime ������ ��� �������� ���������� � ��������� "���������" �������
	rdoRuntime::parser_for_Queue m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
class RDOPROCQueue: public RDOPROCBlockForQueue
{
DECLARE_FACTORY(RDOPROCQueue);
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
	void createRuntime();
	void setResource  (CREF(tstring) name);

protected:
	tstring      m_resourceName;
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCQueue);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
class RDOPROCDepart: public RDOPROCBlockForQueue
{
DECLARE_FACTORY(RDOPROCDepart);
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
	void createRuntime();
	void setResource  (CREF(tstring) name);

protected:
	tstring      m_resourceName;
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCDepart(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCDepart);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
class RDOPROCBlockForSeize: public RDOPROCOperator
{
protected:
	RDOPROCBlockForSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);

	//! m_parserForRuntime ������ ��� �������� ���������� � ��������� "���������" �������
	rdoRuntime::parser_for_Seize m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
DECLARE_FACTORY(RDOPROCSeize);
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
	void createRuntime();
	void addResource  (CREF(tstring) name);

protected:
	typedef std::list  <tstring>                      ResourceList;
	typedef std::vector<rdoRuntime::parser_for_Seize> ParserForRuntime;

	ResourceList     m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock     m_pRuntime;

private:
	RDOPROCSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCSeize);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize
{
DECLARE_FACTORY(RDOPROCRelease);
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
	void createRuntime();
	void addResource  (CREF(tstring) name);

protected:
	typedef std::list  <tstring>                      ResourceList;
	typedef std::vector<rdoRuntime::parser_for_Seize> ParserForRuntime;

	ResourceList     m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock     m_pRuntime;

private:
	RDOPROCRelease(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCRelease);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCAdvance);
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCAdvance(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCTerminate);

public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCTerminate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(ruint) term);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
class RDOPROCAssign: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCAssign);
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCAssign(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pValue, int resID, int paramID);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDODPT_H_
