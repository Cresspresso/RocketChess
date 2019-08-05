
#include <WinSock2.h>
#include <sstream>

#include "find_container.hpp"
#include "lambda.hpp"
#include "time.hpp"

#include "game.hpp"

#include "debug_log.hpp"

namespace
{
	float g_timeOfLastNetworkUpdate = 0;

	constexpr size_t maxUsernameLength = 32;
	constexpr size_t maxTransformLength = 256;
}

void Bullet::updateMotion()
{
	transform.position += velocity * Time::getDeltaTime();
}

void Player::update()
{
	transform.position += vec3(0.1f, 0, 0) * Time::getDeltaTime();
}

void Game::processPacket(PacketData const & data)
{
	NetworkEvent eventType = static_cast<NetworkEvent>(data.at(0));
	switch (eventType)
	{
	case NetworkEvent::GameUpdate:
	{
		processGameUpdatePacket(data);
		break;
	}
	case NetworkEvent::GameOver:
	{
		isGameOver = true;
		break;
	}
	default:
		throw std::runtime_error{ "invalid enum value" };
		break;
	}
}

void Game::processGameUpdatePacket(PacketData const& data)
{
	GameUpdateNetworkEvent updateType = static_cast<GameUpdateNetworkEvent>(data.at(1));
	switch (updateType)
	{
	case GameUpdateNetworkEvent::BulletsResize:
	{
		auto const deser = deserializeResize(data);
		size_t const newSize = static_cast<size_t>(deser.size);
		size_t const oldSize = bullets.size();
		bullets.resize(deser.size);
		for (size_t i = oldSize; i < newSize; ++i)
		{
			auto& bullet = bullets[i];
			bullet.id = i;
		}
		break;
	}
	case GameUpdateNetworkEvent::EnemiesResize:
	{
		auto const deser = deserializeResize(data);
		size_t const newSize = static_cast<size_t>(deser.size);
		size_t const oldSize = enemies.size();
		enemies.resize(deser.size);
		for (size_t i = oldSize; i < newSize; ++i)
		{
			auto& enemy = enemies[i];
			enemy.id = i;
		}
		break;
	}
	case GameUpdateNetworkEvent::PlayerTransform:
	{
		auto const deser = Player::deserializeTransform(data);
		players.at(deser.username).apply(deser);
		break;
	}
	case GameUpdateNetworkEvent::PlayerOtherDetails:
	{
		auto const deser = Player::deserializeOtherDetails(data);
		players.at(deser.username).apply(deser);
		break;
	}
	case GameUpdateNetworkEvent::EnemyTransform:
	{
		auto const deser = Enemy::deserializeTransform(data);
		enemies.at(deser.id).apply(deser);
		break;
	}
	case GameUpdateNetworkEvent::BulletTransform:
	{
		auto const deser = Bullet::deserializeTransform(data);
		bullets.at(deser.id).apply(deser);
		break;
	}
	default:
		throw std::runtime_error{ "invalid enum value" };
		break;
	}
}

void Game::update()
{
	if (isServer)
	{
		// Ensure all clients have an associated player.
		for (auto const& [addr, clientUsername] : clients)
		{
			// If players does not contain a player with this client username...
			if (!container::hasIf(players, LAM(pair, pair.first == clientUsername)))
			{
				// Add player with that username.
				auto const [it, _] = players.emplace(std::make_pair(clientUsername, Player{}));
				auto& [_2, player] = *it;
				player.username = clientUsername;
			}
		}

		// Erase any players with unknown usernames.
		// TODO inform other clients that the client has dropped.
		container::eraseIf(players,
			[this](std::pair<std::string, Player> const& playerPair)
		{
			return !container::hasIf(clients,
				[&playerPair](std::pair<sockaddr_in, std::string> const& clientPair)
			{
				return clientPair.second == playerPair.first;
			});
		});
		
		// Check for Game Over Conditions.
		// If all players are dead...
		if (std::all_of(players.begin(), players.end(), LAM(pair, pair.second.isDead())))
		{
			sendToAllClients(serializeGameOver());
			isGameOver = true;
			// TODO stop network
		}

		updateWorld();

		// Network Update Timer.
		tickTimer -= Time::getDeltaTime();
		if (tickTimer <= 0)
		{
			tickTimer = tickCooldown;
			g_timeOfLastNetworkUpdate = Time::getCurrentTime();

			// Send the Game State Snapshot to all Clients.
			for (auto const& [_, player] : players)
			{
				sendToAllClients(player.serializeTransform());
				sendToAllClients(player.serializeOtherDetails());
			}

			for (auto const& enemy : enemies)
			{
				sendToAllClients(enemy.serializeTransform());
			}

			for (auto const& bullet : bullets)
			{
				sendToAllClients(bullet.serializeTransform());
			}
		}
	}
	else //! if (isServer)
	{
		// TODO process received packets

		// Client side prediction.
		// Predicting bullet updates locally.
		for (auto& bullet : bullets)
		{
			bullet.updateMotion();
		}
		players[username].update(); // Predicting player movement locally.

		// TODO send update to server
		tickTimer -= Time::getDeltaTime();
		if (tickTimer <= 0)
		{
			tickTimer = tickCooldown;
			g_timeOfLastNetworkUpdate = Time::getCurrentTime();

			// Send player position updates.
			auto const& player = players[username];
			sendToServer(player.serializeTransform());
			sendToServer(player.serializeOtherDetails());
		}
	}
	//~ else
}

