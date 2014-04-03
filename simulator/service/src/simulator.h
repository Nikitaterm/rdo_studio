/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date
  \brief
  \indent    4T
*/

#ifndef _LIB_SERVICE_SIMULATOR_H_
#define _LIB_SERVICE_SIMULATOR_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <vector>
#include <sstream>

#include <boost/noncopyable.hpp>

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
	#ifdef RDO_MT
		#include <afxwin.h>
	#else
		#include <windows.h>
	#endif
#endif // COMPILER_VISUAL_STUDIO
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "simulator/runtime/thread_proxy_i.h"
#include "simulator/compiler/procgui/procgui.h"
#include "converter/smr2rdox/rdo_common/model_objects_convertor.h"
// --------------------------------------------------------------------------------

//#define DISABLE_CORBA
//#ifndef DISABLE_CORBA
namespace rdo {
namespace animation {

struct Frame;

}} // namespace rdo::animation

#define DISABLE_CORBA

//#define CORBA_ENABLE
#ifdef CORBA_ENABLE

#ifndef __WIN32__
#define __WIN32__
#endif

#ifndef __x86__
#define __x86__
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef __NT__
#define __NT__
#endif

#ifndef __OSVERSION__
#define __OSVERSION__ 4
#endif

#pragma warning(disable: 4996)
#include <omniORB4/CORBA.h>
#include "simulator/corba/RDOCorba.hh"
#pragma warning(default: 4996)

namespace rdoCorba {

// --------------------------------------------------------------------------------
// -------------------- RDOThreadCorba
// --------------------------------------------------------------------------------
class RDOThreadCorba: public RDOThreadMT
{
public:
	RDOThreadCorba();

private:
	virtual ~RDOThreadCorba() {}; //! Чтобы нельзя было удалить через delete
	virtual void proc(REF(RDOMessageInfo) msg);
	virtual void idle();
	virtual void start();
	virtual void stop();

	PTR(CWinThread) thread_corbaRunThreadFun;
	static ruint corbaRunThreadFun(PTR(void) pParam);
};

} //! namespace rdoCorba

#endif // CORBA_ENABLE

namespace rdo {
namespace service {
namespace simulation {

class RDOThreadSimulator;

}}} // namespace rdo::service::simulation

OPEN_RDO_RUNTIME_NAMESPACE

class RDOResult;

// --------------------------------------------------------------------------------
// -------------------- RDOThreadRunTime
// --------------------------------------------------------------------------------
OBJECT(RDOThreadRunTime)
	IS  INSTANCE_OF      (RDOThreadMT )
	AND IMPLEMENTATION_OF(IThreadProxy)
{
DECLARE_FACTORY(RDOThreadRunTime);
public:
	rbool runtimeError() const;

	struct GetFrame
	{
		PTR(rdo::animation::Frame) m_pFrame;
		ruint                      m_number;

		GetFrame(PTR(rdo::animation::Frame) pFrame, ruint number)
			: m_pFrame(pFrame)
			, m_number(number)
		{}
	};

	struct FrameAreaDown
	{
		ruint   m_number;
		tstring m_name;

		FrameAreaDown(ruint number, CREF(tstring) name)
			: m_number(number)
			, m_name  (name  )
		{}
	};

private:
	PTR(rdo::service::simulation::RDOThreadSimulator)  m_pSimulator;
	rbool                                              m_runtimeError;
	ruint64                                            m_timeStart;

	RDOThreadRunTime();
	virtual ~RDOThreadRunTime() //! Чтобы нельзя было удалить через delete
	{};

	virtual void proc   (REF(RDOMessageInfo) msg);
	virtual void idle   ();
	virtual void start  ();
	virtual void stop   ();
	virtual void destroy();

	void writeResultsInfo();

	void sendMessage(ThreadID threadID, ruint messageID, PTR(void) pParam);
};

CLOSE_RDO_RUNTIME_NAMESPACE

class RDOTrace;

OPEN_RDO_PARSER_NAMESPACE
PREDECLARE_POINTER(RDOParser);
CLOSE_RDO_PARSER_NAMESPACE

