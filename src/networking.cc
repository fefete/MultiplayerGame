#include "networking.h"



int connect(sf::TcpSocket* socket, const char * server, unsigned int port)
{
  sf::Socket::Status status = socket->connect(server, port);
  if (status != sf::Socket::Done)
  {
    //printf("\nERROR CONNECTING\n");
    return 0;
  }
  return 1;
}

int bindUDP(sf::UdpSocket* socket, unsigned int port)
{
  // bind the socket to a port
  if (socket->bind(port) != sf::Socket::Done)
  {
    //printf("\nBINDING ERROR\n");
    return -1;
  }
  return 0;
}

int sendTCP(sf::TcpSocket* socket, netTCPPackage package)
{
  // TCP socket:
  if (socket->send(package.system_message.c_str(), 100) != sf::Socket::Done)
  {
    //printf("\nTCP SENDING ERROR\n");
    return -1;
  }
  return 0;
}

int sendUDP(sf::UdpSocket* socket, const char * recipient, unsigned int port, netUDPPackage package)
{
  if (socket->send(package.system_message.c_str(), 255, recipient, port) != sf::Socket::Done)
  {
   // printf("\nUDP SENDING ERROR\n");
    return -1;
  }
  return 0;
}

netTCPPackage* recieveTCP(sf::TcpSocket* socket, netTCPPackage * package)
{
  std::size_t received;
  void* buffer_data = malloc(100);
  // TCP socket:
  if (socket->receive(buffer_data, 100, received) != sf::Socket::Done)
  {
    //printf("\nTCP RECIEVING ERROR\n");
    delete buffer_data;
    return nullptr;
  }
  netTCPPackage* l_package = new netTCPPackage();
  char* message = (char*)buffer_data;
  l_package->system_message = std::string(message);
  delete buffer_data;
  return l_package;
}

netUDPPackage* recieveUDP(sf::UdpSocket * socket, sf::IpAddress& sender, unsigned short port)
{
  std::size_t received;
  char* buffer_data[255];
  if (socket->receive(buffer_data, 255, received, sender, port) != sf::Socket::Done)
  {
    //printf("\nUDP RECIEVE ERROR\n");
    return nullptr;
  }
  netUDPPackage* l_package = new netUDPPackage();
  char* message = (char*)buffer_data;
  l_package->system_message = std::string(message);
  //printf("%s\n", l_package->system_message.c_str());
  return l_package;
}






