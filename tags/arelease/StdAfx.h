// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__438C05E7_56F3_11D6_9CB7_A8B1CE397073__INCLUDED_)
#define AFX_STDAFX_H__438C05E7_56F3_11D6_9CB7_A8B1CE397073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning( disable : 4786 )

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>
#include <afxmt.h>

std::string format( const char* str, ... );
std::string format( UINT resource, ... );
void trim( std::string& str );
void trimLeft( std::string& str );
void trimRight( std::string& str );
void appendMenu( CMenu* from, const int from_index, CMenu* to );
void eraseMenu( CMenu* from, const int from_index = -1 );
int roundDouble( const double val );

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__438C05E7_56F3_11D6_9CB7_A8B1CE397073__INCLUDED_)
