/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_log_edit_line_info.h
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief
  \indent    4T
 */

#ifndef _LOG_EDIT_LINE_INFO_H_
#define _LOG_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

class LogEditLineInfo : public FileMessage
{
public:
	LogEditLineInfo(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type);
	explicit LogEditLineInfo(CREF(FileMessage) message);
	explicit LogEditLineInfo(CREF(tstring) message);
	virtual ~LogEditLineInfo();

	virtual tstring getMessage() const;
	rbool isSimpleTextMessage() const;

	ruint getPosInLog() const;
	void setPosInLog(int posInLog);

private:
	ruint m_posInLog;
	rbool m_simpleTextMessage;
};

}}} // namespace rdo::simulation::report

#endif // _LOG_EDIT_LINE_INFO_H_
