
#include "console.hpp"
#include "to_string.hpp"

#include "network_manager.hpp"



ReturnCode NetworkManager::init()
{
	TRY_CATCH_RE(network.Startup());
	return RC_SUCCESS;
}



void NetworkManager::destroy() noexcept
{
	TRY_CATCH_PRINT(network.Cleanup());
}



ReturnCode NetworkManager::openClient(std::string const& username)
{
	if (NetworkState::None != state)
	{
		throw std::runtime_error{ "network manager is already open" };
	}

	state = NetworkState::Client;
	isOpen_ = true;

	try
	{
		client = std::make_unique<Client>(username);

		// Broadcast to detect servers.
		auto const validServers = client->receiveBroadcastResponses();

		// Print the servers that were found.
		if (validServers.empty())
		{
			console::log("No servers found.");
		}
		else
		{
			for (size_t i = 0; i < validServers.size(); ++i)
			{
				console::log(stringLink("Found server [", i, "] at : ", validServers[i].GetString()));
			}

			// Join the first server.
			// TODO let user choose
			client->connectTo(validServers[0]);
		}

		// Start the threads.
		client->startThreads(isOpen_);
	}
	catch (...)
	{
		close();
		FAIL_RE(stringException());
	}

	return RC_SUCCESS;
}



ReturnCode NetworkManager::openHost(std::string const& servername, std::string const& username)
{
	if (NetworkState::None != state)
	{
		throw std::runtime_error{ "network manager is already open" };
	}

	state = NetworkState::Host;
	isOpen_ = true;

	try
	{
		server = std::make_unique<Server>(servername);
		server->startThreads(isOpen_);

		client = std::make_unique<Client>(username);
		client->startThreads(isOpen_);
		client->connectTo(server->getAddress());
	}
	catch (...)
	{
		std::string const msg = stringException();
		console::error(msg);
		close();
		FAIL_RE("Could not open Host network entities.");
	}

	return RC_SUCCESS;
}



void NetworkManager::close() noexcept
{
	if (client)
	{
		TRY_CATCH_PRINT(client->dropConnection());
	}

	// stop threads
	isOpen_ = false;

	// join threads
	client.reset();
	server.reset();

	state = NetworkState::None;
}



ReturnCode NetworkManager::update()
{
	if (!isOpen_) { return RC_SUCCESS; }

	// Main Thread
	try
	{
		if (server) { server->updateProcess(); }
	}
	CATCH_RE();
	try
	{
		if (client) { client->updateProcess(); }
	}
	CATCH_RE();

	return RC_SUCCESS;
}
