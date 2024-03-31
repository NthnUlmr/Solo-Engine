#include "AudioSystem.h"
#include "AudioEngine.h"
namespace Solo {

	void AudioSystem::init(std::shared_ptr<Scene> scene)
	{
		AudioEngine::Init();
	}

	bool AudioSystem::update(std::shared_ptr<Scene> scene, const TimeStep& dt)
	{
		AudioEngine::Update(0.1);
		return true;
	}

	void AudioSystem::destroy(std::shared_ptr<Scene> scene)
	{
		AudioEngine::Shutdown();
	}

} // namespace Solo
