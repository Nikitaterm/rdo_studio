/*!
  \copyright (c) RDO-Team, 2012
  \file      error_code.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.02.2012
  \brief     ���� ��������� �������������
  \indent    4T
*/

#ifndef _SIMULATOR_REPORT_ERROR_CODE_H_
#define _SIMULATOR_REPORT_ERROR_CODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/model_objects.h"
#include "simulator/report/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

//! ��� ��������� ���������� ��� ������� ������
enum RDOExitCode
{
	EC_OK = 0,        //!< ������ ���������������� ��� ����������� ��� ������
	EC_ParserError,   //!< ���������� ����������� � �������
	EC_RunTimeError,  //!< ������ ������ ���������� � �������
	EC_UserBreak,     //!< ������ ����������� �������������
	EC_ModelNotFound, //!< ���� ������� ������ �� ������
	EC_NoMoreEvents   //!< ������ ����������� ������, ��� ������ ������ ������������
};

//! �������������� ���������
struct RDOSyntaxMessage
{
	enum ErrorCode
	{
		UNKNOWN = 1
	};

	enum Type
	{
		MT_ERROR = 0,
		MT_WARNING
	};

	tstring                      text; //!< ����� ���������
	ErrorCode                    code; //!< ��� ������
	rdoModelObjects::RDOFileType file; //!< ���� � �������
	ruint                        line; //!< ����� ������ � �������
	ruint                        pos;  //!< ������� ������ � ������
	Type                         type; //!<  ��� ���������

	RDOSyntaxMessage(CREF(tstring) text, ErrorCode code, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MT_ERROR) : 
		text(text),
		code(code),
		file(file),
		line(line),
		pos (pos ),
		type(type)
	{
	}
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _SIMULATOR_REPORT_ERROR_CODE_H_