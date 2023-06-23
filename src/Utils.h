#pragma once
#include "detours/detours.h"

#define HOOK_MAKE_THIS(hookName, retName, thisName, ...)          \
	struct Hook_##hookName                                        \
	{                                                             \
		static retName __fastcall hooked(thisName*, __VA_ARGS__); \
		static inline REL::Relocation<decltype(hooked)> oldFunc;  \
	};
#define HOOK_FUNC_THIS(managerName, hookName, retName, thisName, ...) \
	retName __fastcall managerName::Hook_##hookName::hooked(thisName* a_pThis, __VA_ARGS__)

#define HOOK_MAKE(seId, aeId, seOffset, aeOffset, hookName, retName, ...)            \
	struct Hook_##hookName                                                           \
	{                                                                                \
		static retName __fastcall hooked(__VA_ARGS__);                               \
		static constexpr auto id = REL::RelocationID(seId, aeId);                    \
		static constexpr auto offset = REL::VariantOffset(seOffset, aeOffset, 0x00); \
		static inline REL::Relocation<decltype(hooked)> oldFunc;                     \
	};

#define HOOK_FUNC(managerName, hookName, ...) \
	__fastcall managerName::Hook_##hookName::hooked(__VA_ARGS__)

namespace Utils::Hook
{
	template <class T>
	void DetourAttach(std::uintptr_t a_pAddress)
	{
		T::oldFunc = REL::Relocation<decltype(&T::hooked)>(a_pAddress);
		::DetourAttach(reinterpret_cast<void**>(&T::oldFunc), reinterpret_cast<void*>(T::hooked));
	}

	template <class T>
	void WriteCall(std::uintptr_t a_address)
	{
		SKSE::AllocTrampoline(14);
		auto& trampoline = SKSE::GetTrampoline();
		T::oldFunc = trampoline.write_call<5>(a_address, T::hooked);
	}

	template <class T>
	void WriteCall()
	{
		SKSE::AllocTrampoline(14);
		auto& trampoline = SKSE::GetTrampoline();
		REL::Relocation<std::uint32_t> hook{ T::id, T::offset };
		T::oldFunc = trampoline.write_call<5>(hook.address(), T::hooked);
	}

	void inline DetourStartup()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
	}

	void inline DetourFinish()
	{
		DetourTransactionCommit();
	}
}