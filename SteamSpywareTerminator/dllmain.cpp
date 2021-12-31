// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "includes.h"

extern "C" { __declspec(dllimport) void Init(); }

void __fastcall ThreadInit_hk(void* This, void* _EDX) // ecx actually but im just c+ping old code
{
    char* name = ((char*)(This) + 0x10);
    printf("[!] THREAD started... ID  %i which translates to: %s\n", This, name);
    if (name[15] == 'F' && name[19] == 'M') //CThread::SetName((CThread *)(v1 + 2912), "MachineIDInfoThread"); CSteamEngine::CFillMachineInfoThread
    {
        printf("RED ALERT RED ALERT BURTONJ TRACKING US, HANGING STEAM");
        TerminateThread(*((HANDLE*)This + 1), 0);
    }
}

void Init() {
    if (steamClient && tier0) {
        AllocConsole();
        SetConsoleTitleA("mmmm stean");
        freopen_s((FILE**)stdin, "conin$", "r", stdin);
        freopen_s((FILE**)stdout, "conout$", "w", stdout);
        std::cout << "steanSpywareTerminator build " __DATE__ << " " << __TIME__ << std::endl;

        if (MH_Initialize() != MH_OK)
        {
            std::cout << "[-] Failed to initialize hook management!" << std::endl;
        }

        std::cout << "logging all STEAN threads.........." << std::endl;

        if (MH_CreateHook(GetProcAddress(tier0, "?Init@CThread@@MAE_NXZ"), &ThreadInit_hk, reinterpret_cast<LPVOID*>(oThreadInit)) != MH_OK)
        {
            std::cout << "Failed to hook tier0->CThread::Init..." << std::endl;
        }
        printf("[+] tier0_s.dll->CThread::Init hooked...");

        DWORD OldProtection;

        VirtualProtect((LPVOID)(steamClient + 0x8AACF), sizeof(unsigned char), PAGE_EXECUTE_READWRITE, &OldProtection);
        *(unsigned char*)(steamClient + 0x8AACF) = 'a';
        VirtualProtect((LPVOID)(steamClient + 0x8AACF), sizeof(unsigned char), OldProtection, &OldProtection);

        if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
        {
            std::cout << "Fatal Error!" << std::endl;
        }
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0,0,(LPTHREAD_START_ROUTINE)Init,0,0,0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

