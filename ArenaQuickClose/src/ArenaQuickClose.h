#pragma once

#include <cstdint>
#include <MinHook/MinHook.h>

// Base address of the "bm2dx.dll" module.
uintptr_t bm2dx_addr = 0;

// Changes when VEFX and/or EFFECT buttons are pressed.
// Second bit: EFFECT, third bit: VEFX.
constexpr ptrdiff_t BTN_STATE = 0x6853F2;

// Contains the time remaining before the lobby is closed.
// Starts at 3900 and is decremented once per frame.
constexpr ptrdiff_t ARENA_TIMELEFT = 0x313754C;

// Hook target function that decrements the time remaining value.
// Only runs on the arena matching screen.
constexpr ptrdiff_t ARENA_MATCHING_FN = 0x383570;

int64_t (__fastcall* original_arena_matching_fn) (void*, void*) = nullptr;