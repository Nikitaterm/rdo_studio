/*!
  \copyright (c) RDO-Team, 2011
  \file      atom.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_ATOM_H_
#define _RDOPARSER_TYPE_ATOM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/type/type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_PARSER
// --------------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Type)      \
class RDOType__##Type                      \
	: public rdo::runtime::RDOType__##Type \
	, public IType                         \
	, public IModelStructure               \
{                                          \
public:                                    \
	DECLARE_IType;                         \
	DECLARE_IModelStructure;               \
};                                         \
DECLARE_POINTER(RDOType__##Type);

DEFINE_ATOM_TYPE_PARSER(unknow       );
DEFINE_ATOM_TYPE_PARSER(identificator);
DEFINE_ATOM_TYPE_PARSER(int          );
DEFINE_ATOM_TYPE_PARSER(real         );
DEFINE_ATOM_TYPE_PARSER(bool         );
DEFINE_ATOM_TYPE_PARSER(string       );
DEFINE_ATOM_TYPE_PARSER(void         );

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_ATOM_H_
