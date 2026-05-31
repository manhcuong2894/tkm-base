#pragma once
#include <mutex>
#include <condition_variable>

class LogicThreadManager
{
public:
	LogicThreadManager();
	~LogicThreadManager();

	void Start();
	void Stop();
	void SetSceneActive(bool active);
	void SetLogicFunction(const std::function<void()>& func);

private:
	void ThreadLoop();

	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable m_condition;
	std::atomic<bool> m_running;
	std::atomic<bool> m_sceneActive;
	std::function<void()> m_logicFunc;
};


