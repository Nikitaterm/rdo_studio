/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree_item.cpp
  \author    ������� �����
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

ChartTreeItem::ChartTreeItem(rbool drawable)
	: m_pCtrlItem(NULL    )
	, m_drawable (drawable)
{}

ChartTreeItem::~ChartTreeItem()
{}

void ChartTreeItem::setCtrlItem(PTR(QTreeWidgetItem) pCtrlItem)
{
	m_pCtrlItem = pCtrlItem;
}

REF(QTreeWidgetItem) ChartTreeItem::getCtrlItem()
{
	ASSERT(m_pCtrlItem)
	return *m_pCtrlItem;
}

rbool ChartTreeItem::isDrawable() const
{
	return m_drawable;
}