void Game::updateWorld()
{
	DEBUG_LOG_LINK("Update World!");
	for (auto const&[username, player] : players)
	{
		DEBUG_LOG_LINK(toString(player.transform.position));
	}
}

PacketData Game::serializeGameOver() const
{
	std::vector<Byte> data(1);
	data[0] = static_cast<Byte>(NetworkEvent::GameOver);
	return data;
}

PacketData Game::serializeEnemiesResize() const
{
	std::vector<Byte> data(1 + sizeof(unsigned long));

	size_t n = 0;
	// Network Event
	data[n++] = static_cast<Byte>(NetworkEvent::GameUpdate);
	// Game Update Network Event
	data[n++] = static_cast<Byte>(GameUpdateNetworkEvent::EnemiesResize);
	// Size
	*reinterpret_cast<unsigned long*>(&data[n]) = htonl(static_cast<unsigned long>(enemies.size()));
	n += sizeof(unsigned long);

	return data;
}

PacketData Game::serializeBulletsResize() const
{
	std::vector<Byte> data(1 + sizeof(unsigned long));

	size_t n = 0;
	// Network Event
	data[n++] = static_cast<Byte>(NetworkEvent::GameUpdate);
	// Game Update Network Event
	data[n++] = static_cast<Byte>(GameUpdateNetworkEvent::BulletsResize);
	// Size
	*reinterpret_cast<unsigned long*>(&data[n]) = htonl(static_cast<unsigned long>(bullets.size()));
	n += sizeof(unsigned long);

	return data;
}

ResizePacket Game::deserializeResize(PacketData const & data)
{
	ResizePacket params;

	// Network Event
	params.header.eventType = static_cast<NetworkEvent>(data.at(0));
	assert(params.header.eventType == NetworkEvent::GameUpdate);
	// Game Update Network Event
	params.header.updateType = static_cast<GameUpdateNetworkEvent>(data.at(1));
	assert(
		params.header.updateType == GameUpdateNetworkEvent::BulletsResize
		|| params.header.updateType == GameUpdateNetworkEvent::EnemiesResize
	);
	// Size
	params.size = ntohl(*reinterpret_cast<unsigned long const*>(&data[2]));
	
	return params;
}

std::string Transform::stringify() const
{
	std::stringstream s;
	s << position.x << ';';
	s << position.y << ';';
	s << position.z << '|';

	s << rotation.x << ';';
	s << rotation.y << ';';
	s << rotation.z << ';';
	s << rotation.w << '|';

	s << scale.x << ';';
	s << scale.y << ';';
	s << scale.z;

	return s.str();
}

Transform Transform::destringify(std::string const& data)
{
	Transform params;

	std::stringstream s{ data };

	char c;
	s >> params.position.x >> c; assert(c == ';');
	s >> params.position.y >> c; assert(c == ';');
	s >> params.position.z >> c; assert(c == '|');

	s >> params.rotation.x >> c; assert(c == ';');
	s >> params.rotation.y >> c; assert(c == ';');
	s >> params.rotation.z >> c; assert(c == ';');
	s >> params.rotation.w >> c; assert(c == '|');

	s >> params.scale.x >> c; assert(c == ';');
	s >> params.scale.y >> c; assert(c == ';');
	s >> params.scale.z;

	return params;
}

