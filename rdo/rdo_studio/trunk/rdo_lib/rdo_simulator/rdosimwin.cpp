/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdosimwin.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#pragma warning(disable : 4786)

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <sstream>
#include <algorithm>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_simulator/rdosimwin.h"
#include "rdo_kernel/rdokernel.h"
#include "rdo_repository/rdorepository.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
#include "rdo_common/rdodebug.h"
// ===============================================================================

//#ifndef DISABLE_CORBA

//#define CORBA_ENABLE

#ifdef CORBA_ENABLE

#include <omniORB4/CORBA.h>

namespace rdoCorba
{
CORBA::ORB_var g_orb;

class RDOCorba_i: public POA_rdoParse::RDOCorba
{
public:
	RDOCorba_i()
	{}
	virtual ~RDOCorba_i()
	{}
    
	virtual PTR(rdoParse::RDOCorba::GetRTP) getRDORTPlist (REF(::CORBA::Long) rtp_count);
	virtual PTR(rdoParse::RDOCorba::GetRSS) getRDORSSPlist(REF(::CORBA::Long) rss_count);

	static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref);
};

PTR(rdoParse::RDOCorba::GetRTP) RDOCorba_i::getRDORTPlist(REF(::CORBA::Long) rtp_count)
{
	//! ������� ������ �������� ��� �������� ���������� �� ������� ����� ��������
	rdoParse::RDOCorba::GetRTP_var my_rtpList = new rdoParse::RDOCorba::GetRTP;

	//! �������� ����������� ��� ������ � ����� �������� ���
	kernel->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RTP, &my_rtpList);
	
	return my_rtpList._retn();
}

PTR(rdoParse::RDOCorba::GetRSS) RDOCorba_i::getRDORSSPlist(REF(::CORBA::Long) rss_count)
{
	//! ������� ������ �������� ��� �������� ���������� �� ������� ��������
	rdoParse::RDOCorba::GetRSS_var my_rssList = new rdoParse::RDOCorba::GetRSS;
	
	//! �������� ����������� ��� ������ � �������� ���
	kernel->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RSS, &my_rssList);
	
	return my_rssList._retn();
}

CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref, CPTR(char) ModelName)
{
	CosNaming::NamingContext_var rootContext;

	try
	{
		//! Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;
		obj = orb->resolve_initial_references(_T("NameService"));
		
		//! Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);
		if (CORBA::is_nil(rootContext))
		{
			TRACE(_T("Failed to narrow the root naming context."));
			return 0;
		}
	}
	catch (REF(CORBA::NO_RESOURCES))
	{
		TRACE(_T("Caught NO_RESOURCES exception. You must configure omniORB with the location of the naming service."));
		return 0;
	}
	catch (REF(CORBA::ORB::InvalidName))
	{
		//! This should not happen!
		TRACE(_T("Service required is invalid [does not exist]."));
		return 0;
	}

	try
	{
		//! Bind a context called "test" to the root context:
		CosNaming::Name contextName;
		contextName.length(1);
		contextName[0].id   = (CPTR(char)) _T("RDO");         //! string copied
		contextName[0].kind = (CPTR(char)) _T("RDO_context"); //! string copied
	
		//! Note on kind: The kind field is used to indicate the type
		//! of the object. This is to avoid conventions such as that used
		//! by files (name.type -- e.g. test.ps = postscript etc.)
		CosNaming::NamingContext_var testContext;
		
		try
		{
			//! Bind the context to root.
			testContext = rootContext->bind_new_context(contextName);
		}
		catch(REF(CosNaming::NamingContext::AlreadyBound))
		{
			//! If the context already exists, this exception will be raised.
			//! In this case, just resolve the name and assign testContext
			//! to the object returned:
			CORBA::Object_var obj;
			obj = rootContext->resolve(contextName);
			
			testContext = CosNaming::NamingContext::_narrow(obj);
			
			if (CORBA::is_nil(testContext))
			{
				TRACE(_T("Failed to narrow naming context."));
				return 0;
			}
		}

		//! Bind objref with name Echo to the testContext:
		CosNaming::Name objectName;
		objectName.length(1);
		
		//! rdoParse::RDOParserSMRInfo parser;
		//! parser.parse();

		objectName[0].id   = ModelName;
		objectName[0].kind = (CPTR(char)) _T("Object");

		try
		{
			testContext->bind(objectName, objref);
		}
		catch(REF(CosNaming::NamingContext::AlreadyBound))
		{
			testContext->rebind(objectName, objref);
		}
		
		//! Note: Using rebind() will overwrite any Object previously bound
		//! to /test/Echo with obj.
		//! Alternatively, bind() can be used, which will raise a
		//! CosNaming::NamingContext::AlreadyBound exception if the name
		//! supplied is already bound to an object.
		//! Amendment: When using OrbixNames, it is necessary to first try bind
		//! and then rebind, as rebind on it�s own will throw a NotFoundexception if
		//! the Name has not already been bound. [This is incorrect behaviour -
		//! it should just bind].
	}
	catch(REF(CORBA::TRANSIENT))
	{
		TRACE(_T("Caught system exception TRANSIENT -- unable to contact the naming service."));
		TRACE(_T("Make sure the naming server is running and that omniORB is configured correctly."));
		return 0;
	}
	catch(REF(CORBA::SystemException) ex)
	{
		TRACE1(_T("Caught a CORBA:: %s while using the naming service."), ex._name());
		return 0;
	}
	
	return 1;
}

