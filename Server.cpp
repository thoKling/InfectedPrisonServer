#include "Server.h"

#include "SocketManager.h"
#include "ZombiesManager.h"
#include "ProjectilesManager.h"
#include "Weapon.h"
#include "Ammo.h"
#include "ItemsManager.h"

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
	Item* item = new Weapon();
	item->setPosition(500, 500);
	ItemsManager::addItem(item);
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
	_tick++;
	if (_zombiesSpawned < _currentWave*4 && _tick % 300 == 0 && SocketManager::getPlayers().size() > 0) {
		for (size_t i = 0; i < _currentWave; i++)
		{
			ZombiesManager::createZombie(sf::Vector2f(450 + rand() % 100, 150 + rand() % 100));
			Item* item = new Ammo(WeaponType::Gun);
			item->setPosition(500, 500);
			item->setStack(4);
			ItemsManager::addItem(item);
		}
		_zombiesSpawned++;
		_tick = 0;
	}
	if (_zombiesSpawned == _currentWave * 4 && ZombiesManager::getZombies().size() == 0) {
		_zombiesSpawned = 0;
		_currentWave++;
		sf::Packet packet;
		packet << SocketManager::PacketType::NextWave << _currentWave;
		SocketManager::broadcastPacket(packet);
	}
	ZombiesManager::update();
	ProjectilesManager::update();
}
