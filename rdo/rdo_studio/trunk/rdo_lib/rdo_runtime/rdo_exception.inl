/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      rdo_exception.inl
 * @author    ����� �������
 * @date      06.08.2011
 * @brief     ����������� ����������
 * @indent    4T
 *********************************************************************************/

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOException
// --------------------------------------------------------------------------------
inline RDOException::RDOException(CREF(tstring) message)
	: m_message(message)
{}

inline CREF(tstring) RDOException::message() const
{
	return m_message;
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeException
// --------------------------------------------------------------------------------
inline RDORuntimeException::RDORuntimeException(CREF(tstring) message)
	: RDOException(message)
{}

inline tstring RDORuntimeException::getType() const
{
	return "RDO Runtime Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOTypeException
// --------------------------------------------------------------------------------
inline RDOTypeException::RDOTypeException(CREF(tstring) message)
	: RDORuntimeException(message)
{}

inline tstring RDOTypeException::getType() const
{
	return "RDOType Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOValueException
// --------------------------------------------------------------------------------
inline RDOValueException::RDOValueException(CREF(tstring) message)
	: RDORuntimeException(message)
{}

inline tstring RDOValueException::getType() const
{
	return "RDOValue Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOInternalException
// --------------------------------------------------------------------------------
inline RDOInternalException::RDOInternalException(CREF(tstring) message)
	: RDORuntimeException(message)
{}

inline tstring RDOInternalException::getType() const
{
	return "RDO Internal Error";
}

CLOSE_RDO_RUNTIME_NAMESPACE
