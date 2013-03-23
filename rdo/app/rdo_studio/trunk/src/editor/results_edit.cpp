/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      parser_edit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/results_edit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

Results::Results(QWidget* pParent)
	: super(pParent)
	, PopupMenu(pParent)
{
	kw0 = "TRUE FALSE";
	kw1 = "";
	kw2 = "";
	kw3 = "";

	setReadOnly( true );
}

Results::~Results()
{}

void Results::setEditorStyle(PTR(ResultsStyle) pStyle)
{
	super::setEditorStyle(pStyle);
}

void Results::onHelpContext()
{
	tstring keyword = getCurrentOrSelectedWord();
	tstring s = getAllKW();

	if (s.find_first_of(keyword) == tstring::npos || keyword.empty())
	{
		keyword = "pmv";
	}

	QByteArray ba;
	ba.append("activateKeyword ");
	ba.append(keyword.c_str());
	ba.append("\n");
	g_pApp->callQtAssistant(ba);
}

void Results::contextMenuEvent(QContextMenuEvent *pEvent)
{
	m_pPopupMenu->exec(pEvent->globalPos());
}