ruint RDOThreadCorba::corbaRunThreadFun(PTR(void) param)
{
	try
	{
		int argc = 0;
		g_orb = CORBA::ORB_init(argc, NULL);
		
		CORBA::Object_var obj = g_orb->resolve_initial_references(_T("RootPOA"));
		PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
		
		PTR(RDOCorba_i) myrdocorba = new RDOCorba_i();

		PortableServer::ObjectId_var myrdocorbaid = poa->activate_object(myrdocorba);

		//! Obtain a reference to the object, and register it in
		//! the naming service.
		obj = myrdocorba->_this();

		CPTR(char) ModelName = _T("���");
		//! CPTR(char) ModelName = _T("�����");

		if (!bindObjectToName(g_orb, obj, ModelName))
		{
			delete myrdocorba;
			return 1;
		}

		myrdocorba->_remove_ref();
		
		PortableServer::POAManager_var pman = poa->the_POAManager();
		
		pman->activate();
		
		g_orb->run();
	}
	catch(REF(CORBA::SystemException) ex)
	{
		TRACE1(_T("Caught CORBA::%s"), ex._name());
	}
	catch(REF(CORBA::Exception))
	{
		TRACE(_T("Caught CORBA::Exception: "));
	}	
	catch(REF(omniORB::fatalException) fe)
	{
		TRACE3(_T("Caught omniORB::fatalException: file: %s, line: %d, mesg: %s "), fe.file(), fe.line(), fe.errmsg());
	}
	
	return 0;
}


RDOThreadCorba::RDOThreadCorba()
	: RDOThreadMT             (_T("RDOThreadCorba"))
	, thread_corbaRunThreadFun(NULL                )
{
	//! ����� ��� ���������� ��������� �����
	after_constructor();
}

void RDOThreadCorba::proc(REF(RDOMessageInfo) msg)
{
	//! ����� ��� ��������� ��������� �����
}

void RDOThreadCorba::start()
{
#ifdef TR_TRACE
	trace(thread_name + _T(" corba begin"));
#endif

	//! ����� ��� ������� �����
	thread_corbaRunThreadFun = AfxBeginThread(corbaRunThreadFun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	thread_corbaRunThreadFun->m_bAutoDelete = false;
	thread_corbaRunThreadFun->ResumeThread();

#ifdef TR_TRACE
	trace(thread_name + _T(" corba end"));
#endif
}

void RDOThreadCorba::idle()
{
#ifdef RDO_MT
	if (broadcast_waiting || !was_start || was_close)
	{
		RDOThread::idle();
		return;
	}
#endif
	//! ��� ��� �������� ������ �����
}

void RDOThreadCorba::stop()
{
#ifdef TR_TRACE
	trace(thread_name + _T(" stop begin"));
#endif
	//! ����� ��� ��������� �����
	if (g_orb != CORBA::ORB::_nil())
	{
		try
		{
			g_orb->shutdown(true);
			g_orb->destroy();
		}
		catch (...)
		{}
	}

	if (thread_corbaRunThreadFun)
	{
		DWORD res;
		while (true)
		{
			if (::GetExitCodeThread(thread_corbaRunThreadFun->m_hThread, &res))
			{
				if 	(res == STILL_ACTIVE)
				{
					::Sleep(1);
				}
				else
				{
					break;
				}
			}
			else
			{
				int i = 0;
			}
		}
//		thread_corbaRunThreadFun->Delete();
		delete thread_corbaRunThreadFun;
		thread_corbaRunThreadFun = NULL;
	}

	RDOThread::stop();
#ifdef TR_TRACE
	trace(thread_name + _T(" stop end"));
#endif
}

} //! namespace rdoCorba

#endif //! CORBA_ENABLE


OPEN_RDO_SIMULATOR_NAMESPACE

// --------------------------------------------------------------------
// ---------- RDORuntimeTracer
// --------------------------------------------------------------------
class RDORuntimeTracer: public rdoRuntime::RDOTrace, public rdoRuntime::RDOEndL
{
public:
	virtual REF(std::ostream)        getOStream()    { return m_stream; }
	virtual REF(rdoRuntime::RDOEndL) getEOL()        { return *this;    }

	void onEndl()
	{
		CREF(tstring) trace_str = m_stream.str();
		if (trace_str.empty()        ) return;
		if (!m_pSimulator->m_canTrace) return;
		tstring::size_type pos = 0;
		while (true)
		{
			tstring::size_type next = trace_str.find('\n', pos);
			tstring str = trace_str.substr(pos, next-pos);
			m_pSimulator->m_pThreadRuntime->broadcastMessage(RDOThread::RT_RUNTIME_TRACE_STRING, &str, true);
			if (next == tstring::npos) break;
			pos = next + 1;
			if (pos >= trace_str.length()) break;
		}
		m_stream.str(_T(""));
	}

	RDORuntimeTracer(PTR(RDOThreadSimulator) m_pSimulator)
		: m_pSimulator(m_pSimulator)
	{
		m_isNullTracer = false;
	}

private:
	PTR(RDOThreadSimulator) m_pSimulator;
	rdo::textstream         m_stream;
};

// --------------------------------------------------------------------
// ---------- RDOSimResulter
// --------------------------------------------------------------------
class RDOSimResulter: public rdoRuntime::RDOResults
{
public:
	RDOSimResulter(REF(std::ostream) stream)
		: m_stream(stream)
	{}

private:
	REF(std::ostream) m_stream;

	virtual REF(std::ostream) getOStream()
	{
		return m_stream;
	}
};

CLOSE_RDO_SIMULATOR_NAMESPACE

OPEN_RDO_RUNTIME_NAMESPACE
// --------------------------------------------------------------------
// ---------- RDOThreadRunTime
// --------------------------------------------------------------------
RDOThreadRunTime::RDOThreadRunTime()
	: RDOThreadMT    (_T("RDOThreadRunTime"))
	, m_pSimulator   (NULL                  )
	, m_runtime_error(false                 )
{
	::GetSystemTime(&m_time_start);

	m_pSimulator = kernel->simulator();

	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY);
	notifies.push_back(RT_RUNTIME_GET_MODE                  );
	notifies.push_back(RT_RUNTIME_SET_MODE                  );
	notifies.push_back(RT_RUNTIME_GET_SPEED                 );
	notifies.push_back(RT_RUNTIME_SET_SPEED                 );
	notifies.push_back(RT_RUNTIME_GET_SHOWRATE              );
	notifies.push_back(RT_RUNTIME_SET_SHOWRATE              );
	notifies.push_back(RT_RUNTIME_GET_TIMENOW               );
	notifies.push_back(RT_RUNTIME_GET_FRAME                 );
	notifies.push_back(RT_RUNTIME_KEY_DOWN                  );
	notifies.push_back(RT_RUNTIME_KEY_UP                    );
	notifies.push_back(RT_RUNTIME_FRAME_AREA_DOWN           );
	after_constructor();
}

