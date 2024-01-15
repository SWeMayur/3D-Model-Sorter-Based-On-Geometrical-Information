// ModelFactory.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "ModelFactory.h"


// This is an example of an exported variable
MODELFACTORY_API int nModelFactory=0;

// This is an example of an exported function.
MODELFACTORY_API int fnModelFactory(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CModelFactory::CModelFactory()
{
    return;
}
