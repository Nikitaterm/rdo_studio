/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/view_preferences.cpp
  \author    ������� ������� (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/view_preferences.h"
#include "app/rdo_studio/res/build_version.h"
#include "app/rdo_studio/src/application.h"
#include "simulator/report/build_edit_line_info.h"
// --------------------------------------------------------------------------------
using namespace rdo::simulation::report;
using namespace rdoEditCtrl;
using namespace rdoEditor;
using namespace rdo::gui::tracer;

ViewPreferences::ViewPreferences(PTR(QWidget) pParent)
	: QDialog(pParent)
{
	setupUi(this);

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButton()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(onCancelButton()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(onApplyButton()));
	
	m_setup           = studioApp.getFileAssociationSetup();
	m_checkInFuture   = studioApp.getFileAssociationCheckInFuture();
	m_openLastProject = studioApp.getOpenLastProject();
	m_showFullName    = studioApp.getShowCaptionFullName();

	style_editor.init();
	style_build.init();
	style_debug.init();
	style_trace.init();
	style_results.init();
	style_find.init();

	style_editor  = studioApp.getStyle()->style_editor;
	style_build   = studioApp.getStyle()->style_build;
	style_debug   = studioApp.getStyle()->style_debug;
	style_trace   = studioApp.getStyle()->style_trace;
	style_results = studioApp.getStyle()->style_results;
	style_find    = studioApp.getStyle()->style_find;

	fontComboBox->setEditable(false);
	//������� "��������"
	connect(setupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onSetup(int)));
	connect(checkInFutureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckInFuture(int)));
	connect(openLastProjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onOpenLastProject(int)));
	connect(showFullNameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onShowFullName(int)));
	//������� "��������"
	connect(checkBoxCodeCompUse, SIGNAL(stateChanged(int)), this, SLOT(onCodeCompUse(int)));
	connect(radioButtonFullList, SIGNAL(toggled(bool)), this, SLOT(onCodeCompShowFullList(bool)));
	connect(radioButtonNearestWords, SIGNAL(toggled(bool)), this, SLOT(onCodeCompShowFullList(bool)));
	connect(checkBoxMarginFold, SIGNAL(stateChanged(int)), this, SLOT(onMarginFold(int)));
	connect(checkBoxMarginBookmark, SIGNAL(stateChanged(int)), this, SLOT(onMarginBookmark(int)));
	connect(checkBoxMarginLineNum, SIGNAL(stateChanged(int)), this, SLOT(onMarginLineNumber(int)));
	//������� "���������"
	tabSizeLineEdit->setValidator(new QIntValidator(1, 100, this));
	tabSizeLineEdit->setText(QString::number(style_editor.tab->tabSize));
	indentSizeLineEdit->setValidator(new QIntValidator(1, 100, this));
	indentSizeLineEdit->setText(QString::number(style_editor.tab->indentSize));
	connect(useTabSymbolCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onUseTabSymbol(int)));
	connect(indentAsTabcheckBox, SIGNAL(stateChanged(int)), this, SLOT(onIndentAsTab(int)));
	connect(autoIndentCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onAutoIndent(int)));
	connect(eraseWithTabRadioButton, SIGNAL(toggled(bool)), this, SLOT(onEraseWithTab(bool)));
	connect(tabSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onTabSize(const QString&)));
	connect(indentSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onIndentSize(const QString&)));
	//������� "����� � ����"
	stackedWidget->setCurrentWidget(pageRoot);
	createTree();
	createPreview();

	switchPreviewComboBox->addItem("Editor",  IT_EDITOR);
	switchPreviewComboBox->addItem("Build",   IT_BUILD);
	switchPreviewComboBox->addItem("Debug",   IT_DEBUG);
	switchPreviewComboBox->addItem("Tracer",  IT_LOG);
	switchPreviewComboBox->addItem("Results", IT_RESULT);
	switchPreviewComboBox->addItem("Find",    IT_FIND);
	switchPreviewComboBox->addItem("Chart",   IT_CHART);
	switchPreviewComboBox->addItem("Frame",   IT_FRAME);

	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeWidgetItemActivated(QTreeWidgetItem*, int)));
	connect(switchPreviewComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchPreviewComboBox(int)));
	connect(fontSizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFontSize(int)));
	connect(boldCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFontBold(int)));
	connect(italicCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFontBold(int)));
	connect(underlineCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFontBold(int)));

	updateDialog();
}

