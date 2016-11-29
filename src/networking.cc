#include "networking.h"

WSADATA wsaData;
int iResult;

//1
bool initWS() {

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return false;
  }
  return true;
}
int connect(SOCKET* s, int ai_family, int ai_socktype, int ai_protocol, const char* IP) {
#ifdef _WIN32
  struct addrinfo *result = NULL,
    *ptr = NULL,
    hints;

  //1 
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = ai_family;
  hints.ai_socktype = ai_socktype;
  hints.ai_protocol = ai_protocol;


  iResult = getaddrinfo(IP, DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", iResult);
    WSACleanup();
    return -1;
  }
  if (ai_protocol == IPPROTO_TCP) {
    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

      // Create a SOCKET for connecting to server
      *s = socket(ptr->ai_family, ptr->ai_socktype,
        ptr->ai_protocol);
      if (*s == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return -2;
      }

      // Connect to server.
      iResult = connect(*s, ptr->ai_addr, (int)ptr->ai_addrlen);
      if (iResult == SOCKET_ERROR) {
        closesocket(*s);
        *s = INVALID_SOCKET;
        continue;
      }
      break;
    }
    freeaddrinfo(result);

    if (*s == INVALID_SOCKET) {
      printf("Unable to connect to server!\n");
      WSACleanup();
      return -3;
    }
    return 0;
  }
#endif
#ifdef __APPLE__
#endif
}

int sendTCP(SOCKET s, const char* buff, netPackage p) {
#ifdef _WIN32

  iResult = send(s, buff, (int)strlen(buff), 0);
  if (iResult == SOCKET_ERROR) {
    printf("send failed with error: %d\n", WSAGetLastError());
    //closesocket(s);
    //WSACleanup();
    return -4;
  }
  printf("Bytes Sent: %ld\n", iResult);

  //shutdown the connection since no more data will be sent
  iResult = shutdown(s, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    printf("shutdown failed with error: %d\n", WSAGetLastError());
    //closesocket(s);
    //WSACleanup();
    return -5;
  }
  return 0;
#endif
}

int recieveTCP(SOCKET s, netPackage* p) {
#ifdef _WIN32
  // Receive until the peer closes the connection
  char recvbuf[DEFAULT_BUFLEN];
  int iResult;
  int recvbuflen = DEFAULT_BUFLEN;
  do {
    iResult = recv(s, recvbuf, recvbuflen, 0);
    if (iResult > 0)
      printf("Bytes received: %d\n", iResult);
    else if (iResult == 0)
      printf("Connection closed\n");
    else {
      printf("recv failed with error: %d\n", WSAGetLastError());
    }
  } while (iResult > 0);
  return iResult;
#endif
}