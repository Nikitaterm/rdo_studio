/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorresults.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTS_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresultsstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorResults
// --------------------------------------------------------------------------------
class RDOEditorResults: public RDOEditorBaseEdit
{
public:
	RDOEditorResults(PTR(QWidget) pParent);
	virtual ~RDOEditorResults();

	void setEditorStyle(PTR(RDOEditorResultsStyle) pStyle);

private:
	typedef QWidget parent_type;

	QMenu* m_pPopupMenu;

	virtual void onHelpContext();
	virtual void mousePressEvent(QMouseEvent* pEvent);
};

} // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTS_H_