void RDOThreadRunTime::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_THREAD_CLOSE:
		{
			broadcastMessage(RT_RUNTIME_MODEL_STOP_BEFORE);
			break;
		}
		case RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY:
		{
			m_runtime_error = true;
			break;
		}
		case RT_RUNTIME_GET_MODE:
		{
			*static_cast<PTR(rdoRuntime::RunTimeMode)>(msg.param) = m_pSimulator->m_pRuntime->getMode();
			break;
		}
		case RT_RUNTIME_SET_MODE:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->setMode(*static_cast<PTR(rdoRuntime::RunTimeMode)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_SPEED:
		{
			*static_cast<PTR(double)>(msg.param) = m_pSimulator->m_pRuntime->getSpeed();
			break;
		}
		case RT_RUNTIME_SET_SPEED:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->setSpeed(*static_cast<PTR(double)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_SHOWRATE:
		{
			*static_cast<PTR(double)>(msg.param) = m_pSimulator->m_pRuntime->getShowRate();
			break;
		}
		case RT_RUNTIME_SET_SHOWRATE:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->setShowRate(*static_cast<PTR(double)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_TIMENOW:
		{
			*static_cast<PTR(double)>(msg.param) = m_pSimulator->m_pRuntime->getTimeNow();
			break;
		}
		case RT_RUNTIME_GET_FRAME:
		{
			msg.lock();
			PTR(GetFrame) getframe = static_cast<PTR(GetFrame)>(msg.param);
			m_pSimulator->m_pRuntime->allFrames.at(getframe->m_number)->prepareFrame(getframe->m_pFrame, m_pSimulator->m_pRuntime);
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_LAST_BREAKPOINT:
		{
			msg.lock();
			*static_cast<PTR(tstring)>(msg.param) = m_pSimulator->m_pRuntime->getLastBreakPointName();
			msg.unlock();
			break;
		}
		case RT_RUNTIME_KEY_DOWN:
		{
			msg.lock();
			if (std::find(m_pSimulator->m_pRuntime->using_scan_codes.begin(), m_pSimulator->m_pRuntime->using_scan_codes.end(), *static_cast<PTR(ruint)>(msg.param)) != m_pSimulator->m_pRuntime->using_scan_codes.end())
			{
				if (!m_pSimulator->m_pRuntime->keyDown(*static_cast<PTR(ruint)>(msg.param)))
				{
					m_pSimulator->m_pRuntime->setShowRate(m_pSimulator->m_pRuntime->getShowRate());
				}
			}
			msg.unlock();
			break;
		}
		case RT_RUNTIME_KEY_UP:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->keyUp(*static_cast<PTR(ruint)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_RUNTIME_FRAME_AREA_DOWN:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->activeAreasMouseClicked.push_back(*static_cast<PTR(tstring)>(msg.param));
			m_pSimulator->m_pRuntime->setShowRate(m_pSimulator->m_pRuntime->getShowRate());
			msg.unlock();
			break;
		}
	}
}

void RDOThreadRunTime::start()
{
#ifdef TR_TRACE
	trace(thread_name + _T(" start begin"));
#endif

	broadcastMessage(RT_RUNTIME_MODEL_START_BEFORE);

	PTR(RDOTrace)               pTracer;
	PTR(rdoRuntime::RDOResults) pResults;
	PTR(rdoRuntime::RDOResults) pResultsInfo;

	//! Creating tracer and results //////////////////////////////////
	if (!m_pSimulator->m_pParser->getSMR()->hasFile(_T("Trace_file")))
	{
		pTracer = new RDOTrace();
	}
	else
	{
		pTracer = new rdoSimulator::RDORuntimeTracer(m_pSimulator);
	}

	m_pSimulator->m_resultString.str(_T(""));
	pResults = new rdoSimulator::RDOSimResulter(m_pSimulator->m_resultString);

	m_pSimulator->m_resultInfoString.str(_T(""));
	pResultsInfo = new rdoSimulator::RDOSimResulter(m_pSimulator->m_resultInfoString);

	//! RDO config initialization
	m_pSimulator->m_pRuntime->keysDown.clear();
	m_pSimulator->m_pRuntime->activeAreasMouseClicked.clear();
	m_pSimulator->m_pRuntime->setStartTime     (m_pSimulator->m_pParser->getSMR()->getRunStartTime  ());
	m_pSimulator->m_pRuntime->setTraceStartTime(m_pSimulator->m_pParser->getSMR()->getTraceStartTime());
	m_pSimulator->m_pRuntime->setTraceEndTime  (m_pSimulator->m_pParser->getSMR()->getTraceEndTime  ());

	//! Modelling
	m_pSimulator->m_canTrace = true;

	try
	{
		m_pSimulator->m_exitCode = rdoSimulator::EC_OK;
		m_pSimulator->m_pRuntime->rdoInit(pTracer, pResults, pResultsInfo);
		switch (m_pSimulator->m_pParser->getSMR()->getShowMode())
		{
			case rdoSimulator::SM_NoShow   : m_pSimulator->m_pRuntime->setMode(rdoRuntime::RTM_MaxSpeed); break;
			case rdoSimulator::SM_Animation: m_pSimulator->m_pRuntime->setMode(rdoRuntime::RTM_Sync    ); break;
			case rdoSimulator::SM_Monitor  : m_pSimulator->m_pRuntime->setMode(rdoRuntime::RTM_Pause   ); break;
		}
		m_pSimulator->m_pRuntime->setShowRate(m_pSimulator->m_pParser->getSMR()->getShowRate());
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{
		m_runtime_error = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
	}
	catch (REF(rdoRuntime::RDORuntimeException) ex)
	{
		m_runtime_error = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
		tstring mess = ex.getType() + _T(" : ") + ex.message();
		sendMessage(kernel, RDOThread::RT_DEBUG_STRING, &mess);
	}

	if (!m_runtime_error)
	{
		broadcastMessage(RT_RUNTIME_MODEL_START_AFTER);
	}

#ifdef TR_TRACE
	trace(thread_name + _T(" start end, runing simulation"));
#endif
}

void RDOThreadRunTime::idle()
{
#ifdef RDO_MT
	if (broadcast_waiting || !was_start || was_close)
	{
		RDOThread::idle();
		return;
	}
#endif
//	TRACE(_T("R. %d, %d, %d, %d\n"), ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId());
	try
	{
		if (m_runtime_error || !m_pSimulator->m_pRuntime->rdoNext())
		{
			sendMessage(this, RT_THREAD_CLOSE);
		}
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{
		m_runtime_error = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
	}
	catch (REF(rdoRuntime::RDORuntimeException) ex)
	{
		m_runtime_error = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
		tstring mess = ex.getType() + _T(" : ") + ex.message();
		sendMessage(kernel, RDOThread::RT_DEBUG_STRING, &mess);
	}
//	catch (...) {
//		m_runtime_error = true;
//		TRACE(_T("******************************** ������ rdoNext()\n"));
//		m_pSimulator->m_pRuntime->onRuntimeError();
//	}
}

void RDOThreadRunTime::writeResultsInfo()
{
	switch (m_pSimulator->m_pRuntime->whyStop)
	{
		case rdoSimulator::EC_OK:
			m_pSimulator->m_pRuntime->getResultsInfo() << _T("$Status = ") << _T("NORMAL_TERMINATION");
			break;
		case rdoSimulator::EC_NoMoreEvents:
			m_pSimulator->m_pRuntime->getResultsInfo() << _T("$Status = ") << _T("NO_MORE_EVENTS");
			break;
		case rdoSimulator::EC_RunTimeError:
			m_pSimulator->m_pRuntime->getResultsInfo() << _T("$Status = ") << _T("RUN_TIME_ERROR");
			break;
		case rdoSimulator::EC_UserBreak:
			m_pSimulator->m_pRuntime->getResultsInfo() << _T("$Status = ") << _T("USER_BREAK");
			break;
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << _T("$Result_values  0  ") << m_pSimulator->m_pRuntime->getTimeNow();
	SYSTEMTIME time_stop;
	::GetSystemTime(&time_stop);
	double delay = -1;
	if (m_time_start.wYear == time_stop.wYear && m_time_start.wMonth == time_stop.wMonth)
	{
		delay = (time_stop.wDay - m_time_start.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - m_time_start.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - m_time_start.wMinute) * 60 * 1000 + (time_stop.wSecond - m_time_start.wSecond) * 1000 + (time_stop.wMilliseconds - m_time_start.wMilliseconds);
	}
	else if (time_stop.wYear - m_time_start.wYear == 1 && m_time_start.wMonth == 12 && time_stop.wMonth == 1)
	{
		delay = (time_stop.wDay + 31 - m_time_start.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - m_time_start.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - m_time_start.wMinute) * 60 * 1000 + (time_stop.wSecond - m_time_start.wSecond) * 1000 + (time_stop.wMilliseconds - m_time_start.wMilliseconds);
	}
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << _T("  ") << delay / 1000.0;
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << _T("  ?");
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << _T("  EventCount           ") << m_pSimulator->m_pRuntime->get_cnt_events() << _T("  ") << (double)m_pSimulator->m_pRuntime->get_cnt_events() / m_pSimulator->m_pRuntime->getTimeNow() << _T("  ");
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_events() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << _T("?");
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << _T("  OperRuleCheckCounter ") << m_pSimulator->m_pRuntime->get_cnt_choice_from() << _T("  ") << (double)m_pSimulator->m_pRuntime->get_cnt_choice_from() / m_pSimulator->m_pRuntime->getTimeNow() << _T("  ");
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_choice_from() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << _T("?");
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << _T("  AExpCalcCounter      ") << m_pSimulator->m_pRuntime->get_cnt_calc_arithm() << _T("  ") << (double)m_pSimulator->m_pRuntime->get_cnt_calc_arithm() / m_pSimulator->m_pRuntime->getTimeNow() << _T("  ");
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_calc_arithm() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << _T("?");
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << _T("  BExpCalcCounter      ") << m_pSimulator->m_pRuntime->get_cnt_calc_logic() << _T("  ") << (double)m_pSimulator->m_pRuntime->get_cnt_calc_logic() / m_pSimulator->m_pRuntime->getTimeNow() << _T("  ");
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_calc_logic() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << _T("?");
	}
}

void RDOThreadRunTime::stop()
{
#ifdef TR_TRACE
	trace(thread_name + _T(" stop begin"));
#endif

	try
	{
		m_pSimulator->m_pRuntime->rdoPostProcess();
		writeResultsInfo();
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{
		m_runtime_error = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
	}
	catch (REF(rdoRuntime::RDORuntimeException) ex)
	{
		m_runtime_error = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
		tstring mess = ex.getType() + _T(" : ") + ex.message();
		sendMessage(kernel, RDOThread::RT_DEBUG_STRING, &mess);
	}

	broadcastMessage(RT_RUNTIME_MODEL_STOP_AFTER);

	RDOThread::stop();

#ifdef TR_TRACE
	trace(thread_name + _T(" stop end"));
#endif
}

CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_SIMULATOR_NAMESPACE
// --------------------------------------------------------------------
// ---------- RDOThreadSimulator
// --------------------------------------------------------------------
RDOThreadSimulator::RDOThreadSimulator()
	: RDOThreadMT     (_T("RDOThreadSimulator"))
	, m_pRuntime      (NULL                    )
	, m_pParser       (NULL                    )
	, m_pThreadRuntime(NULL                    )
	, m_exitCode      (rdoSimulator::EC_OK     )
{
	notifies.push_back(RT_STUDIO_MODEL_BUILD              );
	notifies.push_back(RT_STUDIO_MODEL_RUN                );
	notifies.push_back(RT_STUDIO_MODEL_STOP               );
	notifies.push_back(RT_SIMULATOR_GET_MODEL_STRUCTURE   );
	notifies.push_back(RT_SIMULATOR_GET_MODEL_RESULTS     );
	notifies.push_back(RT_SIMULATOR_GET_MODEL_RESULTS_INFO);
	notifies.push_back(RT_SIMULATOR_GET_MODEL_EXITCODE    );
	notifies.push_back(RT_SIMULATOR_GET_LIST              );
	notifies.push_back(RT_SIMULATOR_GET_ERRORS            );
	notifies.push_back(RT_THREAD_STOP_AFTER               );
	notifies.push_back(RT_CODECOMP_GET_DATA               );
	notifies.push_back(RT_CORBA_PARSER_GET_RTP            );
	notifies.push_back(RT_CORBA_PARSER_GET_RSS            );
	//notifies.push_back(RT_CORBA_PARSER_GET_RTP_COUNT      );
	//notifies.push_back(RT_CORBA_PARSER_GET_RTP_PAR_COUNT  );
	after_constructor();
}

RDOThreadSimulator::~RDOThreadSimulator()
{
	terminateModel();
	closeModel    ();
}

void RDOThreadSimulator::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_STUDIO_MODEL_BUILD:
		{
			parseModel();
			break;
		}
		case RT_STUDIO_MODEL_RUN:
		{
			runModel();
			break;
		}
		case RT_STUDIO_MODEL_STOP:
		{
			stopModel();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_STRUCTURE:
		{
			msg.lock();
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_pParser->getModelStructure();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS:
		{
			msg.lock();
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_resultString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS_INFO:
		{
			msg.lock();
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_pParser->getChanges();
			*static_cast<PTR(rdo::textstream)>(msg.param) << std::endl << std::endl;
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_resultInfoString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_EXITCODE:
		{
			msg.lock();
			*static_cast<PTR(rdoSimulator::RDOExitCode)>(msg.param) = m_exitCode;
			msg.unlock();
			break;
		}
		case RT_CODECOMP_GET_DATA:
		{
			codeCompletion();
			break;
		}
		
#ifdef CORBA_ENABLE

		case RT_CORBA_PARSER_GET_RTP:
		{
			msg.lock();
			corbaGetRTP(*static_cast<PTR(rdoParse::RDOCorba::GetRTP_var)>(msg.param));
			msg.unlock();
			break;
		}

		case RT_CORBA_PARSER_GET_RSS:
		{
			msg.lock();
			corbaGetRSS(*static_cast<PTR(rdoParse::RDOCorba::GetRSS_var)>(msg.param));
			msg.unlock();
			break;
		}
#endif //! CORBA_ENABLE

		case RT_SIMULATOR_GET_LIST:
		{
			msg.lock();
			PTR(GetList) getlist = static_cast<PTR(GetList)>(msg.param);
			switch (getlist->m_type)
			{
				case GetList::frames:
				{
					if (m_pRuntime)
					{
						int size = m_pRuntime->allFrames.size();
						for (int i = 0; i < size; i++)
						{
							getlist->m_list->push_back(m_pRuntime->allFrames.at(i)->name());
						}
					}
					break;
				}
				case GetList::bitmaps:
				{
					if (m_pRuntime)
					{
						int size = m_pRuntime->allFrames.size();
						for (int i = 0; i < size; i++)
						{
							m_pRuntime->allFrames.at(i)->getBitmaps(*getlist->m_list);
						}
					}
					break;
				}
			}
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_ERRORS:
		{
			SyntaxErrorList errors = getErrors();
			msg.lock();
			PTR(SyntaxErrorList) msg_errors = static_cast<PTR(SyntaxErrorList)>(msg.param);
			msg_errors->assign(errors.begin(), errors.end());
			msg.unlock();
			break;
		}
		case RT_THREAD_STOP_AFTER:
		{
			if (msg.from == m_pThreadRuntime)
			{
				//! rdoSimulator::EC_ParserError   - �� ������������ � run-time
				//! rdoSimulator::EC_ModelNotFound - �� ������������ � run-time
				//! rdoSimulator::EC_UserBreak     - ��������������� � m_pSimulator, �������� RT_THREAD_STOP_AFTER �� �����������
				m_exitCode = m_pRuntime->whyStop;
				if (!m_pThreadRuntime->m_runtime_error)
				{
					//! ������������ ���� ���������
					broadcastMessage(RT_SIMULATOR_MODEL_STOP_OK);
					closeModel();
				}
				else
				{
					//! ������������ ����, �� �� ���������
					broadcastMessage(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
					closeModel();
				}
				//! ����� ��������� ����, ���� ������� � �������
				//! �������� ��� ��� ��������, �.�. thread_destroy �� ������ �������������� � m_pThreadRuntime ���� �������������� RT_THREAD_STOP_AFTER
#ifdef RDO_MT
				delete m_pThreadRuntime->thread_destroy;
				m_pThreadRuntime->thread_destroy = NULL;
#endif
				m_pThreadRuntime = NULL;
			}
			break;
		}
	}
}

rbool RDOThreadSimulator::parseModel()
{
	terminateModel();
	closeModel();

	m_pParser  = new rdoParse::RDOParserModel();
	m_pRuntime = m_pParser->runtime();

	try
	{
		m_exitCode = rdoSimulator::EC_OK;
		m_pParser->parse();
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{
		m_exitCode = rdoSimulator::EC_ParserError;
		broadcastMessage(RT_SIMULATOR_PARSE_ERROR);
		closeModel();
		return false;
	}
	catch (REF(rdoRuntime::RDORuntimeException) ex)
	{
		tstring mess = ex.getType() + _T(" : ") + ex.message();
		broadcastMessage(RT_SIMULATOR_PARSE_STRING, &mess);
		m_exitCode = rdoSimulator::EC_ParserError;
		broadcastMessage(RT_SIMULATOR_PARSE_ERROR);
		closeModel();
		return false;
	}

	m_showMode = getInitialShowMode();
	m_showRate = getInitialShowRate();

	broadcastMessage(RT_SIMULATOR_PARSE_OK);

	return true;
}

void RDOThreadSimulator::runModel()
{
	if (parseModel())
	{
		m_pParser->error().clear();
		m_exitCode = rdoSimulator::EC_OK;
		m_pThreadRuntime = new rdoRuntime::RDOThreadRunTime();
	}
}

void RDOThreadSimulator::stopModel()
{
	m_pRuntime->onUserBreak();
	m_exitCode = rdoSimulator::EC_UserBreak;
	terminateModel();
	m_canTrace = false;
	broadcastMessage(RT_SIMULATOR_MODEL_STOP_BY_USER);
	closeModel();
//	kernel.callback(RDOKernel::modelExit, rdoSimulator::EC_UserBreak);
}

void RDOThreadSimulator::terminateModel()
{
	if (m_pThreadRuntime)
	{
		//! ��������� ����������� �� ��������� run-time-�����, �.�. ��������� � ����
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.erase(std::find(notifies.begin(), notifies.end(), RT_THREAD_STOP_AFTER));
#ifdef RDO_MT
		notifies_mutex.Unlock();
		PTR(CEvent) thread_destroy = m_pThreadRuntime->thread_destroy;
#endif

		sendMessage(m_pThreadRuntime, RDOThread::RT_THREAD_CLOSE);

#ifdef RDO_MT
		thread_destroy->Lock();
		delete thread_destroy;
#endif
		m_pThreadRuntime = NULL;

		//! ����� ������ ����������� �� ��������� run-time-�����, ����� ���������� ���������� ���������� ������ (��� �� run-time-error)
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.push_back(RT_THREAD_STOP_AFTER);
#ifdef RDO_MT
		notifies_mutex.Unlock();
#endif
	}
}

void RDOThreadSimulator::closeModel()
{
/*
	try
	{
		if (m_pRuntime)
		{
			delete m_pRuntime;
			m_pRuntime = NULL;
		}
	}
	catch (...)
	{
		m_pRuntime = NULL;
		TRACE(_T("******************************** ������ �������� m_pRuntime\n"));
	}
*/
	m_pRuntime = NULL;
	try
	{
		if (m_pParser)
		{
			delete m_pParser;
			m_pParser = NULL;
		}
	}
	catch (...)
	{
		m_pParser = NULL;
		TRACE(_T("******************************** ������ �������� m_pParser\n"));
	}
}

void RDOThreadSimulator::parseSMRFileInfo(REF(rdo::textstream) smr, REF(rdoModelObjects::RDOSMRFileInfo) info)
{
	try
	{
		rdoConverter::RDOParserModel converter;
		rdoRepository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::SMR);
		kernel->sendMessage(kernel->repository(), RT_REPOSITORY_MODEL_GET_FILEINFO, &fileInfo);
		switch (converter.convert(fileInfo.m_full_name))
		{
		case rdoConverter::RDOParserModel::CNV_NONE : break;
		case rdoConverter::RDOParserModel::CNV_OK   : break;
		case rdoConverter::RDOParserModel::CNV_ERROR:
			{
				tstring mess(_T("������ ����������\n"));
				broadcastMessage(RT_DEBUG_STRING, &mess);
				CREF(rdoConverter::Error::ErrorList) errorList = converter.error().getList();
				BOOST_AUTO(it, errorList.begin());
				while (it != errorList.end())
				{
					broadcastMessage(RT_DEBUG_STRING, const_cast<PTR(tstring)>(&it->message));
					++it;
				}
			}
			break;
		default: NEVER_REACH_HERE;
		}
	}
	catch (REF(rdoConverter::RDOSyntaxException))
	{}
	catch (REF(rdoRuntime::RDORuntimeException))
	{}
	catch (...)
	{}

	rdoParse::RDOParserSMRInfo parser;

	try
	{
		info.m_error = false;
		parser.parse(smr);
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{
		broadcastMessage(RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR);
		info.m_error = true;
	}
	catch (REF(rdoRuntime::RDORuntimeException) ex)
	{
		tstring mess = ex.getType() + _T(" : ") + ex.message();
		broadcastMessage(RDOThread::RT_SIMULATOR_PARSE_STRING, &mess);
		broadcastMessage(RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR);
		info.m_error = true;
	}

	if (!parser.hasSMR())
	{
		broadcastMessage(RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR);
		broadcastMessage(RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY);
		info.m_error = true;
	}
	else
	{
		info.m_modelName     = parser.getSMR()->getFile(_T("Model_name")    );
		info.m_resourceFile  = parser.getSMR()->getFile(_T("Resource_file") );
		info.m_frameFile     = parser.getSMR()->getFile(_T("Frame_file")    );
		info.m_statisticFile = parser.getSMR()->getFile(_T("Statistic_file"));
		info.m_resultsFile   = parser.getSMR()->getFile(_T("Results_file")  );
		info.m_traceFile     = parser.getSMR()->getFile(_T("Trace_file")    );
	}
}

RDOThreadSimulator::SyntaxErrorList RDOThreadSimulator::getErrors()
{
	SyntaxErrorList res;

	if (!m_pParser)
	{
		return res;
	}

	res = m_pParser->error().getList();
	res.insert(res.end(), m_pRuntime->errors.begin(), m_pRuntime->errors.end());
	return res;
}

ShowMode RDOThreadSimulator::getInitialShowMode() const
{
	return m_pParser->getSMR()->getShowMode();
}

int RDOThreadSimulator::getInitialFrameNumber() const
{
	return m_pParser->getSMR()->getFrameNumber();
}

double RDOThreadSimulator::getInitialShowRate() const
{
	return m_pParser->getSMR()->getShowRate();
}

void RDOThreadSimulator::codeCompletion()
{}

#ifdef CORBA_ENABLE

void RDOThreadSimulator::corbaGetRTP(REF(rdoParse::RDOCorba::GetRTP_var) my_rtpList)
{
	//! ���������� ���� � ������� ������ ������ (�������, � �� ����������)
	rdoParse::RDOParserCorba parser;
	try
	{
		parser.parse();
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{}
	catch (REF(rdoRuntime::RDORuntimeException))
	{}

	::CORBA::Long i = 0, j = 0;

	//! ����������� �� ���� ����� � ���������� � RTPList
	rdoMBuilder::RDOResTypeList rtpList(&parser);
	
	//! ������� ���������� ����� ��������
	rdoMBuilder::RDOResTypeList::List::const_iterator rtp_it = rtpList.begin();
	
	::CORBA::Long rtp_count = 0;

	while (rtp_it != rtpList.end())
	{
		rtp_count++;
		rtp_it++;
	}

	//! �������� ������ ��� ������������������
	my_rtpList->length(rtp_count);

	//! ����� ������������ � ������ ������ ����� ��������
	rtp_it = rtpList.begin();

	while (rtp_it != rtpList.end())
	{
		//! ������� ��������� ���������

		my_rtpList[i].m_name = CORBA::string_dup(rtp_it->name().c_str());
		
		if ((rtp_it->getType()) == rdoMBuilder::RDOResType::rt_permanent)
			my_rtpList[i].m_type=rdoParse::RDOCorba::rt_permanent;
		else
			my_rtpList[i].m_type=rdoParse::RDOCorba::rt_temporary;

		//! ������� ���������� ���������� i-�� ���� �������
		rdoMBuilder::RDOResType::ParamList::List::const_iterator param_it = rtp_it->m_params.begin();
		my_rtpList[i].m_param_count = 0;

		while (param_it != rtp_it->m_params.end())
		{
			my_rtpList[i].m_param_count++;
			param_it++;
		}
		
		//! �������� ������ ��� ������������������ ���������� i-�� ���� ��������
		my_rtpList[i].m_param.length(my_rtpList[i].m_param_count);

		//! ����� ������������ � ������ ������ ���������� i-�� ���� ��������
		param_it = rtp_it->m_params.begin();

		while (param_it != rtp_it->m_params.end())
		{
			//! ��������� � ��������� ��������!!!!!!!!!!!!!!!!
			my_rtpList[i].m_param[j].m_name = CORBA::string_dup(param_it->name().c_str());

			my_rtpList[i].m_param[j].m_diap_int = 0;
			my_rtpList[i].m_param[j].m_default_int_ch = 0;
			my_rtpList[i].m_param[j].m_diap_double = 0;
			my_rtpList[i].m_param[j].m_default_double_ch = 0;
			my_rtpList[i].m_param[j].m_var_enum_ch = 0;	
			my_rtpList[i].m_param[j].m_default_enum_ch = 0;
			my_rtpList[i].m_param[j].m_var_enum_count = 0;

			switch (param_it->typeID())
			{
				case rdoRuntime::RDOType::t_int:
				{
					my_rtpList[i].m_param[j].m_type = rdoParse::RDOCorba::int_type;

					if (param_it->hasDiap())
					{
						my_rtpList[i].m_param[j].m_min_int = param_it->getMin()->getInt();
						my_rtpList[i].m_param[j].m_max_int = param_it->getMax()->getInt();
						my_rtpList[i].m_param[j].m_diap_int = 1;

					}
					if (param_it->hasDefault())
					{
						my_rtpList[i].m_param[j].m_default_int = param_it->getDefault()->getInt();
						my_rtpList[i].m_param[j].m_default_int_ch = 1;
					}
					break;
				}
				case rdoRuntime::RDOType::t_real:
				{
					my_rtpList[i].m_param[j].m_type = rdoParse::RDOCorba::double_type;

					if (param_it->hasDiap())
					{
						my_rtpList[i].m_param[j].m_min_double = param_it->getMin()->getDouble();
						my_rtpList[i].m_param[j].m_max_double = param_it->getMax()->getDouble();
						my_rtpList[i].m_param[j].m_diap_double = 1;

					}
					if (param_it->hasDefault())
					{
						my_rtpList[i].m_param[j].m_default_double = param_it->getDefault()->getDouble();
						my_rtpList[i].m_param[j].m_default_double_ch = 1;
					}
					break;
				}
				case rdoRuntime::RDOType::t_enum:
				{
					my_rtpList[i].m_param[j].m_type = rdoParse::RDOCorba::enum_type;
					
					//! ������� ���������� �������� ������������� ����
					rdoRuntime::RDOEnumType::CIterator enum_it = param_it->getEnum()->getEnums().begin();

					CORBA::Long k = 0;

					while (enum_it != param_it->getEnum()->getEnums().end())
					{
						k++;					
						enum_it++;
					}

					//! �������� ������ ��� ������������������ �������� j-�� ��������� ������������� ���� i-�� ���� ��������
					my_rtpList[i].m_param[j].m_var_enum.length(k);
					
					enum_it = param_it->getEnum()->getEnums().begin();
					k = 0;

					while (enum_it != param_it->getEnum()->getEnums().end())
					{
						my_rtpList[i].m_param[j].m_var_enum[k] = CORBA::string_dup(enum_it->c_str());
						enum_it++;
						k++;
					}
					
					if (param_it->hasDefault())
					{
						my_rtpList[i].m_param[j].m_default_enum = CORBA::string_dup(param_it->getDefault()->getAsString().c_str());
						my_rtpList[i].m_param[j].m_default_enum_ch = 1;
					}

					my_rtpList[i].m_param[j].m_var_enum_ch = 1;
					my_rtpList[i].m_param[j].m_var_enum_count = k;
					break;
				}
				default: break;
			}
		
			j++;
			param_it++;
		}
		j = 0;
		i++;		
		rtp_it++;
	}

}

void RDOThreadSimulator::corbaGetRSS(REF(rdoParse::RDOCorba::GetRSS_var) my_rssList)
{
	//! ���������� ���� � ������� ������ ������ (�������, � �� ����������)
	rdoParse::RDOParserCorba parser;
	try
	{
		parser.parse();
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{}
	catch (REF(rdoRuntime::RDORuntimeException))
	{}

	//! ����������� �� ���� �������� � ���������� � RSSList
	rdoMBuilder::RDOResourceList rssList(&parser);
	rdoMBuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();

	::CORBA::Long i = 0, j = 0;
	::CORBA::Long rss_count = 0;

	//! ������� ���������� ��������
	while (rss_it != rssList.end())
	{
		rss_count++;
		rss_it++;
	}

	//! �������� ������ ��� ������������������
	my_rssList->length(rss_count);

	//! ����� ������������ � ������ ������ ����� ��������
	rss_it = rssList.begin();

	while (rss_it != rssList.end())
	{
		//! ��������� �������� ���������
		my_rssList[i].m_name = CORBA::string_dup(rss_it->name().c_str());
		my_rssList[i].m_type = CORBA::string_dup(rss_it->getType().name().c_str());
		
		//! ������� ���������� ���������� i-�� ���� �������
		rdoMBuilder::RDOResource::Params::const_iterator param_it = rss_it->begin();
		
		my_rssList[i].m_param_count = 0;

		while (param_it != rss_it->end())
		{
			my_rssList[i].m_param_count++;
			param_it++;
		}
		
		//! �������� ������ ��� ������������������ ���������� i-�� �������
		my_rssList[i].m_param.length(my_rssList[i].m_param_count);

		//! ����� ������������ � ������ ������ ���������� i-�� �������
		param_it = rss_it->begin();

		while (param_it != rss_it->end())
		{
			my_rssList[i].m_param[j].m_name = CORBA::string_dup(param_it->first.c_str());;
		
			switch (param_it->second.typeID())
			{
				case rdoRuntime::RDOType::t_int:
				{
					
					my_rssList[i].m_param[j].m_int = param_it->second->getInt();
					my_rssList[i].m_param[j].m_type = rdoParse::RDOCorba::int_type;

					break;
				}
				case rdoRuntime::RDOType::t_real:
				{

					my_rssList[i].m_param[j].m_double = param_it->second->getDouble();
					my_rssList[i].m_param[j].m_type = rdoParse::RDOCorba::double_type;
					
					break;
				}
				case rdoRuntime::RDOType::t_enum:
				{

					my_rssList[i].m_param[j].m_enum = param_it->second->getAsString().c_str();
					my_rssList[i].m_param[j].m_type = rdoParse::RDOCorba::enum_type;

					break;
				}
				default: break;
			}

			param_it++;
			j++;
		}
	
		j = 0;
		rss_it++;
		i++;
	}

/*
	//! ���������� ���� � ������� ������ ������ (�������, � �� ����������)
	rdoParse::RDOParserCorba parser;
	try
	{
		parser.parse();
	}
	catch (REF(rdoParse::RDOSyntaxException))
	{}
	catch (REF(rdoRuntime::RDORuntimeException))
	{}

	//! ����������� �� ���� �������� � ���������� � RSSList
	rdoMBuilder::RDOResourceList rssList(&parser);
	rdoMBuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();
	/*while (rss_it != rssList.end())
	{
		//! ������� ��������� ���������
		RSS rss;
		rss.m_name = rss_it->name();
		//! ���������� � ������
		RSSList->push_back(rss);
		rss_it++;
	}
*/
}
#endif //! CORBA_ENABLE

// --------------------------------------------------------------------
// ---------- RDOThreadCodeComp
// --------------------------------------------------------------------
RDOThreadCodeComp::RDOThreadCodeComp()
	: RDOThreadMT(_T("RDOThreadCodeComp"))
	, m_pParser  (NULL                   )
{
//	notifies.push_back(RT_CODECOMP_GET_DATA);
	after_constructor();
}

RDOThreadCodeComp::~RDOThreadCodeComp()
{}

void RDOThreadCodeComp::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_CODECOMP_GET_DATA:
		{
//			if (rdoParse::m_pParser) m_pParser = rdoParse::m_pParser;
			if (!m_pParser) break;
			msg.lock();
			PTR(GetCodeComp) data = static_cast<PTR(GetCodeComp)>(msg.param);
//			rdo::binarystream stream;
//			sendMessage(kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &rdoRepository::RDOThreadRepository::FileData(data->file, stream));
//			data->result = stream.data();
			CREF(rdoParse::RDOParser::RTPResTypeList) rtp_list = m_pParser->getRTPResTypes(); 
			STL_FOR_ALL_CONST(rtp_list, rtp_it)
			{
				CREF(rdoParse::RDORTPResType::ParamList) param_list = (*rtp_it)->getParams();
				rdoParse::RDORTPResType::ParamList::const_iterator param_it = param_list.begin();
				while (param_it != param_list.end())
				{
					data->m_result += (*param_it)->name() + ' ';
					param_it++;
				}
			}
			msg.unlock();
//			if (m_pParser != rdoParse::m_pParser) delete m_pParser;
			m_pParser = NULL;
			break;
		}
	}
}

CLOSE_RDO_SIMULATOR_NAMESPACE