void ViewPreferences::onOkButton()
{
	apply();
	done(Accepted);
}

void ViewPreferences::onCancelButton()
{
	done(Rejected);
}

void ViewPreferences::onApplyButton()
{
	apply();
}

void ViewPreferences::onCodeCompUse(int state)
{
	style_editor.autoComplete->useAutoComplete = state;
	
	switch(state)
	{
	case 0:
		radioButtonFullList->setEnabled(false);
		radioButtonNearestWords->setEnabled(false);
		break;
	case 2:
		radioButtonFullList->setEnabled(true);
		radioButtonNearestWords->setEnabled(true);
		break;
	}

}

void ViewPreferences::onCodeCompShowFullList(bool state)
{
	UNUSED(state);

	if(radioButtonFullList->isChecked())
		style_editor.autoComplete->showFullList = true;
	if(radioButtonNearestWords->isChecked())
		style_editor.autoComplete->showFullList = false;
}

void ViewPreferences::onMarginFold(int state)
{
	style_editor.margin->fold = state;
}

void ViewPreferences::onMarginBookmark(int state)
{
	style_editor.margin->bookmark = state;
}

void ViewPreferences::onMarginLineNumber(int state)
{
	style_editor.margin->lineNumber = state;
}

void ViewPreferences::onUseTabSymbol(int state)
{
	style_editor.tab->backspaceUntabs = state;
}

void ViewPreferences::onIndentAsTab(int state)
{
	style_editor.tab->tabIndents = state;
}

void ViewPreferences::onAutoIndent(int state)
{
	style_editor.tab->autoIndent = state;
}

void ViewPreferences::onEraseWithTab(bool state)
{
	UNUSED(state);

	if(eraseWithTabRadioButton->isChecked())
		style_editor.tab->useTabs = true;
	if(eraseWithIndentRadioButton->isChecked())
		style_editor.tab->useTabs = false;
}

void ViewPreferences::onTabSize(const QString& text)
{
	style_editor.tab->tabSize = text.toInt();
}

void ViewPreferences::onIndentSize(const QString& text)
{
	style_editor.tab->indentSize = text.toInt();
}

void ViewPreferences::onSetup(int state)
{
	m_setup = state;
}

void ViewPreferences::onCheckInFuture(int state)
{
	m_checkInFuture = state;
}

void ViewPreferences::onOpenLastProject(int state)
{
	m_openLastProject = state;
}

void ViewPreferences::onShowFullName(int state)
{
	m_showFullName = state;
}

void ViewPreferences::onTreeWidgetItemActivated(QTreeWidgetItem* item, int column)
{
	stackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt());
	if(item->data(column, Qt::UserRole).toInt() != 0)
	{
		switchPreviewComboBox->setCurrentIndex(item->data(column, Qt::UserRole).toInt() - 1);
	}
	previewStackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt() - 1);
}

void ViewPreferences::onSwitchPreviewComboBox(int index)
{
	previewStackedWidget->setCurrentIndex(switchPreviewComboBox->itemData(index, Qt::UserRole).toInt() - 1);
}

