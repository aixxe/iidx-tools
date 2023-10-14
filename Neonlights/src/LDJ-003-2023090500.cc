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

    auto constexpr timer_offset = 0x7216730; // C7 05 [?? ?? ?? ?? ?? ?? ?? ??] 33 C0 3D 00 02 00 00
    auto constexpr patch_offset = 0xB3F819;  // 89 05 ?? ?? ?? ?? E8 ?? ?? ?? ?? EB 28
    auto constexpr hook_offset = 0xB3F7C0;   // 48 83 EC 38 83 3D ?? ?? ?? ?? ?? 75 02

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
    auto constexpr rate_demo_offset = 0xA4F93C + 1; // BA [78] 00 00 00 8B 4C 24 28
    auto constexpr rate_play_offset = 0xA75586 + 1; // BA [78] 00 00 00 8B 8C 24 10 01 00 00

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
    auto constexpr patch_offset = 0xA4F5F0; // 48 89 4C 24 08 48 83 EC 58 E8 ?? ?? ?? ?? 48 8B C8 E8 ?? ?? ?? ?? 48 8B 44 24 60 48 83 38 00

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
    auto constexpr patch_offset = 0xB429D0; // 48 89 4C 24 08 48 83 EC 28 48 8B 44 24 30 83 38 01

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
    auto constexpr patch_outer_offset = 0xA4F8C0; // 48 89 54 24 10 48 89 4C 24 08 48 83 EC 68 48 8B 4C 24 70 E8 ?? ?? ?? ?? 88 44 24 20
    auto constexpr patch_inner_offset = 0x4894E0; // E8 [?? ?? ?? ??] 48 8B 44 24 70 48 8B 00 8B 40 34 89 44 24 28

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