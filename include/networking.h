#pragma once

#define DEFAULT_BUFLEN 1204

#include <SFML\Network.hpp>
#include "types.h"

int connect(sf::TcpSocket* socket,
  const char* server, unsigned int port);

int bindUDP(sf::UdpSocket* socket,
  unsigned int port);

int sendTCP(sf::TcpSocket* socket, netTCPPackage package);

int sendUDP(sf::UdpSocket* socket, 
  const char* server, unsigned int port, netUDPPackage package); 

netTCPPackage* recieveTCP(sf::TcpSocket* socket, netTCPPackage* package);

netUDPPackage* recieveUDP(sf::UdpSocket* socket,
  sf::IpAddress& sender, unsigned short port);