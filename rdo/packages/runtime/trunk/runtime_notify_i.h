/*!
  \copyright (c) RDO-Team, 2012
  \file      runtime_notify_i.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      08.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_NOTIFY_I_H_
#define _LIB_RUNTIME_NOTIFY_I_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

//! ��������� ����������� �� ��������
class INotify
{
public:
	virtual void notify(ruint message, PTR(void) pParam) = 0;
};
#define DECLARE_INotify \
	virtual void notify(ruint message, PTR(void) pParam);

#endif // _LIB_RUNTIME_NOTIFY_I_H_
