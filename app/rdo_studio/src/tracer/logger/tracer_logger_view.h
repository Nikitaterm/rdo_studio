/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_logger_view.h
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_LOGGER_VIEW_H_
#define _RDO_STUDIO_TRACER_LOGGER_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include <QEvent>
#include <QAbstractScrollArea>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/logger/tracer_logger_view_style.h"
#include "app/rdo_studio/src/action_activator/action_activator_widget.h"
#include "app/rdo_studio/src/dialog/find_dialog.h"
#include "ui/qt/headers/scroll_bar/scroll_bar.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- LogView
// --------------------------------------------------------------------------------
class LogView: public ActionActivatorWidget
{
Q_OBJECT

public:
	LogView(PTR(QAbstractScrollArea) pParent, PTR(LogStyle) pStyle);
	virtual ~LogView();

	void push_back (CREF(tstring) log );
	void setText   (tstring       text);
	void clear     ();
	void selectLine(rsint index);

	bool getFocusOnly() const;
	void setFocusOnly(bool value);

	CREF(LogStyle) getStyle() const;
	void setStyle(LogStyle* style, bool needRedraw = true);

	bool getDrawLog() const;
	void setDrawLog(bool value);

private:
	typedef  ActionActivatorWidget  parent_type;

	class StringList
	{
	public:
		typedef  std::list<QString>            List;
		typedef  List::const_iterator          const_iterator;
		typedef  List::const_reverse_iterator  const_reverse_iterator;

		StringList();

		void push_back(CREF(QString) value);

		const_iterator begin() const;
		const_iterator end  () const;

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend  () const;

		void   clear    ();
		rsint  count    () const;
		rsint  maxLegth () const;
		void   setCursor(rsint pos, rsint max);

		const_iterator         findString (rsint index) const;
		const_reverse_iterator rFindString(rsint index) const;

	private:
		List            m_list;
		rsint           m_count;
		List::iterator  m_cursorIt;
		rsint           m_cursor;
		rsint           m_maxLegth;
	};

	struct SubitemColors
	{
		typedef std::map<rsint, LogColorPair> List;

		List         m_colorList;
		bool         m_addingSubitems;
		LogColorPair m_parentColor;

		SubitemColors();
		SubitemColors(CREF(SubitemColors) subitemColors);
	};

	struct ScrollMetricVert: public ScrollMetric
	{
		rsint lastViewableLine;

		ScrollMetricVert();
		bool isVisible(rsint index) const;
		bool applyInc (rsint delta);
	};

	QAbstractScrollArea* m_pScrollArea;
	StringList           m_strings;
	LogStyle*            m_logStyle;
	rsint                m_lineHeight;
	rsint                m_charWidth;
	rsint                m_selectedLine;
	QMenu*               m_pPopupMenu;
	ScrollMetric         m_SM_X;
	ScrollMetricVert     m_SM_Y;
	SubitemColors        m_subitemColors;
	QRect                m_clientRect;
	QRect                m_prevWindowRect;
	rsint                m_fullRepaintLines;
	bool                 m_focusOnly;
	FindDialog*          m_pFindDialog;
	FindDialog::Settings m_findSettings;
	bool                 m_drawLog;
	QFont                m_font;

	bool getItemColors(CREF(QString) item, LogColorPair &colors) const;
	bool getItemColors(rsint index, LogColorPair &colors) const;

	rsint selectedLine() const;
	void  setSelectedLine(rsint selectedLine);

	QString     getString           (rsint index) const;
	QString     getSelected         () const;

	QScrollBar& getVertScrollBar    ();
	QScrollBar& getHorzScrollBar    ();
	void        updateScrollBars    ();
	bool        scrollVertically    (rsint inc);
	bool        scrollHorizontally  (rsint inc);

	bool        makeLineVisible     (rsint index);
	bool        isFullyVisible      (rsint index) const;
	QRect       getLineRect         (rsint index) const;
	void        repaintLine         (rsint index);

	void        setFont             ();
	void        updateActionFind    (bool activated);
	void        updateActionEditCopy(bool activated);
	void        updateCoordStatusBar(bool activated);
	bool        canCopy             () const;

	rsint       find                (bool searchDown);
	void        onFindDlgFind       (CREF(FindDialog::Settings) settings);
	void        onFindDlgClose      ();

	virtual void resizeEvent     (QResizeEvent* pEvent);
	virtual void paintEvent      (QPaintEvent*  pEvent);
	virtual void keyPressEvent   (QKeyEvent*    pEvent);
	virtual void wheelEvent      (QWheelEvent*  pEvent);
	virtual void mousePressEvent (QMouseEvent*  pEvent);
	virtual void contextMenuEvent(QContextMenuEvent* pEvent);

	virtual void onUpdateActions(bool activated);

private slots:
	void onVertScrollBarValueChanged(int value);
	void onHorzScrollBarValueChanged(int value);
	void onEditCopy                 ();
	void onSearchFind               ();
	void onSearchFindNext           ();
	void onSearchFindPrevious       ();
	void onHelpContext              ();
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_LOGGER_VIEW_H_
