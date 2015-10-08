#pragma once
#include <SFML/Network.hpp>
#include "Game.h"

/// When a GameClient connects, we assign it to a Client structure stored on
/// the server. The Client is made up of a Socket and Player.
struct Client
{
	sf::TcpSocket socket;
	Player *player;
};

/// The GameServer accepts incoming GameClient connections and runs the Ping
/// simulation. The server is authoratative and sends snapshots to the clients
/// at a regular interval, so they can stay in sync.
class GameServer : public Game, public StageObserver
{
public:
	Client client[2];
	sf::TcpListener serverSocket;

	GameServer(void);
	~GameServer(void);
	
	// Interface
    virtual bool initialize() override;
    virtual void shutdown() override;
    virtual void update(float deltaTime) override;
    virtual void ballHitTop() override;
    virtual void ballHitBottom() override;

	/// Sends a packet to both of the clients.
	/// @param [in]	p	The Packet to send.
	void sendMessage(sf::Packet &p);

	/// Check to see whether any network data has been received
	void pollMessages();

	/// Wait for incoming client connections.
	/// @return	true if two clients connect successfully, false otherwise.
	bool waitForClients();

	float syncRate;			///< Delay between sending game sync messages
	float timeUntilSync;	///< Time left before sending game sync message
};

