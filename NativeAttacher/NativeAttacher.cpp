// NativeAttacher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "time.h"


#include "Header.h"


//returns Process Id
ulonglong CreateDummyProcess()
{
	PROCESS_INFORMATION PI;
	STARTUPINFO SI = {sizeof(SI)};

	wchar_t Path_All[MAX_PATH+1] = {0};

	GetSystemDirectory(Path_All,MAX_PATH);
	wcscat(Path_All,L"\\cmd.exe");

	if(!CreateProcess(Path_All,0,0,0,FALSE,CREATE_NEW_CONSOLE /*CREATE_SUSPENDED*/,0,0,&SI,&PI))
	{
		printf("Error creating process\r\n");
		return 0;
	}
	ulonglong Pid = PI.dwProcessId;

	printf("PID: %I64X\r\n",Pid);
	return Pid;
}

//returns Process Id
ulonglong CreateDummyProcess(wchar_t* pExeName)
{
	PROCESS_INFORMATION PI;
	STARTUPINFO SI = {sizeof(SI)};

	wchar_t Path_All[MAX_PATH+1] = {0};
	wcscat(Path_All,pExeName);

	if(!CreateProcess(Path_All,0,0,0,FALSE,CREATE_NEW_CONSOLE /*CREATE_SUSPENDED*/,0,0,&SI,&PI))
	{
		printf("Error creating process\r\n");
		return 0;
	}
	ulonglong Pid = PI.dwProcessId;

	printf("PID: %I64X\r\n",Pid);
	return Pid;
}


void PrintExceptionRecord(_EXCEPTION_RECORD* pRec)
{
		ulonglong ExceptionCode = pRec->ExceptionCode;
		printf("ExceptionCode: %I64X\r\n",ExceptionCode);


		ulonglong ExceptionFlags = pRec->ExceptionFlags;
		printf("ExceptionFlags: %I64X\r\n",ExceptionFlags);

		_EXCEPTION_RECORD* pNextExceptionRecord = pRec->ExceptionRecord;
		printf("pNextExceptionRecord: %I64X\r\n",pNextExceptionRecord);
								
		void* ExceptionAddress = pRec->ExceptionAddress;
		printf("ExceptionAddress: %I64X\r\n",ExceptionAddress);

		ulong NumberParameters = pRec->NumberParameters;
		printf("NumberParameters: %X\r\n",NumberParameters);

		ulong Pad_After_NumParams = *(ulong*)(((uchar*)(&pRec->NumberParameters)) + 0x4);
		printf("Pad: %X\r\n",Pad_After_NumParams);


		ulong Count = 15;
		if(NumberParameters > Count)
		{
			Count = NumberParameters;
		}

		for(ulong ioi=0;ioi<Count;ioi++)
		{
			printf("ExceptionInformation (%X): %I64X\r\n",ioi,pRec->ExceptionInformation[ioi]);
		}
		return;
}

void PrintExceptionDebugEvent(_DBGKM_EXCEPTION* pDbgkmException)
{
		PrintExceptionRecord(&pDbgkmException->ExceptionRecord);

		ulonglong dwFirstChance = pDbgkmException->dwFirstChance;
		printf("dwFirstChance: %X\r\n",dwFirstChance);

		ulonglong PadF = pDbgkmException->Pad;
		printf("Pad: %X\r\n",PadF);
}

void PrintThreadCreationDebugEvent(_DBGKM_CREATE_THREAD* pDbgkmThread)
{
		ulonglong lpThreadLocalBase = pDbgkmThread->lpThreadLocalBase;
		printf("lpThreadLocalBase: %I64X\r\n",lpThreadLocalBase);

		ulonglong Win32StartAddress = pDbgkmThread->Win32StartAddress;
		printf("Win32StartAddress: %I64X\r\n",Win32StartAddress);
}

void PrintProcessCreationDebugEvent(_DBGKM_CREATE_PROCESS* pDbgkmProcess)
{
		ulonglong Unk0 = pDbgkmProcess->Unk0;
		printf("Unk0: %I64X\r\n",Unk0);

		HANDLE hFile = pDbgkmProcess->hFile;
		printf("hFile: %I64X\r\n",hFile);

		ulonglong BaseAddress = pDbgkmProcess->SectionBaseAddress;
		printf("BaseAddress: %I64X\r\n",BaseAddress);

		ulonglong dwDebugInfoFileOffset = pDbgkmProcess->dwDebugInfoFileOffset;
		printf("dwDebugInfoFileOffset: %I64X\r\n",dwDebugInfoFileOffset);

		ulonglong nDebugInfoSize = pDbgkmProcess->nDebugInfoSize;
		printf("nDebugInfoSize: %I64X\r\n",nDebugInfoSize);

		ulonglong Unk2 = pDbgkmProcess->Unk2;
		printf("Unk2: %I64X\r\n",Unk2);

		ulonglong lpStartAddress = pDbgkmProcess->lpStartAddress;
		printf("lpStartAddress: %I64X\r\n",lpStartAddress);
}

