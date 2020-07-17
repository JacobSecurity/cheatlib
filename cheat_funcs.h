#include <windows.h>
#pragma once
DWORD get_pid(const char *module_name);
uintptr_t proc_base(DWORD pid, const char *mod_name);