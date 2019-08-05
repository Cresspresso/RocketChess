
#pragma once

#include <thread>

#include "errors.hpp"

#include "network.hpp"
#include "server.hpp"
#include "client.hpp"
#include "network_state.hpp"



class NetworkManager
{
public:
	ReturnCode init();
	void destroy() noexcept;

	ReturnCode openClient(std::string const& username);
	ReturnCode openHost(std::string const& servername, std::string const& username);
	void close() noexcept;

	NetworkState getState() const { return state; }
	std::atomic_bool const& isOpen() { return isOpen_; }

	ReturnCode update();

private:
	Network network;

	std::atomic_bool isOpen_ = false;
	NetworkState state = NetworkState::None;
	std::unique_ptr<Client> client;
	std::unique_ptr<Server> server;
};
