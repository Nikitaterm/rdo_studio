/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_build_edit_line_info.cpp
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief
  \indent    4T
 */

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/report/src/build_edit_line_info.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

namespace
{
	const std::string WARNING_STRING("предупреждение");
	const std::string ERROR_STRING("ошибка");
}

BuildEditLineInfo::BuildEditLineInfo(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type)
	: LogEditLineInfo(text, file, line, pos, type)
{}

BuildEditLineInfo::BuildEditLineInfo(CREF(FileMessage) message)
	: LogEditLineInfo(message)
{}

BuildEditLineInfo::BuildEditLineInfo(CREF(tstring) message)
	: LogEditLineInfo(message)
{}

BuildEditLineInfo::~BuildEditLineInfo()
{}

tstring BuildEditLineInfo::getMessage() const
{
	if (isSimpleTextMessage())
	{
		return getText();
	}
	else
	{
		const tstring file = rdoModelObjects::getFileTypeString(getFileType());
		const tstring error = (getType() == FileMessage::MT_WARNING) ? WARNING_STRING : ERROR_STRING;
		const tstring text = rdo::format("%s (%d): %s: %s", file.c_str(), getLineNumber() + 1, error.c_str(), getText().c_str());
		return text;
	}
}

}}} // namespace rdo::simulation::report