void PrintThreadExitDebugEvent(_DBGKM_EXIT_THREAD* pDbgkmExitThread)
{
		ulonglong dwExitCodeThread = pDbgkmExitThread->dwExitCode;
		printf("Thread ExitCode: %X\r\n",dwExitCodeThread);
}

void PrintProcessExitDebugEvent(_DBGKM_EXIT_PROCESS* pDbgkmExitProcess)
{
		ulonglong dwExitCodeThread = pDbgkmExitProcess->dwExitCode;
		printf("Process ExitCode: %X\r\n",dwExitCodeThread);
}

void PrintLoadDllDebugEvent(_DBGKM_LOAD_DLL* pDbgKmLoadDll)
{
		HANDLE hFile = pDbgKmLoadDll->hFile;
		printf("hFile: %I64X\r\n",hFile);

		ulonglong BaseOfDll = pDbgKmLoadDll->BaseOfDll;
		printf("BaseOfDll: %I64X\r\n",BaseOfDll);

		ulonglong DebugInfoFileOffset = pDbgKmLoadDll->DebugInfoFileOffset;
		printf("DebugInfoFileOffset: %I64X\r\n",DebugInfoFileOffset);

		ulonglong DebugInfoSize = pDbgKmLoadDll->DebugInfoSize;
		printf("DebugInfoSize: %I64X\r\n",DebugInfoSize);


		void* pTempDllName = (void*)(pDbgKmLoadDll->pTempDllName);
		printf("pTempDllName: %I64X\r\n",pTempDllName);
}

void PrintUnloadDllDebugEvent(_DBGKM_UNLOAD_DLL* pDbgkmUnloadDll)
{
		ulonglong lpBaseOfDll = pDbgkmUnloadDll->lpBaseOfDll;
		printf("lpBaseOfDll: %I64X\r\n",lpBaseOfDll);
}


