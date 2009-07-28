// activex.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{$guid2}", 
		 name = "activex", 
		 helpstring = "activex 1.0 Type Library",
		 resource_name = "IDR_ACTIVEX") ]
class CactivexModule
{
public:
// Override CAtlDllModuleT members
};
		 
