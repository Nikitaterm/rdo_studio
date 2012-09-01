/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseeditstyle.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditTheme
// --------------------------------------------------------------------------------
class RDOEditorBaseEditTheme: public rdoEditCtrl::RDOBaseEditTheme
{
public:
	RDOEditorBaseEditTheme();
	virtual ~RDOEditorBaseEditTheme();

	RDOEditorBaseEditTheme& operator =( const RDOEditorBaseEditTheme& theme );
	rbool operator ==( const RDOEditorBaseEditTheme& theme ) const;
	rbool operator !=( const RDOEditorBaseEditTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	COLORREF identifierColor;
	COLORREF keywordColor;
	COLORREF functionsColor;
	COLORREF traceColor;
	COLORREF colorColor;
	COLORREF commentColor;
	COLORREF numberColor;
	COLORREF stringColor;
	COLORREF operatorColor;

	rdoStyle::RDOStyleFont::style identifierStyle;
	rdoStyle::RDOStyleFont::style keywordStyle;
	rdoStyle::RDOStyleFont::style functionsStyle;
	rdoStyle::RDOStyleFont::style traceStyle;
	rdoStyle::RDOStyleFont::style colorStyle;
	rdoStyle::RDOStyleFont::style commentStyle;
	rdoStyle::RDOStyleFont::style numberStyle;
	rdoStyle::RDOStyleFont::style stringStyle;
	rdoStyle::RDOStyleFont::style operatorStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOEditorBaseEditTheme getDefaultTheme();
	static RDOEditorBaseEditTheme getCppTheme();
	static RDOEditorBaseEditTheme getPascalTheme();
	static RDOEditorBaseEditTheme getHtmlTheme();
	static RDOEditorBaseEditTheme getClassicTheme();
	static RDOEditorBaseEditTheme getTwilightTheme();
	static RDOEditorBaseEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditStyle
// --------------------------------------------------------------------------------
class RDOEditorBaseEditStyle: public rdoEditCtrl::RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOEditorBaseEditStyle();
	virtual ~RDOEditorBaseEditStyle();

	RDOEditorBaseEditStyle& operator =( const RDOEditorBaseEditStyle& style );
	rbool operator ==( const RDOEditorBaseEditStyle& style ) const;
	rbool operator !=( const RDOEditorBaseEditStyle& style ) const;
};

}; // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_