std::vector<Byte> Player::serializeTransform() const
{
	// TODO also include timestamp using g_timeOfLastNetworkUpdate
	// [ Game Update | Player Transform Update | Player Username | Position | Rotation | Scale ]

	std::string const str = transform.stringify();

	std::vector<Byte> data(2 + username.size() + 1 + str.size() + 1);
	ZeroMemory(data.data(), data.size());

	size_t n = 0;
	// Network Event
	data[n++] = static_cast<Byte>(NetworkEvent::GameUpdate);
	// Game Update Network Event
	data[n++] = static_cast<Byte>(GameUpdateNetworkEvent::PlayerTransform);
	// Username
	strcpy_s(&data[n], username.size(), username.c_str());
	n += username.size() + 1;
	// Transform Data
	strcpy_s(&data[n], str.size(), str.c_str());
	n += str.size() + 1;

	return data;
}

std::vector<Byte> Player::serializeOtherDetails() const
{
	// [ Game Update | Player Other Details | Player Username | Health | Score ]
	std::vector<Byte> data(2 + username.size() + 1 + sizeof(unsigned short) + sizeof(unsigned long));
	ZeroMemory(data.data(), data.size());

	size_t n = 0;
	// Network Event
	data[n++] = static_cast<Byte>(NetworkEvent::GameUpdate);
	// Game Update Network Event
	data[n++] = static_cast<Byte>(GameUpdateNetworkEvent::PlayerOtherDetails);
	// Username
	strcpy_s(&data[n], username.size(), username.c_str());
	n += username.size() + 1;
	// Health
	*reinterpret_cast<unsigned short*>(&data[n]) = htons(health);
	n += sizeof(unsigned short);
	// Score
	*reinterpret_cast<unsigned long*>(&data[n]) = htonl(score);
	n += sizeof(unsigned long);

	return data;
}

PlayerTransformPacket Player::deserializeTransform(PacketData const & data)
{
	PlayerTransformPacket params;

	// Network Event
	params.header.eventType = static_cast<NetworkEvent>(data.at(0));
	assert(params.header.eventType == NetworkEvent::GameUpdate);
	// Game Update Network Event
	params.header.updateType = static_cast<GameUpdateNetworkEvent>(data.at(1));
	assert(params.header.updateType == GameUpdateNetworkEvent::PlayerTransform);
	// Username
	size_t const a = std::strlen(&data.at(2));
	params.username = std::string{ &data[2], a };

	size_t n = 2 + a + 1;
	// Transform
	params.transform = Transform::destringify(std::string{ &data.at(n) });

	return params;
}

PlayerOtherDetailsPacket Player::deserializeOtherDetails(PacketData const & data)
{
	PlayerOtherDetailsPacket params;

	// Network Event
	params.header.eventType = static_cast<NetworkEvent>(data.at(0));
	assert(params.header.eventType == NetworkEvent::GameUpdate);
	// Game Update Network Event
	params.header.updateType = static_cast<GameUpdateNetworkEvent>(data.at(1));
	assert(params.header.updateType == GameUpdateNetworkEvent::PlayerOtherDetails);
	// Username
	size_t const a = std::strlen(&data.at(2));
	params.username = std::string{ &data[2], a };

	size_t n = 2 + a + 1;
	assert(data.size() >= n + sizeof(unsigned short) + sizeof(unsigned long));
	// Health
	params.health = ntohs(*reinterpret_cast<unsigned short const*>(&data[n]));
	n += sizeof(unsigned short);
	// Score
	params.score = ntohl(*reinterpret_cast<unsigned long const*>(&data[n]));
	n += sizeof(unsigned long);

	return params;
}

void Player::apply(PlayerTransformPacket const & params)
{
	assert(username == params.username);
	transform = params.transform;
}

void Player::apply(PlayerOtherDetailsPacket const & params)
{
	assert(username == params.username);
	health = std::max<unsigned short>(0, params.health);
	score = std::max<unsigned long>(0, params.score);
}

