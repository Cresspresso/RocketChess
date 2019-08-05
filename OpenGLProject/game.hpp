
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <string_view>

#include "common.hpp"

//#include "network_entity.hpp"



using Byte = char;
using PacketData = std::vector<Byte>;

enum class NetworkEvent : Byte
{
	GameUpdate,
	GameOver,
};

enum class GameUpdateNetworkEvent : Byte
{
	PlayerTransform,
	PlayerOtherDetails,
	EnemiesResize,
	EnemyTransform,
	BulletsResize,
	BulletTransform,
};



struct Transform
{
	vec3 position = vec3();
	quat rotation = quat();
	vec3 scale = vec3(1);

	std::string stringify() const;
	static Transform destringify(std::string const& data);
};

enum class EnemyType : Byte
{
	Alpha,
	Beta,
};



struct UpdatePacket
{
	NetworkEvent eventType = NetworkEvent::GameUpdate;
	GameUpdateNetworkEvent updateType = GameUpdateNetworkEvent::PlayerTransform;
};

struct ResizePacket
{
	UpdatePacket header;
	unsigned long size = 0;
};

struct PlayerTransformPacket
{
	UpdatePacket header;
	std::string username;
	Transform transform;
};

struct PlayerOtherDetailsPacket
{
	UpdatePacket header;
	std::string username;
	unsigned short health = 0;
	unsigned long score = 0;
};

struct EnemyTransformPacket
{
	UpdatePacket header;
	unsigned long id = 0;
	EnemyType enemyType = EnemyType::Alpha;
	Transform transform;
};

struct BulletTransformPacket
{
	UpdatePacket header;
	unsigned long id = 0;
	Transform transform;
	vec3 velocity = vec3(0);
};



struct Player
{
	std::string username;
	Transform transform;
	unsigned short health = 3;
	unsigned long score = 0;
	bool isDead() const { return health <= 0; }

	void update();

	PacketData serializeTransform() const;
	PacketData serializeOtherDetails() const;
	static PlayerTransformPacket deserializeTransform(PacketData const& data);
	static PlayerOtherDetailsPacket deserializeOtherDetails(PacketData const& data);
	void apply(PlayerTransformPacket const& params);
	void apply(PlayerOtherDetailsPacket const& params);
};

struct Enemy
{
	unsigned long id = 0;
	Transform transform;
	EnemyType type = EnemyType::Alpha;

	PacketData serializeTransform() const;
	static EnemyTransformPacket deserializeTransform(PacketData const& data);
	void apply(EnemyTransformPacket const& params);
};

struct Bullet
{
	unsigned long id = 0;
	Transform transform;
	vec3 velocity;

	void updateMotion();

	std::string stringifyVelocity() const;
	static vec3 destringifyVec3(std::string const& data);

	PacketData serializeTransform() const;
	static BulletTransformPacket deserializeTransform(PacketData const& data);
	void apply(BulletTransformPacket const& params);
};



class Game
{
	//std::unique_ptr<INetworkEntity> networkEntity = nullptr;
	std::string username;
	bool isServer;
	bool isGameOver = false;

	std::vector<Enemy> enemies;
	std::vector<Bullet> bullets;
	std::map<std::string, Player> players; // username -> player

	std::map<sockaddr_in, std::string> clients; // address -> username

	float tickTimer = 0;
	float tickCooldown = 1.0f / 60.0f;

	void sendToAllClients(PacketData const& data) { throw "not implemented"; };
	void sendToServer(PacketData const& data) { throw "not implemented"; };
	void processPacket(PacketData const& data);
	void processGameUpdatePacket(PacketData const& data);

	void update();
	void updateWorld();

	PacketData serializeGameOver() const;
	PacketData serializeEnemiesResize() const;
	PacketData serializeBulletsResize() const;
	ResizePacket deserializeResize(PacketData const& data);
};
