#include "stdafx.h"
#include "rdoeditorbaseeditstyle.h"
#include "../edit_ctrls/sci/SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditor;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOEditorBaseEditTheme
// ----------------------------------------------------------------------------
RDOEditorBaseEditTheme::RDOEditorBaseEditTheme(): RDOBaseEditTheme()
{
	defaultColor    = RGB( 0x80, 0x80, 0x80 );
	identifierColor = RGB( 0x00, 0x00, 0x00 );
	keywordColor    = RGB( 0x00, 0x00, 0x00 );
	functionsColor  = RGB( 0x00, 0x80, 0x00 );
	traceColor      = RGB( 0x80, 0x00, 0x00 );
	commentColor    = RGB( 0x00, 0x80, 0x00 );
	numberColor     = RGB( 0x00, 0x00, 0x80 );
	stringColor     = RGB( 0x80, 0x00, 0x80 );
	operatorColor   = RGB( 0x00, 0x00, 0x00 );

	identifierStyle = RDOFS_NONE;
	keywordStyle    = RDOFS_BOLD;
	functionsStyle  = RDOFS_BOLD;
	traceStyle      = RDOFS_BOLD;
	commentStyle    = RDOFS_ITALIC;
	numberStyle     = RDOFS_NONE;
	stringStyle     = RDOFS_NONE;
	operatorStyle   = RDOFS_NONE;
}

RDOEditorBaseEditTheme::~RDOEditorBaseEditTheme()
{
}

RDOEditorBaseEditTheme& RDOEditorBaseEditTheme::operator =( const RDOEditorBaseEditTheme& theme )
{
	RDOBaseEditTheme::operator=( theme );

	identifierColor = theme.identifierColor;
	keywordColor    = theme.keywordColor;
	functionsColor  = theme.functionsColor;
	traceColor      = theme.traceColor;
	commentColor    = theme.commentColor;
	numberColor     = theme.numberColor;
	stringColor     = theme.stringColor;
	operatorColor   = theme.operatorColor;

	identifierStyle = theme.identifierStyle;
	keywordStyle    = theme.keywordStyle;
	functionsStyle  = theme.functionsStyle;
	traceStyle      = theme.traceStyle;
	commentStyle    = theme.commentStyle;
	numberStyle     = theme.numberStyle;
	stringStyle     = theme.stringStyle;
	operatorStyle   = theme.operatorStyle;

	return *this;
}

bool RDOEditorBaseEditTheme::operator ==( const RDOEditorBaseEditTheme& theme ) const
{
	bool flag = RDOBaseEditTheme::operator==( theme );

	if ( flag ) flag &= identifierColor == theme.identifierColor &&
	                    keywordColor    == theme.keywordColor &&
	                    functionsColor  == theme.functionsColor &&
	                    traceColor      == theme.traceColor &&
	                    commentColor    == theme.commentColor &&
	                    numberColor     == theme.numberColor &&
	                    stringColor     == theme.stringColor &&
	                    operatorColor   == theme.operatorColor &&

	                    identifierStyle == theme.identifierStyle &&
	                    keywordStyle    == theme.keywordStyle &&
	                    functionsStyle  == theme.functionsStyle &&
	                    traceStyle      == theme.traceStyle &&
	                    commentStyle    == theme.commentStyle &&
	                    numberStyle     == theme.numberStyle &&
	                    stringStyle     == theme.stringStyle &&
	                    operatorStyle   == theme.operatorStyle;
	return flag;
}

