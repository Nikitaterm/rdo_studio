#include "stdafx.h"
#include "rdostudiooutput.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiomodel.h"
#include "edit_ctrls/rdobuildedit.h"
#include "edit_ctrls/rdodebugedit.h"
#include "edit_ctrls/rdofindedit.h"
#include "rdo_edit/rdoeditorresults.h"
#include "rdo_edit/rdoeditortabctrl.h"
#include "rdo_tracer/rdotracer.h"
#include "rdo_tracer/tracer_ctrls/rdotracerlogctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOStudioOutput
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOutput, RDOStudioDockWnd)
	//{{AFX_MSG_MAP(RDOStudioOutput)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOutput::RDOStudioOutput()
{
}

RDOStudioOutput::~RDOStudioOutput()
{
	eraseMenu( &popupMenu );
}

int RDOStudioOutput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioDockWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, 0 );
	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();

	BOOL maximized;
	studioApp.mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 8, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 10, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 2, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 7, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 8, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 9, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 10, &popupMenu );

	build   = new RDOBuildEdit;
	debug   = new RDODebugEdit;
	trace   = tracer->createLog();
	results = new RDOEditorResults;
	find    = new RDOFindEdit;

	build->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	debug->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	trace->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	results->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	find->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );

	build->setEditorStyle( &studioApp.mainFrame->style_build );
	build->setPopupMenu( &popupMenu );

	debug->setEditorStyle( &studioApp.mainFrame->style_debug );
	debug->setPopupMenu( &popupMenu );

	trace->setStyle( &studioApp.mainFrame->style_trace );

	results->setEditorStyle( &studioApp.mainFrame->style_results );
	results->setPopupMenu( &popupMenu );

	find->setEditorStyle( &studioApp.mainFrame->style_find );
	find->setPopupMenu( &popupMenu );

	tab.insertItem( build, "Build" );
	tab.insertItem( debug, "Debug" );
	tab.insertItem( trace, "Trace" );
	tab.insertItem( results, "Results" );
	tab.insertItem( find, "Find in Model" );

	return 0;
}

BOOL RDOStudioOutput::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( tab.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return RDOStudioDockWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void RDOStudioOutput::showBuild()
{
	HWND hwnd = ::GetFocus();
	tab.setCurrentItem( 0 );
	build->SetFocus();
	if ( hwnd ) {
		::SetFocus( hwnd );
	}
}

void RDOStudioOutput::showDebug()
{
	HWND hwnd = ::GetFocus();
	tab.setCurrentItem( 1 );
	debug->SetFocus();
	if ( hwnd ) {
		::SetFocus( hwnd );
	}
}

void RDOStudioOutput::showTrace()
{
	HWND hwnd = ::GetFocus();
	tab.setCurrentItem( 2 );
	trace->SetFocus();
	if ( hwnd ) {
		::SetFocus( hwnd );
	}
}

void RDOStudioOutput::showResults()
{
	HWND hwnd = ::GetFocus();
	tab.setCurrentItem( 3 );
	results->SetFocus();
	if ( hwnd ) {
		::SetFocus( hwnd );
	}
}

void RDOStudioOutput::showFind()
{
	HWND hwnd = ::GetFocus();
	tab.setCurrentItem( 4 );
	find->SetFocus();
	if ( hwnd ) {
		::SetFocus( hwnd );
	}
}

void RDOStudioOutput::clearBuild()
{
	build->clearAll();
}

void RDOStudioOutput::clearDebug()
{
	debug->clearAll();
}

void RDOStudioOutput::clearResults()
{
	results->clearAll();
}

void RDOStudioOutput::clearFind()
{
	find->clearAll();
}

void RDOStudioOutput::appendStringToBuild( const string& str, const rdoModelObjects::RDOFileType fileType, const int lineNumber, const bool error ) const
{
	RDOBuildEditLineInfo* line = new RDOBuildEditLineInfo( str, fileType, lineNumber, error );
	build->appendLine( line );
}

void RDOStudioOutput::appendStringToDebug( const string& str ) const
{
	debug->appendLine( str );
}

void RDOStudioOutput::appendStringToFind( const string& str, const rdoModelObjects::RDOFileType fileType, const int lineNumber, const int posInLine ) const
{
	RDOLogEditLineInfo* line = new RDOLogEditLineInfo( str, fileType, lineNumber, posInLine );
	find->appendLine( line );
}

void RDOStudioOutput::Tab::changeCurrentItem()
{
	studioApp.mainFrame->output.updateLogConnection();
}

void RDOStudioOutput::updateLogConnection() const
{
	int item = tab.getCurrentIndex();
	RDOLogEdit* log = NULL;
	if ( item == 0 ) {
		log = build;
	} else if ( item == 4 ) {
		log = find;
	}
	if ( log ) {
		rdoEditor::RDOEditorTabCtrl* editor_tab = model->getTab();
		if ( editor_tab ) {
			for ( int i = 0; i < editor_tab->getItemCount(); i++ ) {
				editor_tab->getItemEdit( i )->setLog( *log );
			}
		}
	}
}

void RDOStudioOutput::updateStyles() const
{
	build->setEditorStyle( &studioApp.mainFrame->style_build );
	debug->setEditorStyle( &studioApp.mainFrame->style_debug );
	trace->setStyle( &studioApp.mainFrame->style_trace );
	results->setEditorStyle( &studioApp.mainFrame->style_results );
	find->setEditorStyle( &studioApp.mainFrame->style_find );
}
