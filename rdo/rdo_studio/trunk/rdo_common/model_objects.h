/*
 * copyright: (c) RDO-Team, 2009
 * filename : model_objects.h
 * author   : �������� ����, ������ ������
 * date     : 13.06.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCOMMON_MODEL_OBJECTS_H_
#define _RDOCOMMON_MODEL_OBJECTS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

#define OPEN_RDO_MODEL_OBJECTS_NAMESPACE	namespace rdoModelObjects {
#define CLOSE_RDO_MODEL_OBJECTS_NAMESPACE	};

OPEN_RDO_MODEL_OBJECTS_NAMESPACE
	struct RDOSMRFileInfo
	{
		tstring  m_modelName;
		tstring  m_resourceFile;
		tstring  m_frameFile;
		tstring  m_statisticFile;
		tstring  m_resultsFile;
		tstring  m_traceFile;
		rbool    m_error;

		RDOSMRFileInfo()
			: m_modelName    (_T(""))
			, m_resourceFile (_T(""))
			, m_frameFile    (_T(""))
			, m_statisticFile(_T(""))
			, m_resultsFile  (_T(""))
			, m_traceFile    (_T(""))
			, m_error        (false )
		{}
	};

	enum RDOFileType  {RTP = 0, RSS, EVN, PAT, DPT, PRC, FRM, FUN, SMR, PMD, PMV, TRC};

	enum RDOParseType
	{
		obNONE = 0x0000,
		obPRE  = 0x0001,
		obRTP  = 0x0002,
		obRSS  = 0x0004,
		obFUN  = 0x0008,
		obEVN  = 0x0010,
		obPAT  = 0x0020,
		obDPT  = 0x0040,
		obPRC  = 0x0080,
		obPMD  = 0x0100,
		obFRM  = 0x0200,
		obSMR  = 0x0400,
		obPOST = 0x0800,
		obALL  = 0x0FFF
	};
CLOSE_RDO_MODEL_OBJECTS_NAMESPACE

#endif //! _RDOCOMMON_MODEL_OBJECTS_H_