bool RDOEditorBaseEditTheme::operator !=( const RDOEditorBaseEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOEditorBaseEditTheme::load( string regPath )
{
	RDOBaseEditTheme::load( regPath );

	regPath += "theme";
	identifierColor        = AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	keywordColor           = AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	functionsColor         = AfxGetApp()->GetProfileInt( regPath.c_str(), "functionsColor", functionsColor );
	traceColor             = AfxGetApp()->GetProfileInt( regPath.c_str(), "traceColor", traceColor );
	commentColor           = AfxGetApp()->GetProfileInt( regPath.c_str(), "commentColor", commentColor );
	numberColor            = AfxGetApp()->GetProfileInt( regPath.c_str(), "numberColor", numberColor );
	stringColor            = AfxGetApp()->GetProfileInt( regPath.c_str(), "stringColor", stringColor );
	operatorColor          = AfxGetApp()->GetProfileInt( regPath.c_str(), "operatorColor", operatorColor );
	identifierStyle        = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierStyle", identifierStyle ));
	keywordStyle           = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordStyle", keywordStyle ));
	functionsStyle         = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "functionsStyle", functionsStyle ));
	traceStyle             = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "traceStyle", traceStyle ));
	commentStyle           = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "commentStyle", commentStyle ));
	numberStyle            = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "numberStyle", numberStyle ));
	stringStyle            = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "stringStyle", stringStyle ));
	operatorStyle          = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "operatorStyle", operatorStyle ));
}

void RDOEditorBaseEditTheme::save( string regPath ) const
{
	RDOBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "functionsColor", functionsColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "traceColor", traceColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "commentColor", commentColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "numberColor", numberColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "stringColor", stringColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "operatorColor", operatorColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierStyle", identifierStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordStyle", keywordStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "functionsStyle", functionsStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "traceStyle", traceStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "commentStyle", commentStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "numberStyle", numberStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "stringStyle", stringStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "operatorStyle", operatorStyle );
}

bool RDOEditorBaseEditTheme::styleDefault( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT;
}

bool RDOEditorBaseEditTheme::styleUsing( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT || styleType == SCE_RDO_IDENTIFIER ||
	       styleType == SCE_RDO_KEYWORD || styleType == SCE_RDO_FUNCTION ||
	       styleType == SCE_RDO_TRACE   || styleType == SCE_RDO_COMMENT ||
	       styleType == SCE_RDO_NUMBER  || styleType == SCE_RDO_STRING ||
	       styleType == SCE_RDO_OPERATOR;
}

bool RDOEditorBaseEditTheme::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT   : return defaultStyle    & RDOFS_BOLD ? true : false;
		case SCE_RDO_IDENTIFIER: return identifierStyle & RDOFS_BOLD ? true : false;
		case SCE_RDO_KEYWORD   : return keywordStyle    & RDOFS_BOLD ? true : false;
		case SCE_RDO_FUNCTION  : return functionsStyle  & RDOFS_BOLD ? true : false;
		case SCE_RDO_TRACE     : return traceStyle      & RDOFS_BOLD ? true : false;
		case SCE_RDO_COMMENT   : return commentStyle    & RDOFS_BOLD ? true : false;
		case SCE_RDO_NUMBER    : return numberStyle     & RDOFS_BOLD ? true : false;
		case SCE_RDO_STRING    : return stringStyle     & RDOFS_BOLD ? true : false;
		case SCE_RDO_OPERATOR  : return operatorStyle   & RDOFS_BOLD ? true : false;
	}
	return false;
}

bool RDOEditorBaseEditTheme::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT   : return defaultStyle    & RDOFS_ITALIC ? true : false;
		case SCE_RDO_IDENTIFIER: return identifierStyle & RDOFS_ITALIC ? true : false;
		case SCE_RDO_KEYWORD   : return keywordStyle    & RDOFS_ITALIC ? true : false;
		case SCE_RDO_FUNCTION  : return functionsStyle  & RDOFS_ITALIC ? true : false;
		case SCE_RDO_TRACE     : return traceStyle      & RDOFS_ITALIC ? true : false;
		case SCE_RDO_COMMENT   : return commentStyle    & RDOFS_ITALIC ? true : false;
		case SCE_RDO_NUMBER    : return numberStyle     & RDOFS_ITALIC ? true : false;
		case SCE_RDO_STRING    : return stringStyle     & RDOFS_ITALIC ? true : false;
		case SCE_RDO_OPERATOR  : return operatorStyle   & RDOFS_ITALIC ? true : false;
	}
	return false;
}