void PrintDebugEvent(ulonglong UserEventId,_USER_STATE_CHANGE* pUserStateChange)
{
		ulong Pad = pUserStateChange->Pad;
		ulonglong ProcessId = pUserStateChange->ProcessId;
		ulonglong ThreadId = pUserStateChange->ThreadId;

		if(UserEventId == 7)
		{
			printf("STATUS_BREAKPOINT (KernelEventId: 0)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);


			_DBGKM_EXCEPTION* pDbgkmException = (_DBGKM_EXCEPTION*)
				(&pUserStateChange->DebugBuffer[0]);

			PrintExceptionDebugEvent(pDbgkmException);

		}
		else if(UserEventId == 8)
		{
			printf("STATUS_SINGLE_STEP (KernelEventId: 0)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);

			_DBGKM_EXCEPTION* pDbgkmException = (_DBGKM_EXCEPTION*)
				(&pUserStateChange->DebugBuffer[0]);

			PrintExceptionDebugEvent(pDbgkmException);
		}
		else if(UserEventId == 6)
		{
			printf("Other exceptions (KernelEventId: 0)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);

			_DBGKM_EXCEPTION* pDbgkmException = (_DBGKM_EXCEPTION*)
				(&pUserStateChange->DebugBuffer[0]);

			PrintExceptionDebugEvent(pDbgkmException);
		}
		else if(UserEventId == 2)
		{
			printf("CREATE_THREAD (KernelEventId: 1)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);

			HANDLE hThread = *(HANDLE*)(&pUserStateChange->DebugBuffer[0]);
			printf("hThread: %I64X\r\n",hThread);


			_DBGKM_CREATE_THREAD* pDbgkmThread = 
				(_DBGKM_CREATE_THREAD*)(&pUserStateChange->DebugBuffer[8]);

			PrintThreadCreationDebugEvent(pDbgkmThread);
		}
		else if(UserEventId == 3)
		{
			printf("CREATE_PROCESS (KernelEventId: 2)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);

			HANDLE hProcess = *(HANDLE*)(&pUserStateChange->DebugBuffer[0]);
			printf("hProcess: %I64X\r\n",hProcess);

			HANDLE hThread = *(HANDLE*)(&pUserStateChange->DebugBuffer[8]);
			printf("hThread: %I64X\r\n",hThread);

			_DBGKM_CREATE_PROCESS* pDbgkmProcess = 
				(_DBGKM_CREATE_PROCESS*)(&pUserStateChange->DebugBuffer[0x10]);

			PrintProcessCreationDebugEvent(pDbgkmProcess);

		}
		else if(UserEventId == 4)
		{
			printf("EXIT_THREAD (KernelEventId: 3)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);

			_DBGKM_EXIT_THREAD* pDbgkmExitThread = 
				(_DBGKM_EXIT_THREAD*)(&pUserStateChange->DebugBuffer[0]);

			PrintThreadExitDebugEvent(pDbgkmExitThread);

		}
		else if(UserEventId == 5)
		{
			printf("EXIT_PROCESS (KernelEventId: 4)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);

			_DBGKM_EXIT_PROCESS* pDbgkmExitProcess= 
				(_DBGKM_EXIT_PROCESS*)(&pUserStateChange->DebugBuffer[0]);

			PrintProcessExitDebugEvent(pDbgkmExitProcess);

		}
		else if(UserEventId == 9)
		{
			printf("LOAD_DLL (KernelEventId: 5)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);

			_DBGKM_LOAD_DLL* pDbgKmLoadDll = 
				(_DBGKM_LOAD_DLL*)(&pUserStateChange->DebugBuffer[0]);

			PrintLoadDllDebugEvent(pDbgKmLoadDll);

		}
		else if(UserEventId == 0xA)
		{
			printf("UNLOAD_DLL (KernelEventId: 6)\r\n");
			printf("Pad: %X\r\n",Pad);
			printf("Pid: %I64X\r\n",ProcessId);
			printf("Tid: %I64X\r\n",ThreadId);


			_DBGKM_UNLOAD_DLL* pDbgkmUnloadDll = 
				(_DBGKM_UNLOAD_DLL*)(&pUserStateChange->DebugBuffer[0]);

			PrintUnloadDllDebugEvent(pDbgkmUnloadDll);
		}
		else
		{
			printf("Fatal Error: Unknown UserEvendId\r\n");
			MessageBox(0,L"www",L"aaa",0);
			ExitProcess(0);
		}

		return;
}


void NativeDebugger(ulonglong Pid)
{
	if(!Pid)	return;

	_CLIENT_ID Cid = {0};
	Cid.UniqueProcess = Pid;

	_OBJECT_ATTRIBUTES ObjAttr = {0};
	ObjAttr.Length = sizeof(ObjAttr);

	HANDLE hProcess = 0;
	int ret = ZwOpenProcess(&hProcess,PROCESS_ALL_ACCESS,&ObjAttr,&Cid);
	printf("ZwOpenProcess, ret: %x\r\n",ret);
	if(ret >= 0)
	{
		printf("hProcess: %I64X\r\n",hProcess);

		HANDLE hDebug = 0;

		bool bKillOnClose = true;
		ret = ZwCreateDebugObject(&hDebug,GENERIC_ALL,0,bKillOnClose?1:0);
		printf("ZwCreateDebugObject, ret: %x\r\n",ret);
		if(ret >= 0)
		{
			printf("hDebug: %I64X\r\n",hDebug);

			ulong bKillOnClose = 1;
			ret = ZwSetInformationDebugObject(hDebug,1,&bKillOnClose,0x4,0);
			printf("ZwSetInformationDebugObject, ret: %x\r\n",ret);

			if(ret >= 0)
			{

				ret = ZwDebugActiveProcess(hProcess,hDebug);
				printf("ZwDebugActiveProcess, ret: %x\r\n",ret);

				if(ret >= 0)
				{
					

					_LARGE_INTEGER Timeout = {0};
					Timeout.QuadPart = -1LL;//1 nanosecond

					ulonglong Alertable = 1;

					


					while(1)
					{
						_USER_STATE_CHANGE S = {0};
						memset(&S,0xCC,sizeof(S));

						ret = ZwWaitForDebugEvent(hDebug,Alertable,&Timeout,&S);
						printf("ZwWaitForDebugEvent, ret: %x\r\n",ret);

						if(ret >= 0)
						{

							DumpHex_8(&S,sizeof(S));
							printf("\r\n");

							if( (ret == STATUS_TIMEOUT) || (ret == STATUS_USER_APC) )
							{
								continue;
							}

							ulonglong UserEventId = S.UserEventId;
							PrintDebugEvent(UserEventId,&S);
							//--------------------------------------------
							_CLIENT_ID CidXX = {0};
							CidXX.UniqueProcess = S.ProcessId;
							CidXX.UniqueThread = S.ThreadId;


							ulonglong var_Status = 0;
							//Breakpoint Or SingleStep
							if((UserEventId == 7)||(UserEventId == 8))
							{
								var_Status = DBG_CONTINUE;
							}
							else if(UserEventId == 6)
							{
								var_Status = DBG_EXCEPTION_NOT_HANDLED;
							}
							else
							{
								var_Status = DBG_CONTINUE;
							}

							ret = ZwDebugContinue(hDebug,&CidXX,var_Status);
							printf("ZwDebugContinue, ret: %x (i: %I64X)\r\n",ret,var_Status);

							if(ret >= 0)
							{
								printf("Press any key to continue");
								getchar();
							}
						}
					}



				}

			}

			ZwClose(hDebug);
		}
		ZwClose(hProcess);
	}
	return;
}


int _tmain(int argc, _TCHAR* argv[])
{
	ulonglong Pid = 0;

	if(argc == 1)
	{
		Pid = CreateDummyProcess();
	}
	else if(argc > 1)
	{
		Pid = CreateDummyProcess(argv[1]);
	}


	ulonglong PidX = 0;



	NativeDebugger(Pid);


	Sleep(-1);

	return 0;
}

