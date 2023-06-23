#include "PlayerManager.h"
#include "RendererManager.h"

void MessageHandler(SKSE::MessagingInterface::Message* a_msg) noexcept
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostPostLoad:
		break;
	case SKSE::MessagingInterface::kInputLoaded:
		RE::BSInputDeviceManager::GetSingleton()->AddEventSink(PlayerManager::GetSingleton());
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		break;
	default:
		break;
	}
}

DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());

	SKSE::Init(a_skse);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);
	SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);

	PlayerManager::GetSingleton()->Install();
	RendererManager::GetSingleton()->Install();

	return true;
}
