#include <iostream>
#include <windows.h>

using namespace std;

/// @brief Entry point for application that create a process for mspaint executable.
/// @return 
int main()
{
    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi = { 0, };
    
    BOOL ret = CreateProcess("C:\\Windows\\system32\\mspaint.exe", 
                             NULL, 
                             NULL, 
                             NULL, 
                             FALSE,
                             BELOW_NORMAL_PRIORITY_CLASS,
                             NULL,
                             NULL,
                             &si,
                             &pi);
    if(!ret)
    {
        cout << "(-) Something wrong" << endl;
    }
    else
    {
        cout << "(-) PID = " << pi.dwProcessId << '\t' << "TID = " << pi.dwThreadId << endl;
        const DWORD wait = WaitForSingleObject(pi.hProcess, 3000U);
        if(wait != WAIT_OBJECT_0)
        {
            cout << "(-) Wait failed :" << wait << endl;
        }
        if(!CloseHandle(pi.hProcess))
        {
            cout << "(-) Close failed." << endl;
        }
    }
    return 0;
}