#pragma once
#include "Utils.h"

class PlayerManager final :
	public Singleton<PlayerManager>,
	public RE::BSTEventSink<RE::InputEvent*>
{
public:
	void Install();

private:
	RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_pEvent, RE::BSTEventSource<RE::InputEvent*>*);

	HOOK_MAKE_THIS(AddSkillExperience, void, RE::PlayerCharacter, RE::ActorValue, float)
};