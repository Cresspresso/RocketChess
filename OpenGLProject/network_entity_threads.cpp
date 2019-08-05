
#include "network_entity.hpp"
#include "errors.hpp"

#include "network_entity_threads.hpp"



namespace
{
	void ReceiveThreadFunc(std::atomic_bool& whileCondition, NetworkEntity& entity)
	{
		while (whileCondition)
		{
			try
			{
				entity.updateReceive();
			}
			CATCH_PRINT();
		}
	}



	void KeepAliveThreadFunc(std::atomic_bool& whileCondition, NetworkEntity& entity)
	{
		while (whileCondition)
		{
			try
			{
				entity.updateKeepAlive();
			}
			CATCH_PRINT();
		}
	}



	//void ProcessThreadFunc(std::atomic_bool& whileCondition, NetworkEntity& entity)
	//{
	//	while (whileCondition)
	//	{
	//		try
	//		{
	//			entity.updateProcess();
	//		}
	//		CATCH_PRINT();
	//	}
	//}



	void destroyThreadPtr(std::unique_ptr<std::thread>& p)
	{
		if (p)
		{
			p->join();
		}
		p.reset();
	}
}



void NetworkEntityThreads::start(std::atomic_bool& whileCondition, NetworkEntity& net)
{
	if (isRunning())
	{
		throw std::runtime_error{ "threads are already running" };
	}

	assert(!receive);
	receive = std::make_unique<std::thread>(ReceiveThreadFunc, std::ref(whileCondition), std::ref(net));

	assert(!keepAlive);
	keepAlive = std::make_unique<std::thread>(KeepAliveThreadFunc, std::ref(whileCondition), std::ref(net));

	/*assert(!process);
	process = std::make_unique<std::thread>(ProcessThreadFunc, std::ref(whileCondition), std::ref(net));*/
}



void NetworkEntityThreads::stop()
{
	//destroyThreadPtr(process);
	destroyThreadPtr(keepAlive);
	destroyThreadPtr(receive);
}



NetworkEntityThreads::~NetworkEntityThreads()
{
	stop();
}
