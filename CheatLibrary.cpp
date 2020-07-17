#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <inttypes.h>
#include <stdint.h>
#include "cheat_funcs.h";

DWORD get_pid(const char *module_name) {
	HANDLE running_processes;
	PROCESSENTRY32 p32;
	int pid = 0;

	running_processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (running_processes == INVALID_HANDLE_VALUE) {
		printf("Could not get a handle to available processes, trying running as Administrator \n");
		return 0;
	};

	p32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(running_processes, &p32)){
		CloseHandle(running_processes);
		printf("Could not get the first process :(\n");
		return 0;
	
	};

	while (Process32Next(running_processes, &p32)) {
		if (lstrcmpiA(module_name,p32.szExeFile) == 0) {
			pid = p32.th32ProcessID;
			break;
		
		};
	
	};

	CloseHandle(running_processes);

	return pid;
};

uintptr_t proc_base(DWORD pid, const char *mod_name) {
	uintptr_t base_addr = NULL;
	MODULEENTRY32 m32;
	m32.dwSize = sizeof(MODULEENTRY32);

	HANDLE mod_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (mod_handle == INVALID_HANDLE_VALUE) {
		printf("Could not get handle to modules list!");
		DWORD error = GetLastError();
		printf("%d\n",error);
		return 0;
	};
		do {
			
			if (!_stricmp(m32.szModule,mod_name)) {
				base_addr = (uintptr_t)m32.modBaseAddr;
				break;
			};

		} while (Module32Next(mod_handle, &m32));
	

	CloseHandle(mod_handle);
	return base_addr;

};



