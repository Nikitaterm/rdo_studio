#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
// --------------------------------------------------------------------------------

#ifdef RDO_MT
// --------------------------------------------------------------------------------
// -------------------- ThreadStudio
// --------------------------------------------------------------------------------
class ThreadStudio: public RDOThread
{
friend class Application;
protected:
    ThreadStudio();
    virtual ~ThreadStudio() {}; // Чтобы нельзя было удалить через delete
    virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------------------
// -------------------- ThreadStudioGUI
// --------------------------------------------------------------------------------
class ThreadStudioGUI: public RDOKernelGUI
{
friend class Application;
protected:
    ThreadStudioGUI();
    virtual ~ThreadStudioGUI() {}; // Чтобы нельзя было удалить через delete
};
#endif
