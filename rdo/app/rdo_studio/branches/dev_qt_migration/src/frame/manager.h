/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/manager.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_MANAGER_H_
#define _RDO_STUDIO_MFC_FRAME_MANAGER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <map>
#include <memory>
#include <QtGui/qmdisubwindow.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/frame/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
OPEN_RDO_ANIMATION_NAMESPACE
struct Frame;
CLOSE_RDO_ANIMATION_NAMESPACE

class RDOStudioFrameManager: QObject
{
Q_OBJECT

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertFrame (CREF(tstring) frameName );
	void insertBitmap(CREF(tstring) bitmapName);

	ruint findFrameIndex(const HTREEITEM             hitem   ) const;
	ruint findFrameIndex(CPTR(FrameAnimationWnd)     pView   ) const;
	ruint findFrameIndex(CPTR(FrameAnimationContent) pContent) const;

	rbool                   isShowing         () const;
	CREF(tstring)           getFrameName      (ruint index) const;
	PTR(FrameAnimationWnd)  getFrameView      (ruint index) const;
	PTR(FrameAnimationWnd)  getFrameViewFirst () const;
	ruint                   count             () const;
	rbool                   isChanged         ();

	void                    areaDown          (ruint frameIndex, CREF(QPoint) point) const;

	PTR(FrameAnimationWnd)  createView    (ruint index);
	void                    disconnectView(CPTR(FrameAnimationWnd) pView);
	void                    closeAll      ();
	void                    clear         ();
	void                    expand        () const;

	ruint getLastShowedFrame      () const;
	void  setLastShowedFrame      (ruint index);
	void  setCurrentShowingFrame  (ruint index);
	void  resetCurrentShowingFrame(ruint index);
	void  showFrame               (CPTRC(rdo::animation::Frame) pFrame, ruint index);
	void  showNextFrame           ();
	void  showPrevFrame           ();
	void  showFrame               (ruint index);
	rbool canShowNextFrame        () const;
	rbool canShowPrevFrame        () const;
	void  updateStyles            () const;

private:
	struct Frame
	{
		 Frame();
		~Frame();

		HTREEITEM                      m_hitem;
		tstring                        m_name;
		PTR(FrameAnimationWnd)         m_pView;
		PTR(FrameAnimationContent)     m_pContent;
		rdo::gui::animation::AreaList  m_areaList;

	private:
		void clear();
	};

	typedef  std::vector<PTR(Frame)>  FrameList;

	FrameList             m_frameList;
	rdo::gui::BitmapList  m_bitmapList;
	ruint                 m_lastShowedFrame;
	ruint                 m_currentShowingFrame;
	rbool                 m_changed;

private slots:
	void onSubWindowActivated(QMdiSubWindow* pWindow);
};

#endif // _RDO_STUDIO_MFC_FRAME_MANAGER_H_