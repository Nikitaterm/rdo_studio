/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_find.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QAction>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_find.h"
#include "simulator/report/log_edit_line_info.h"
// --------------------------------------------------------------------------------

DockFind::DockFind(QWidget* pParent)
	: DockFocusable("Поиск", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 110));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_find.png")));
}

DockFind::~DockFind()
{}

void DockFind::appendString(
	CREF(QString)                str,
	rdoModelObjects::RDOFileType fileType,
	int lineNumber, int posInLine)
{
	PTR(rdo::simulation::report::LogEditLineInfo) pLine = new rdo::simulation::report::LogEditLineInfo(
		rdo::simulation::report::FileMessage(
			str.toStdString(),
			fileType,
			lineNumber,
			posInLine
		)
	);
	getContext().appendLine(pLine);
}

void DockFind::clear()
{
	getContext().clearAll();
}

REF(DockFind::context_type) DockFind::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}