string RDOEditorBaseEditTheme::styleFGColorToHEX( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT   : return colorToHEX( defaultColor );
		case SCE_RDO_IDENTIFIER: return colorToHEX( identifierColor );
		case SCE_RDO_KEYWORD   : return colorToHEX( keywordColor );
		case SCE_RDO_FUNCTION  : return colorToHEX( functionsColor );
		case SCE_RDO_TRACE     : return colorToHEX( traceColor );
		case SCE_RDO_COMMENT   : return colorToHEX( commentColor );
		case SCE_RDO_NUMBER    : return colorToHEX( numberColor );
		case SCE_RDO_STRING    : return colorToHEX( stringColor );
		case SCE_RDO_OPERATOR  : return colorToHEX( operatorColor );
	}
	return RDOBaseEditTheme::styleFGColorToHEX( styleType );
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getDefaultTheme()
{
	RDOEditorBaseEditTheme theme;
	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getCppTheme()
{
	RDOEditorBaseEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0xFF );
	theme.functionsColor  = RGB( 0x00, 0x00, 0xFF );
	theme.traceColor      = RGB( 0x00, 0x00, 0xFF );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getPascalTheme()
{
	RDOEditorBaseEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x00, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = RDOFS_BOLD;
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getHtmlTheme()
{
	RDOEditorBaseEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x80, 0x00, 0x80 );
	theme.functionsColor  = RGB( 0xFF, 0x00, 0x00 );
	theme.traceColor      = RGB( 0xFF, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getClassicTheme()
{
	RDOEditorBaseEditTheme theme;

	theme.defaultColor    = RGB( 0xFF, 0xFF, 0x00 );
	theme.identifierColor = RGB( 0xFF, 0xFF, 0x00 );
	theme.keywordColor    = RGB( 0xFF, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.traceColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0x00 );
	theme.backgroundColor = RGB( 0x00, 0x00, 0x80 );

	theme.caretColor       = RGB( 0xFF, 0xFF, 0x00 );
	theme.selectionBgColor = RGB( 0x00, 0x00, 0x40 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getTwilightTheme()
{
	RDOEditorBaseEditTheme theme;

	theme.defaultColor    = RGB( 0xFF, 0xFF, 0xFF );
	theme.identifierColor = RGB( 0xFF, 0xFF, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0x00 );
	theme.traceColor      = RGB( 0x00, 0xFF, 0x00 );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xC0, 0xC0, 0xC0 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0xFF );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0xFF );
	theme.backgroundColor = RGB( 0x00, 0x00, 0x00 );

	theme.caretColor       = RGB( 0xFF, 0xFF, 0xFF );
	theme.selectionBgColor = RGB( 0x70, 0x70, 0x70 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = static_cast<RDOFontStyle>(RDOFS_BOLD | RDOFS_ITALIC);
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getOceanTheme()
{
	RDOEditorBaseEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0xFF );
	theme.identifierColor = RGB( 0x00, 0x00, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0xFF );
	theme.backgroundColor = RGB( 0x00, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xD0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = RDOFS_BOLD;
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorBaseEditStyle
// ----------------------------------------------------------------------------
RDOEditorBaseEditStyle::RDOEditorBaseEditStyle(): RDOBaseEditStyle()
{
}

RDOEditorBaseEditStyle::~RDOEditorBaseEditStyle()
{
}

void RDOEditorBaseEditStyle::initTheme()
{
	theme = new RDOEditorBaseEditTheme;
}

RDOEditorBaseEditStyle& RDOEditorBaseEditStyle::operator =( const RDOEditorBaseEditStyle& style )
{
	RDOBaseEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOEditorBaseEditTheme*>(theme) = *static_cast<RDOEditorBaseEditTheme*>(style.theme);

	return *this;
}

bool RDOEditorBaseEditStyle::operator ==( const RDOEditorBaseEditStyle& style ) const
{
	bool flag = RDOBaseEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOEditorBaseEditTheme*>(theme) == *static_cast<RDOEditorBaseEditTheme*>(style.theme);
	return flag;
}

bool RDOEditorBaseEditStyle::operator !=( const RDOEditorBaseEditStyle& style ) const
{
	return !(*this == style);
}
