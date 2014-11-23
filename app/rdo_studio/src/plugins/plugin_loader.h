/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/src/plugins/plugin_loader.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_PLUGIN_LOADER_H_
#define _RDO_STUDIO_PLUGIN_LOADER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/plugins/plugin_info.h"
#include "app/rdo_studio/src/plugins/plugin_interface.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace plugin
	{
		class Loader;
	};
};

class rdo::plugin::Loader
{
public:
	Loader ();
	virtual ~Loader();

	const LPPluginInfoList& getPluginInfoList     () const;
	void                    stopPlugin            (const LPPluginInfo& pluginInfo);
	void                    startPlugin           (const LPPluginInfo& pluginInfo, const std::string& commandLine = "");
	void                    autoStartPlugins(const std::map<int, std::string>& options);
	void                    setPluginInfoList     (const PluginInfoList& value) const;
	void                    init                  (QWidget* pParent);

private:
	LPPluginInfoList m_pMergedPluginInfoList;
	QWidget*         m_pPluginsParent;

	QStringList       getFileList            (const QString& startDir) const;
	PluginInfoList    getMergedPluginInfoList() const;
	PluginInfoList    getPluginsHistory      () const;
	PluginInfoList    getCurrentPlugins      () const;

	PluginInterface*  loadPlugin             (QPluginLoader* pluginLoader) const;
	PluginInfo        generatePluginInfo     (PluginInterface* pluginInterface, QPluginLoader* pluginLoader) const;
	int               matchPluginInfo        (const PluginInfoList& list, const LPPluginInfo& pluginInfo) const;
};

#endif // _RDO_STUDIO_PLUGIN_LOADER_H_
