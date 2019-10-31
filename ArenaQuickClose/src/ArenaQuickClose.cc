#include "ArenaQuickClose.h"

int64_t __fastcall replacement_arena_matching_fn(void* a1, void* a2) {
	static uint32_t* btn = reinterpret_cast<uint32_t*>(bm2dx_addr + BTN_STATE);
	static uint32_t* timeleft = reinterpret_cast<uint32_t*>(bm2dx_addr + ARENA_TIMELEFT);

	const bool vefx = *btn & (1 << 3);
	const bool effect = *btn & (1 << 2);

	if (vefx && effect)
		*timeleft = 0;

	return original_arena_matching_fn(a1, a2);
}

BOOL APIENTRY DllMain(HMODULE dll_instance, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(dll_instance);
		MH_Initialize();

		bm2dx_addr = uintptr_t(GetModuleHandleA("bm2dx.dll"));

		MH_CreateHook(
			reinterpret_cast<void*>(bm2dx_addr + ARENA_MATCHING_FN),
			reinterpret_cast<void*>(replacement_arena_matching_fn),
			reinterpret_cast<void**>(&original_arena_matching_fn)
		);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	if (reason == DLL_PROCESS_DETACH)
		MH_Uninitialize();

	return TRUE;
}