/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_trace_tree.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      06.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_TRACE_TREE_H_
#define _RDO_STUDIO_DOCK_TRACE_TREE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdockwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree.h"
// --------------------------------------------------------------------------------

class DockChartTree: public QDockWidget
{
public:
	typedef  ChartTree  context_type;

	DockChartTree(PTR(QWidget) pParent);
	virtual ~DockChartTree();
};

#endif // _RDO_STUDIO_DOCK_TRACE_TREE_H_