std::vector<Byte> Enemy::serializeTransform() const
{
	// [ Game Update | Enemy Transform Update | Enemy ID | Enemy Type | Position | Rotation | Scale ]

	std::string const str = transform.stringify();

	std::vector<Byte> data(2 + sizeof(unsigned long) + 1 + str.size() + 1);
	ZeroMemory(data.data(), data.size());

	size_t n = 0;
	// Network Event
	data[n++] = static_cast<Byte>(NetworkEvent::GameUpdate);
	// Game Update Network Event
	data[n++] = static_cast<Byte>(GameUpdateNetworkEvent::EnemyTransform);
	// ID
	*reinterpret_cast<unsigned long*>(&data[n]) = htonl(id);
	n += sizeof(unsigned long);
	// Type
	data[n++] = static_cast<Byte>(type);
	// Transform Data
	strcpy_s(&data[n], str.size(), str.c_str());
	n += str.size() + 1;

	return data;
}

EnemyTransformPacket Enemy::deserializeTransform(PacketData const & data)
{
	EnemyTransformPacket params;

	// Network Event
	params.header.eventType = static_cast<NetworkEvent>(data.at(0));
	assert(params.header.eventType == NetworkEvent::GameUpdate);
	// Game Update Network Event
	params.header.updateType = static_cast<GameUpdateNetworkEvent>(data.at(1));
	assert(params.header.updateType == GameUpdateNetworkEvent::EnemyTransform);

	size_t n = 2;
	assert(data.size() >= n + sizeof(unsigned long) + 1 + 1);
	// ID
	params.id = ntohl(*reinterpret_cast<unsigned long const*>(&data[n]));
	n += sizeof(unsigned long);
	// Type
	params.enemyType = static_cast<EnemyType>(data[n]);
	++n;
	// Transform
	std::string const str{ &data.at(n) };
	params.transform = Transform::destringify(str);
	n += str.length() + 1;

	return params;
}

void Enemy::apply(EnemyTransformPacket const & params)
{
	assert(id == params.id);
	transform = params.transform;
}

std::string Bullet::stringifyVelocity() const
{
	std::stringstream s;
	s << velocity.x << ';';
	s << velocity.y << ';';
	s << velocity.z;
	return s.str();
}

vec3 Bullet::destringifyVec3(std::string const & data)
{
	vec3 vec;

	std::stringstream s{ data };

	char c;
	s >> vec.x >> c; assert(c == ';');
	s >> vec.y >> c; assert(c == ';');
	s >> vec.z;

	return vec;
}

std::vector<Byte> Bullet::serializeTransform() const
{
	// [ Game Update | Bullet Transform Update | Enemy ID | Enemy Type | Position | Rotation | Scale ]

	std::string const str = transform.stringify();
	std::string const strVel = stringifyVelocity();

	std::vector<Byte> data(2 + sizeof(unsigned long) + 1 + str.size() + 1 + strVel.size() + 1);
	ZeroMemory(data.data(), data.size());

	size_t n = 0;
	// Network Event
	data[n++] = static_cast<Byte>(NetworkEvent::GameUpdate);
	// Game Update Network Event
	data[n++] = static_cast<Byte>(GameUpdateNetworkEvent::BulletTransform);
	// ID
	*reinterpret_cast<unsigned long*>(&data[n]) = htonl(id);
	n += sizeof(unsigned long);
	// Transform Data
	strcpy_s(&data[n], str.size(), str.c_str());
	n += str.size() + 1;
	// Velocity
	strcpy_s(&data[n], strVel.size(), strVel.c_str());
	n += strVel.size() + 1;

	return data;
}

BulletTransformPacket Bullet::deserializeTransform(PacketData const & data)
{
	BulletTransformPacket params;

	// Network Event
	params.header.eventType = static_cast<NetworkEvent>(data.at(0));
	assert(params.header.eventType == NetworkEvent::GameUpdate);
	// Game Update Network Event
	params.header.updateType = static_cast<GameUpdateNetworkEvent>(data.at(1));
	assert(params.header.updateType == GameUpdateNetworkEvent::BulletTransform);

	size_t n = 2;
	assert(data.size() >= n + sizeof(unsigned long) + 2);
	// ID
	params.id = ntohl(*reinterpret_cast<unsigned long const*>(&data[n]));
	n += sizeof(unsigned long);
	// Transform
	std::string const str{ &data.at(n) };
	params.transform = Transform::destringify(str);
	n += str.length() + 1;
	// Velocity
	std::string const str2{ &data.at(n) };
	params.velocity = destringifyVec3(str2);
	n += str2.length() + 1;

	return params;
}

void Bullet::apply(BulletTransformPacket const & params)
{
	assert(id == params.id);
	transform = params.transform;
	velocity = params.velocity;
}
