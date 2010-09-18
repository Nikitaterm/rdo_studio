/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.cpp
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE(Type) RDOType__##Type g_##Type;

DECLARE_ATOM_TYPE(unknow       );
DECLARE_ATOM_TYPE(identificator);
DECLARE_ATOM_TYPE(int          );
DECLARE_ATOM_TYPE(real         );
DECLARE_ATOM_TYPE(bool         );
DECLARE_ATOM_TYPE(string       );
DECLARE_ATOM_TYPE(iterator     );

CLOSE_RDO_RUNTIME_NAMESPACE
