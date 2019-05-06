// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#define PORT 8080
#include "sort.hpp"
using namespace std;

int createConnection(const char *message) {
  struct sockaddr_in address;
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      printf("\n Socket creation error \n");
      return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
  {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
  }
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      printf("\nConnection Failed \n");
      return -1;
  }

  cout << endl;
  cout << "----- Mensagens da comunicação cliente-worker ----- \n";

  send( sock , message , strlen(message) , 0 );
  //printf("Este cliente: Voto enviado.\n");

  read( sock , buffer, 50);
  printf("%s\n",buffer );

    cout << "-------------------------------------------------------- \n";
}



int main() {
  char *message;
  message = new char[1024];

  //Sending pixels
  
  string aux;

  for(int i = 0; i < SIZE_X; i++)
  {
    for (int j = 0; j < SIZE_Y; j++)
    {
      aux = to_string(i) + '@' + to_string(j) + "@0"; 

      cout << aux;

      char *p = &aux.at(0);

      message = p;

      createConnection(message);
    }
  }

	return 0;
}
