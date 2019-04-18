#include "Server.h"

#include "SocketManager.h"
#include "ZombiesManager.h"
#include "ProjectilesManager.h"

#include <iostream>

Server::Server()
{

}


Server::~Server()
{
}

// Lancement du serveur
void Server::start() {
	sf::Thread threadSocket(&SocketManager::start);
	threadSocket.launch();

	ZombiesManager::createZombie({ 450,150 });
	///
	/// Boucle principale
	///
	sf::Clock clock;
	sf::Time lag = sf::seconds(0.0f);
	while (!_done)
	{
		lag += clock.getElapsedTime();
		clock.restart();
		// On update
		// Game Pattern : gameLoop : http://gameprogrammingpatterns.com/game-loop.html
		while (lag.asMilliseconds() >= MS_PER_UPDATE) {
			update();
			lag -= sf::milliseconds(MS_PER_UPDATE);
		}
	}
}

void Server::update()
{
	ZombiesManager::update();
	ProjectilesManager::update();
}
