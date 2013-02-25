/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_pattern.cpp
  \author    ������� �����
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_pattern.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

Pattern::Pattern(Kind kind)
	: ChartTreeItem()
	, m_kind(kind)
{}

Pattern::~Pattern()
{}

CREF(QString) Pattern::getName() const
{
	return m_name;
}

void Pattern::setName(CREF(QString) name)
{
	m_name = name;
}

Pattern::Kind Pattern::getKind() const
{
	return m_kind;
}