void ViewPreferences::onFontSize(int index)
{
	UNUSED(index);
	int size = fontSizeComboBox->currentText().toInt();
	switch(treeWidget->currentItem()->data(0, Qt::UserRole).toInt())
	{
	case IT_ROOT:
		style_editor.font->size  = size;
		style_build.font->size   = size;
		style_debug.font->size   = size;
		style_trace.font->size   = size;
		style_results.font->size = size;
		style_find.font->size    = size;
		//style_chart.font->size   = size;
		//style_frame.font->size   = size;
		break;
	case IT_EDITOR:
		style_editor.font->size = size;
		break;
	case IT_BUILD:
		style_build.font->size = size;
		break;
	case IT_DEBUG:
		style_debug.font->size   = size;
		break;
	case IT_LOG:
		style_trace.font->size = size;
		break;
	case IT_RESULT:
		style_results.font->size = size;
		break;
	case IT_FIND:
		style_find.font->size    = size;
		break;
	case IT_CHART:
		//style_chart.font->size   = size;
		break;
	case IT_FRAME:
		//style_frame.font->size   = size;
		break;
	}

	updatePreview();
}

void ViewPreferences::onFontBold(int state)
{

}

void ViewPreferences::onFontItalic(int state)
{

}

void ViewPreferences::onFontUnderline(int state)
{

}

void ViewPreferences::updateDialog()
{
	setupCheckBox->setCheckState(m_setup
		? Qt::Checked
		: Qt::Unchecked
		);
	checkInFutureCheckBox->setCheckState(m_checkInFuture
		? Qt::Checked
		: Qt::Unchecked
		);
	openLastProjectCheckBox->setCheckState(m_openLastProject
		? Qt::Checked
		: Qt::Unchecked
		);
	showFullNameCheckBox->setCheckState(m_showFullName
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxCodeCompUse->setCheckState(style_editor.autoComplete->useAutoComplete
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginFold->setCheckState(style_editor.margin->fold
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginBookmark->setCheckState(style_editor.margin->bookmark
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginLineNum->setCheckState(style_editor.margin->lineNumber
		? Qt::Checked
		: Qt::Unchecked
		);
	style_editor.autoComplete->showFullList
		? radioButtonFullList->toggle()
		: radioButtonNearestWords->toggle();	
	useTabSymbolCheckBox->setCheckState(style_editor.tab->backspaceUntabs
		? Qt::Checked
		: Qt::Unchecked
		);
	indentAsTabcheckBox->setCheckState(style_editor.tab->tabIndents
		? Qt::Checked
		: Qt::Unchecked
		);
	autoIndentCheckBox->setCheckState(style_editor.tab->autoIndent
		? Qt::Checked
		: Qt::Unchecked
		);
	style_editor.tab->useTabs
		? eraseWithTabRadioButton->toggle()
		: eraseWithIndentRadioButton->toggle();	
}

void ViewPreferences::updatePreview()
{
	preview_editor->setEditorStyle(&style_editor);
	preview_editor->repaint();

	preview_build->setEditorStyle(&style_build);
	preview_build->repaint();

	preview_debug->setEditorStyle(&style_debug);
	preview_debug->repaint();

	//preview_trace->view().setStyle(&style_trace);
	//preview_trace->repaint();

	preview_results->setEditorStyle(&style_results);
	preview_results->repaint();

	preview_find->setEditorStyle(&style_find);
	preview_find->repaint();
}

void ViewPreferences::createPreview()
{
	preview_editor = new RDOEditorEdit(previewStackedWidget->currentWidget(), previewStackedWidget->currentWidget());
	ASSERT(preview_editor);
	preview_editor->setEditorStyle(&style_editor);
	preview_editor->setCanClearErrorLine(false);
	preview_editor->appendText(rdo::format(IDS_COLORSTYLE_EDITOR_SAMPLE));
	preview_editor->scrollToLine(0);
	preview_editor->setReadOnly(true);
	preview_editor->bookmarkToggle();
	preview_editor->setErrorLine(preview_editor->getLineCount() - 1);
	previewStackedWidget->addWidget(preview_editor);

	preview_build = new RDOBuildEdit(previewStackedWidget->currentWidget());
	preview_build->setEditorStyle(&style_build);
	preview_build->appendLine(new BuildEditLineInfo(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE1)));
	preview_build->appendLine(new BuildEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE2), rdoModelObjects::PAT, 40, 0)));
	preview_build->appendLine(new BuildEditLineInfo(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE3)));
	preview_build->gotoNext();
	previewStackedWidget->addWidget(preview_build);

	preview_debug = new RDODebugEdit(previewStackedWidget->currentWidget());
	preview_debug->setEditorStyle(&style_debug);
	preview_debug->appendLine(rdo::format(IDS_COLORSTYLE_DEBUG_SAMPLE));
	previewStackedWidget->addWidget(preview_debug);

	preview_trace = new LogMainWnd(previewStackedWidget->currentWidget());
	preview_trace->view().setStyle(&studioApp.getStyle()->style_trace);
	preview_trace->view().setFocusOnly(true);
	preview_trace->view().setText(rdo::format(IDS_COLORSTYLE_LOG_SAMPLE));
	preview_trace->view().selectLine(0);
	previewStackedWidget->addWidget(preview_trace);

	preview_results = new RDOEditorResults(previewStackedWidget->currentWidget());
	preview_results->setEditorStyle(&style_results);
	preview_results->setReadOnly(false);
	preview_results->replaceCurrent(rdo::format(IDS_COLORSTYLE_RESULTS_SAMPLE), 0);
	preview_results->setReadOnly(true);
	previewStackedWidget->addWidget(preview_results);

	preview_find = new RDOFindEdit(previewStackedWidget->currentWidget());
	preview_find->setEditorStyle(&style_find);
	preview_find->setKeyword("$Time");
	preview_find->appendLine(new LogEditLineInfo(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE1)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE2), rdoModelObjects::PAT, 3, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE3), rdoModelObjects::PAT, 13, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE4)));
	preview_find->gotoNext();
	previewStackedWidget->addWidget(preview_find);
}