namespace rdo {
namespace service {
namespace simulation {

// --------------------------------------------------------------------------------
// -------------------- RDOThreadSimulator
// --------------------------------------------------------------------------------
class RDOThreadSimulator: public RDOThreadMT
{
friend class rdo::runtime::RDOThreadRunTime;
friend class RDORuntimeTracer;
friend class RDOSimResulter;

public:
	struct RTP
	{
		struct Param
		{
			tstring m_name;
		};
		typedef std::vector<Param> ParamList;

		tstring   m_name;
		ParamList m_params;
	};

	struct RSS
	{
		tstring m_name;
	};

	struct GetRTP: public std::vector<RTP>
	{};
	struct GetRSS: public std::vector<RSS>
	{};

private:
	rdo::compiler::parser::LPRDOParser    m_pParser;
	rdo::runtime::LPRDORuntime            m_pRuntime;
	rbool                                 m_canTrace;
	rdo::compiler::gui::LPProcGUIProcess  m_pGUIProcess;
	rdo::compiler::gui::LPProcGUIBlock    m_pBlock;
	rdo::runtime::LPRDOThreadRunTime      m_pThreadRuntime;
	rdo::simulation::report::RDOExitCode  m_exitCode;

	void terminateModel();
	void closeModel    ();

	ShowMode m_showMode; //! current show mode
	double   m_showRate; //! current show mode

	std::stringstream m_resultString;
	std::stringstream m_resultInfoString;


#ifdef CORBA_ENABLE

//	void corbaGetRTPcount(REF(::CORBA::Long) rtp_count);
//	void corbaGetRTPParamscount(REF(rdo::compiler::parser::RDOCorba::PARAM_count) params_count);
	void corbaGetRTP(REF(rdo::compiler::parser::RDOCorba::GetRTP_var) my_rtpList);
	void corbaGetRSS(REF(rdo::compiler::parser::RDOCorba::GetRSS_var) my_rssList);

#endif // CORBA_ENABLE

protected:
	virtual ~RDOThreadSimulator(); //! Чтобы нельзя было удалить через delete помещаем его в protected

	virtual void proc(REF(RDOMessageInfo) msg);

	rbool parseModel();
	void  runModel  ();
	void  stopModel ();

	typedef std::vector<rdo::simulation::report::FileMessage> SyntaxMessageList;
	SyntaxMessageList getErrors();

	void codeCompletion();

	void corbaGetRTP(PTR(GetRTP) RTPList);
	void corbaGetRSS(PTR(GetRSS) RSSList);

public:
	RDOThreadSimulator();

	void parseSMRFileInfo(REF(rdo::converter::smr2rdox::RDOSMRFileInfo) info);

	ShowMode getInitialShowMode   () const;
	int      getInitialFrameNumber() const;
	double   getInitialShowRate   () const;

	struct GetList
	{
		enum Type
		{
			frames,
			bitmaps
		};

		typedef std::list<tstring> StringList;

		Type            m_type;
		PTR(StringList) m_list;

		GetList(Type type, PTR(StringList) list)
			: m_type(type)
			, m_list(list)
		{}
	};
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadCodeComp
// --------------------------------------------------------------------------------
class RDOThreadCodeComp: public RDOThreadMT
{
protected:
	rdo::compiler::parser::LPRDOParser m_pParser;

	virtual ~RDOThreadCodeComp(); //! Чтобы нельзя было удалить через delete помещаем его в protected
	virtual void proc(REF(RDOMessageInfo) msg);

public:
	RDOThreadCodeComp();

	struct GetCodeComp: public boost::noncopyable
	{
		rdoModelObjects::RDOFileType m_file;
		int                          m_pos_x;
		int                          m_pos_y;
		REF(tstring)                 m_result;

		GetCodeComp(rdoModelObjects::RDOFileType file, int pos_x, int pos_y, REF(tstring) result)
			: m_file  (file  )
			, m_pos_x (pos_x )
			, m_pos_y (pos_y )
			, m_result(result)
		{}
	};
};

}}} // namespace rdo::service::simulation

#endif // _LIB_SERVICE_SIMULATOR_H_
