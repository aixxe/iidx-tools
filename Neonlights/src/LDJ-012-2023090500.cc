#include <vector>
#include <MinHook.h>

#define TARGET_MODULE "bm2dx.dll"

std::uint8_t* bm2dx = nullptr;

inline void patch(std::uint8_t* dst, const std::vector<std::uint8_t>& bytes)
{
    auto orig = DWORD {};
    VirtualProtect(dst, bytes.size(), PAGE_EXECUTE_READWRITE, &orig);
    std::copy(bytes.begin(), bytes.end(), dst);
    VirtualProtect(dst, bytes.size(), orig, &orig);
}

namespace ticker
{
    auto increment = true;

    auto constexpr timer_offset = 0x68EEA00; // 01 05 [?? ?? ?? ??] 48 83 C4 28
    auto constexpr patch_offset = 0x5B6220;  // 8B 05 ?? ?? ?? ?? 01 05 ?? ?? ?? ?? 48 83 C4 28
    auto constexpr hook_offset = 0x5B61C0;   // 48 83 EC 28 83 3D ?? ?? ?? ?? ?? 74 68

    void (*original) () = nullptr;
    void install()
    {
        MH_CreateHook(bm2dx + hook_offset, (void*) +[] ()
        {
            original();
            *(std::uint32_t*) (bm2dx + timer_offset) -= increment;
            increment = !increment;
        }, (void**) &original);
        patch(bm2dx + patch_offset, { 0xFF });
    }
}

namespace neon_lamp
{
    auto constexpr rate_demo_offset = 0x54A084 + 1; // BA [78] 00 00 00 8B 48 34 E8 ?? ?? ?? ?? 33 C9
    auto constexpr rate_play_offset = 0x55AD28 + 1; // BA [78] 00 00 00 8B 48 34 E8 ?? ?? ?? ?? 0F B6 97 38 01 00 00

    void install()
    {
        patch(bm2dx + rate_demo_offset, { 0xF0 });
        patch(bm2dx + rate_play_offset, { 0xF0 });
    }
}

namespace sw_lamp
{
    auto increment = true;

    auto constexpr timer_offset = 0x3C;
    auto constexpr patch_offset = 0x549E60; // 40 53 48 83 EC 30 48 8B D9 E8 ?? ?? ?? ?? 48 8B C8 E8 ?? ?? ?? ?? 48 8B 0B

    void* (*original) (void*) = nullptr;
    void install()
    {
        MH_CreateHook(bm2dx + patch_offset, (void*) +[] (std::uint8_t* a1)
        {
            auto result = original(a1);
            *(std::uint32_t*) (a1 + timer_offset) -= increment;
            increment = !increment;
            return result;
        }, reinterpret_cast<void**>(&original));
    }
}

namespace effector
{
    auto increment = true;

    auto constexpr timer_offset = 0x18;
    auto constexpr patch_offset = 0x5B77B0; // 40 53 48 83 EC 20 83 39 01 48 8B D9 B9 00 00 04 00

    void* (*original) (void*) = nullptr;
    void install()
    {
        MH_CreateHook(bm2dx + patch_offset, (void*) +[] (std::uint8_t* a1)
        {
            auto result = original(a1);
            *(std::uint32_t*) (a1 + timer_offset) -= increment;
            increment = !increment;
            return result;
        }, reinterpret_cast<void**>(&original));
    }
 }

namespace spot_light
{
    auto constexpr patch_outer_offset = 0x54A040; // 48 89 5C 24 08 57 48 83 EC 20 48 8B D9 E8 ?? ?? ?? ?? 80 7B 45 00
    auto constexpr patch_inner_offset = 0x343BF0; // E8 [?? ?? ?? ??] C7 45 30 67 67 67 67

    bool enabled = false;

    void (*outer_original) (void*) = nullptr;
    void (*inner_original) (std::int32_t) = nullptr;
    void install()
    {
        MH_CreateHook(bm2dx + patch_outer_offset, (void*) +[] (void* a1)
        {
            enabled = true;
            outer_original(a1);
            enabled = false;
        }, reinterpret_cast<void**>(&outer_original));
        MH_CreateHook(bm2dx + patch_inner_offset, (void*) +[] (std::int32_t timer)
        {
            if (enabled)
                timer >>= 1;
            inner_original(timer);
        }, reinterpret_cast<void**>(&inner_original));
    }
}

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
    if (reason != DLL_PROCESS_ATTACH)
        return TRUE;

    bm2dx = reinterpret_cast<std::uint8_t*>(GetModuleHandleA(TARGET_MODULE));

    MH_Initialize();

    ticker::install();
    sw_lamp::install();
    neon_lamp::install();
    spot_light::install();
    effector::install();

    MH_EnableHook(MH_ALL_HOOKS);

    return TRUE;
}