void ViewPreferences::createTree()
{
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderHidden(true);
	treeWidget->setRootIsDecorated(false);

	m_pRoot = new QTreeWidgetItem(treeWidget);
	m_pRoot->setText(0, "��� ����");
	m_pRoot->setData(0, Qt::UserRole, IT_ROOT);

	m_pText      = createTreeItem(m_pRoot, "�������� �����",   IT_EDITOR);
	m_pCompile   = createTreeItem(m_pRoot, "���� ����������",  IT_BUILD);
	m_pDebug     = createTreeItem(m_pRoot, "���� �������",     IT_DEBUG);
	m_pTrace     = createTreeItem(m_pRoot, "���� �����������", IT_LOG);
	m_pResult    = createTreeItem(m_pRoot, "���� �����������", IT_RESULT);
	m_pSearch    = createTreeItem(m_pRoot, "���� ������",      IT_FIND);
	m_pChart     = createTreeItem(m_pRoot, "���� ��������",    IT_CHART);
	m_pAnimation = createTreeItem(m_pRoot, "���� ��������",    IT_FRAME);

	m_pRoot->setExpanded(true);

	m_pPlainText = createTreeItem(m_pText, "������������ �����", IT_EDITOR_PLAINTEXT);
	m_pVariable  = createTreeItem(m_pText, "����������",         IT_EDITOR_IDENTIFICATOR);
	m_pKeyword   = createTreeItem(m_pText, "�������� �����",     IT_EDITOR_KEYWORD);
	m_pFunction  = createTreeItem(m_pText, "�������",            IT_EDITOR_FUNCTION);
	m_pTraceText = createTreeItem(m_pText, "�����������",        IT_EDITOR_TRACE);
	m_pColor     = createTreeItem(m_pText, "����",               IT_EDITOR_COLOR);
	m_pComment   = createTreeItem(m_pText, "�����������",        IT_EDITOR_COMMENT);
	m_pNumber    = createTreeItem(m_pText, "�����",              IT_EDITOR_NUMBER);
	m_pString    = createTreeItem(m_pText, "������",             IT_EDITOR_STRING);
	m_pOperator  = createTreeItem(m_pText, "��������",           IT_EDITOR_OPERATOR);
	m_pCaret     = createTreeItem(m_pText, "�������",            IT_EDITOR_CARET);
	m_pSelection = createTreeItem(m_pText, "���������",          IT_EDITOR_TEXTSELECTION);
	m_pBookmark  = createTreeItem(m_pText, "��������",           IT_EDITOR_BOOKMARK);
	m_pGroup     = createTreeItem(m_pText, "������",             IT_EDITOR_FOLD);
	m_pError     = createTreeItem(m_pText, "������",             IT_EDITOR_ERROR);

	m_pTextCompile      = createTreeItem(m_pCompile, "�����",             IT_BUILD_TEXT);
	m_pSelectedString   = createTreeItem(m_pCompile, "���������� ������", IT_BUILD_SELECTEDLINE);
	m_pCaretCompile     = createTreeItem(m_pCompile, "�������",           IT_EDITOR_CARET);
	m_pSelectionCompile = createTreeItem(m_pCompile, "���������",         IT_EDITOR_TEXTSELECTION);
	m_pBookmarkCompile  = createTreeItem(m_pCompile, "��������",          IT_EDITOR_BOOKMARK);

	m_pTextDebug      = createTreeItem(m_pDebug, "�����",     IT_BUILD_TEXT);
	m_pCaretDebug     = createTreeItem(m_pDebug, "�������",   IT_EDITOR_CARET);
	m_pSelectionDebug = createTreeItem(m_pDebug, "���������", IT_EDITOR_TEXTSELECTION);
	m_pBookmarkDebug  = createTreeItem(m_pDebug, "��������",  IT_EDITOR_BOOKMARK);

	m_pES     = createTreeItem(m_pTrace, "��������� ������� (ES)",                         IT_LOG_ES);
	m_pEB     = createTreeItem(m_pTrace, "������ �������� (EB)",                           IT_LOG_EB);
	m_pEF     = createTreeItem(m_pTrace, "��������� �������� (EF)",                        IT_LOG_EF);
	m_pEI     = createTreeItem(m_pTrace, "������������ ������� (EI)",                      IT_LOG_EI);
	m_pER     = createTreeItem(m_pTrace, "������������� ������� (ER)",                     IT_LOG_ER);
	m_pRC     = createTreeItem(m_pTrace, "�������� ������� (RC)",                          IT_LOG_RC);
	m_pRE     = createTreeItem(m_pTrace, "�������� ������� (RE)",                          IT_LOG_RE);
	m_pRK     = createTreeItem(m_pTrace, "��������� ��������� ������� (RK)",               IT_LOG_RK);
	m_pV      = createTreeItem(m_pTrace, "����������� ������� (V)",                        IT_LOG_V);
	m_pStatus = createTreeItem(m_pTrace, "������ ��������� ������������� ($Status)",       IT_LOG_STATUS);
	m_pDPS    = createTreeItem(m_pTrace, "���������� �� ������ �� ����� (DPS)",            IT_LOG_DPS);
	m_pSB     = createTreeItem(m_pTrace, "������ ������ (SB)",                             IT_LOG_SB);
	m_pSO     = createTreeItem(m_pTrace, "����������� ������������ ������� (SO)",          IT_LOG_SO);
	m_pSTN    = createTreeItem(m_pTrace, "������� ������� (STN)",                          IT_LOG_STN);
	m_pSTD    = createTreeItem(m_pTrace, "������� ������� (STD)",                          IT_LOG_STD);
	m_pSTR    = createTreeItem(m_pTrace, "������� ������� (STR)",                          IT_LOG_STR);
	m_pSRC    = createTreeItem(m_pTrace, "�������� ������� (��� ������) (SRC)",            IT_LOG_SRC);
	m_pSRE    = createTreeItem(m_pTrace, "�������� ������� (��� ������) (SRE)",            IT_LOG_SRE);
	m_pSRK    = createTreeItem(m_pTrace, "��������� ��������� ������� (��� ������) (SRK)", IT_LOG_SRK);
	m_pSD     = createTreeItem(m_pTrace, "����������� ������� (SD)",                       IT_LOG_SD);
	m_pSES    = createTreeItem(m_pTrace, "���������� ������ (SES)",                        IT_LOG_SES);
	m_pSEN    = createTreeItem(m_pTrace, "���������� ������ (SEN)",                        IT_LOG_SEN);
	m_pSEM    = createTreeItem(m_pTrace, "���������� ������ (SEM)",                        IT_LOG_SEM);
	m_pSEF    = createTreeItem(m_pTrace, "���������� ������ (SEF)",                        IT_LOG_SEF);
	m_pSEU    = createTreeItem(m_pTrace, "���������� ������ (SEU)",                        IT_LOG_SEU);

	m_pPlainTextResult = createTreeItem(m_pResult, "�������� �����", IT_EDITOR_PLAINTEXT);
	m_pVariableResult  = createTreeItem(m_pResult, "����������",     IT_EDITOR_IDENTIFICATOR);
	m_pKeywordResult   = createTreeItem(m_pResult, "�������� �����", IT_EDITOR_KEYWORD);
	m_pNumberResult    = createTreeItem(m_pResult, "�����",          IT_EDITOR_NUMBER);
	m_pStringResult    = createTreeItem(m_pResult, "������",         IT_EDITOR_STRING);
	m_pOperatorResult  = createTreeItem(m_pResult, "��������",       IT_EDITOR_OPERATOR);
	m_pCaretResult     = createTreeItem(m_pResult, "�������",        IT_EDITOR_CARET);
	m_pSelectionResult = createTreeItem(m_pResult, "���������",      IT_EDITOR_TEXTSELECTION);
	m_pBookmarkResult  = createTreeItem(m_pResult, "��������",       IT_EDITOR_BOOKMARK);

	m_pTextSearch           = createTreeItem(m_pSearch, "�����",             IT_BUILD_TEXT);
	m_pStringSearch         = createTreeItem(m_pSearch, "������ ��� ������", IT_FIND_SEARCHTEXT);
	m_pSelectedStringSearch = createTreeItem(m_pSearch, "���������� ������", IT_BUILD_SELECTEDLINE);
	m_pCaretSearch          = createTreeItem(m_pSearch, "�������",           IT_EDITOR_CARET);
	m_pSelectionSearch      = createTreeItem(m_pSearch, "���������",         IT_EDITOR_TEXTSELECTION);
	m_pBookmarkSearch       = createTreeItem(m_pSearch, "��������",          IT_EDITOR_BOOKMARK);

	m_pAxis   = createTreeItem(m_pChart, "���",       IT_CHART_AXIS);
	m_pTitle  = createTreeItem(m_pChart, "���������", IT_CHART_TITLE);
	m_pLegend = createTreeItem(m_pChart, "�������",   IT_CHART_LEGEND);
	m_pGraph  = createTreeItem(m_pChart, "������",    IT_CHART_CHART);
	m_pTime   = createTreeItem(m_pChart, "�����",     IT_CHART_TIME);

	m_pEdgingColor     = createTreeItem(m_pAnimation, "���� ���������",               IT_FRAME_BORDER);
	m_pBackgroundColor = createTreeItem(m_pAnimation, "���� ���� �� ��������� �����", IT_FRAME_BACKGROUND);

	treeWidget->setCurrentItem(m_pRoot);
}

PTR(QTreeWidgetItem) ViewPreferences::createTreeItem(PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType)
{
	PTR(QTreeWidgetItem) item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	item->setData(0, Qt::UserRole, QVariant(itemType));
	return item;
}

void ViewPreferences::apply()
{
	studioApp.getStyle()->style_editor  = style_editor;
	studioApp.setFileAssociationSetup(m_setup);
	studioApp.setFileAssociationCheckInFuture(m_checkInFuture);
	studioApp.setOpenLastProject(m_openLastProject);
	studioApp.setShowCaptionFullName(m_showFullName);
	studioApp.getStyle()->updateAllStyles();
}