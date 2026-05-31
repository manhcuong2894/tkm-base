#include "stdafx.h"
#include "LogicThreadManager.h"

LogicThreadManager::LogicThreadManager()
	: m_running(false), m_sceneActive(false)
{
}

LogicThreadManager::~LogicThreadManager()
{
	Stop();
}

void LogicThreadManager::Start()
{
	if (m_running)
		return;

	m_running = true;
	m_thread = std::thread(&LogicThreadManager::ThreadLoop, this);
}

void LogicThreadManager::Stop()
{
	m_running = false;
	m_condition.notify_all();

	if (m_thread.joinable())
		m_thread.join();
}

void LogicThreadManager::SetSceneActive(bool active)
{
	if (m_sceneActive != active)
	{
		m_sceneActive = active;
		if (active)
			m_condition.notify_one();
	}
}

void LogicThreadManager::SetLogicFunction(const std::function<void()>& func)
{
	m_logicFunc = func;
}

void LogicThreadManager::ThreadLoop()
{
	const int fixedFPS = 60;
	const std::chrono::milliseconds frameTime(1000 / gsteady_clock->GetLimitFps());

	while (m_running)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, [&]() { return m_sceneActive || !m_running; });

		if (!m_running)
			break;

		lock.unlock();

		auto start = std::chrono::steady_clock::now();

		if (m_logicFunc)
			m_logicFunc();

		auto elapsed = std::chrono::steady_clock::now() - start;
		if (elapsed < frameTime)
			std::this_thread::sleep_for(frameTime - elapsed);
	}
}
