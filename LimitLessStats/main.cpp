#include "nvse/PluginAPI.h"
#include <algorithm>

NVSEInterface* g_nvseInterface{};

void SafeWrite32(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt32*)addr) = data;
	VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
}

void ReplaceCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}
template <typename T_Ret = UInt32, typename ...Args>
__forceinline T_Ret ThisStdCall(UInt32 _addr, const void* _this, Args ...args)
{
	return ((T_Ret(__thiscall*)(const void*, Args...))_addr)(_this, std::forward<Args>(args)...);
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "LimitlessStats";
	info->version = 1;

	return true;
}
void SafeWrite8(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt8*)addr) = data;
	VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
}
void WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// call rel32
	SafeWrite8(jumpSrc, 0xE8);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

int __fastcall LuckRail(void* apThis, void*, uint32_t auiActorValue) {
	return (std::min)(10, ThisStdCall<int32_t>(0x66EF20, apThis, auiActorValue));
}
bool NVSEPlugin_Load(NVSEInterface* nvse)
{
	if (!nvse->isEditor) {
		for (uint32_t address : {0x66F425, 0x66F449, 0x66F478, 0x66F49A, 0x66F4BE, 0x66F4E2, 0x66F51E})
		{
			SafeWrite32(address + 1, 1);
		}
		for (uint32_t address : {0x66F896, 0x66F8D2, 0x66F8F3, 0x66F914, 0x66F935, 0x66F953, 0x66F974, 0x66F992, 0x66F9B0, 0x66F9D1, 0x66F9EF, 0x66FA10, 0x66FA30 }) {
			uint32_t uiCorrectedAddr = address + 1;
			uint32_t uiFlags = *(uint32_t*)uiCorrectedAddr;
			SafeWrite32(uiCorrectedAddr, uiFlags & ~0x10);
		}

		//Only works with 57.30, above versions of 57.56 JIP support limitless stats.
		const PluginInfo* pInfo = NVSE::cmdTableInterface.GetPluginInfoByName("JIP LN NVSE");
		if (pInfo && pInfo->version == 5730) {

			HMODULE hJIP = GetModuleHandle("jip_nvse.dll");
			if (hJIP) { //s_patchInstallState.bigGunsSkill we set 0x410 to 0x400 disabling the kAVFlag_Max100
				SafeWrite8(reinterpret_cast<size_t>(hJIP) + 0x113B8 + 1, 0);
			}

		}

		WriteRelCall(0x733C48, UInt32(LuckRail)); // BlackJack


	}

	return true;
}

