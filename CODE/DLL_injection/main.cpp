#include <windows.h>

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    BOOL ret = TRUE;
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            MessageBoxW(NULL, L"ACAB", L"KAWAIII", MB_ICONQUESTION | MB_OK);
            break;
        }

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        default:
        {
            break;
        }
    }
    return ret;
}