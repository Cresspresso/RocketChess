
#pragma once

#include <memory>
#include <thread>
#include <atomic>

class NetworkEntity;

class NetworkEntityThreads
{
public:
	~NetworkEntityThreads();

	void start(std::atomic_bool& whileCondition, NetworkEntity& net);
	void stop();

	bool isRunning() const { return receive != nullptr; }

private:
	std::unique_ptr<std::thread> receive;
	std::unique_ptr<std::thread> keepAlive;
	//std::unique_ptr<std::thread> process;
};
