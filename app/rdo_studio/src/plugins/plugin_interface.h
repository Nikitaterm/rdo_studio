/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/src/plugins/plugininterface.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_PLUGIN_INTERFACE_H_
#define _RDO_STUDIO_PLUGIN_INTERFACE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QUuid>
#include <QString>
#include <QWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class PluginInterface
{
public:
	virtual ~PluginInterface() {}
	virtual QUuid   getGUID      () = 0;
	virtual QString getPluginName() = 0;
	virtual QString getAuthor    () = 0;
	virtual QString getVersion   () = 0;

	virtual void pluginStartAction(QWidget* parent) = 0;
	virtual void pluginStopAction (QWidget* parent) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "RDO_PLUGIN_INTERFACE");

#endif // _RDO_STUDIO_PLUGIN_INTERFACE_H_
