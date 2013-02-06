/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_windows_base.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      01.09.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MAIN_WINDOWS_BASE_H_
#define _RDO_STUDIO_MAIN_WINDOWS_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio/edit_ctrls/rdobaseeditstyle.h"
#include "app/rdo_studio/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio/src/tracer/ctrls/log_view_style.h"
#include "app/rdo_studio/src/style.h"
#include "app/rdo_studio/src/chart/chart_view_style.h"
#include "app/rdo_studio/src/frame/frame_style.h"
#include "app/rdo_studio/src/dock/dock_build.h"
#include "app/rdo_studio/src/dock/dock_debug.h"
#include "app/rdo_studio/src/dock/dock_trace.h"
#include "app/rdo_studio/src/dock/dock_results.h"
#include "app/rdo_studio/src/dock/dock_find.h"
#include "app/rdo_studio/src/dock/dock_chart_tree.h"
#include "app/rdo_studio/src/dock/dock_frame.h"
// --------------------------------------------------------------------------------

class MainWindowBase
{
public:
	rdoEditor::RDOEditorEditStyle     style_editor;
	rdoEditCtrl::RDOBuildEditStyle    style_build;
	rdoEditCtrl::RDOBaseEditStyle     style_debug;
	rdo::gui::tracer::LogStyle        style_trace;
	rdoEditor::RDOEditorResultsStyle  style_results;
	rdoEditCtrl::RDOFindEditStyle     style_find;
	RDOStudioFrameStyle               style_frame;
	RDOStudioChartViewStyle           style_chart;

	rbool isMDIMaximazed() const { return true; }

	REF(DockBuild)      getDockBuild    () { ASSERT(m_pDockBuild    ); return *m_pDockBuild;     }
	REF(DockDebug)      getDockDebug    () { ASSERT(m_pDockDebug    ); return *m_pDockDebug;     }
	REF(DockTrace)      getDockTrace    () { ASSERT(m_pDockTrace    ); return *m_pDockTrace;     }
	REF(DockResults)    getDockResults  () { ASSERT(m_pDockResults  ); return *m_pDockResults;   }
	REF(DockFind)       getDockFind     () { ASSERT(m_pDockFind     ); return *m_pDockFind;      }
	REF(DockChartTree)  getDockChartTree() { ASSERT(m_pDockChartTree); return *m_pDockChartTree; }
	REF(DockFrame)      getDockFrame    () { ASSERT(m_pDockFrame    ); return *m_pDockFrame;     }

	virtual void updateAllStyles() = 0;

	virtual void update_start() = 0;
	virtual void update_stop () = 0;

	virtual void addSubWindow              (QWidget* pWidget) = 0;
	virtual void activateSubWindow         (QWidget* pWidget) = 0;
	virtual void connectOnActivateSubWindow(QObject* pObject) = 0;

protected:
	PTR(DockBuild)      m_pDockBuild;
	PTR(DockDebug)      m_pDockDebug;
	PTR(DockTrace)      m_pDockTrace;
	PTR(DockResults)    m_pDockResults;
	PTR(DockFind)       m_pDockFind;
	PTR(DockChartTree)  m_pDockChartTree;
	PTR(DockFrame)      m_pDockFrame;
};

#endif // _RDO_STUDIO_MAIN_WINDOWS_BASE_H_