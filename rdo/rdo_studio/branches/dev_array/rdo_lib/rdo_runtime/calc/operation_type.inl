/*!
  \copyright (c) RDO-Team, 2011
  \file      operation_type.inl
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      26.07.2011
  \brief     ��� ��������: �������������� ��� ���������� ���������
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- OperatorType
// --------------------------------------------------------------------------------
template <OperatorType::Type>
inline REF(ruint) OperatorType::getCalcCounter()
{
	static ruint s_�ounter = 0;
	return s_�ounter;
}

CLOSE_RDO_RUNTIME_NAMESPACE
