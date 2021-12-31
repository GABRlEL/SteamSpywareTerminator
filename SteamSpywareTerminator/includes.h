#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include "MinHook.h"

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mtd.lib")
#endif

//38C8D964		?Init@CThread@@MAE_NXZ	tier0_s
//38C8D88C		?Terminate@CThread@@QAE_NH@Z	tier0_s
//38C8D944		?GetName@CThread@@QAEPBDXZ	tier0_s
//FillInMachineIDInfo()

HMODULE steamClient = GetModuleHandle("steamclient.dll");
HMODULE tier0 = GetModuleHandle("tier0_s.dll");

typedef void(__thiscall* cThreadInit)(void*);
static cThreadInit oThreadInit;
void __fastcall ThreadInit_hk(void* This, void* _EDX);

typedef const char* (__thiscall* cThreadGetNameFn)(void);
cThreadGetNameFn cThreadGetName = (cThreadGetNameFn)GetProcAddress(tier0, "?GetName@CThread@@QAEPBDXZ");