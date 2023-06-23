#include "PlayerManager.h"

#include "detours/detours.h"

#include "Meter.h"

HOOK_FUNC_THIS(PlayerManager, AddSkillExperience, void, RE::PlayerCharacter, RE::ActorValue a_enumSkillId, float a_fExperience)
{
	auto pValueOwner = a_pThis->AsActorValueOwner();
	auto pActorValueList = RE::ActorValueList::GetSingleton();
	auto pMeter = Meter::GetSingleton();
	auto pSkillInfo = pActorValueList->GetActorValue(a_enumSkillId);
	auto skillOffset = static_cast<std::underlying_type<RE::ActorValue>::type>(a_enumSkillId);

	//auto pPlayer = RE::PlayerCharacter::GetSingleton();
	auto* pSkills = a_pThis->GetInfoRuntimeData().skills;
	auto oldSkillData = pSkills->data->skills[skillOffset - 6];

	oldFunc(a_pThis, a_enumSkillId, a_fExperience);

	auto skillData = pSkills->data->skills[skillOffset - 6];
	if (skillData.levelThreshold > 0.f && a_fExperience > 0.f) {
		float deltaExp = skillData.xp - oldSkillData.xp;
		if (deltaExp < 0.f) {
			deltaExp += oldSkillData.levelThreshold;
		}
		pMeter->Add(static_cast<Skills>(skillOffset - 6), std::string(pSkillInfo->GetName()), deltaExp, skillData.xp, skillData.levelThreshold);
		DEBUG("AddSkillExperience [{}] {}, +{} , xp: ({}/{}), level: {}", skillOffset, pSkillInfo->GetName(), deltaExp, skillData.xp, skillData.levelThreshold, pValueOwner->GetActorValue(a_enumSkillId));
	};
}

void PlayerManager::Install()
{
	Utils::Hook::DetourStartup();

	INFO("Installing PlayerManager");
	auto pAddress = RELOCATION_ID(39413, 40488).address();
	Hook_AddSkillExperience::oldFunc = pAddress;
	::DetourAttach(reinterpret_cast<void**>(&Hook_AddSkillExperience::oldFunc), reinterpret_cast<void*>(Hook_AddSkillExperience::hooked));

	//RE::SkillIncrease::GetEventSource()->AddEventSink(this);

	Utils::Hook::DetourFinish();
}

RE::BSEventNotifyControl PlayerManager::ProcessEvent(RE::InputEvent* const* a_pEvent, RE::BSTEventSource<RE::InputEvent*>*)
{
	if (!a_pEvent) {
		return RE::BSEventNotifyControl::kContinue;
	}
	for (auto event = *a_pEvent; event; event = event->next) {
		if (const auto button = event->AsButtonEvent(); button) {
			RE::BSKeyboardDevice::Key key = static_cast<RE::BSKeyboardDevice::Key>(button->GetIDCode());
			if (key == RE::BSKeyboardDevice::Key::kDelete) {
				std::jthread thread([this] {
					RE::BSInputDeviceManager::GetSingleton()->RemoveEventSink(GetSingleton());
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					RE::BSInputDeviceManager::GetSingleton()->AddEventSink(GetSingleton());
				});
				Meter::GetSingleton()->Toggle();
			}
		}
	}
	return RE::BSEventNotifyControl::kContinue;
}
