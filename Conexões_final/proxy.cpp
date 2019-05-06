#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <clocale>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "sort.hpp"
#define PORT 8080
#define PORT_WORKER 5000

using namespace std;

int createConnection_5000(const char *message) {
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
  serv_addr.sin_port = htons(PORT_WORKER);

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
  cout << "----- Mensagens da comunicação cliente-servidor ----- \n";

  send( sock , message , strlen(message) , 0 );
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char buffer1[1024] = {0};
    char *response = "Servidor: Dados recebidos com sucesso.";
    srand(time(NULL));
    int j = 1, i=0;
    bool verificado = false, escutando = true;
    string task;
    vector<string> bag_of_tasks = {""};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080

      if (bind(server_fd, (struct sockaddr *)&address,
                                   sizeof(address))<0)
      {
          perror("bind failed");
          exit(EXIT_FAILURE);
      }

while (1) {
          if (listen(server_fd, 3) < 0)
          {
              perror("listen");
              exit(EXIT_FAILURE);
          }
          
          if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
          {
              perror("accept");
              exit(EXIT_FAILURE);
          }

          read( new_socket , buffer1, sizeof("[0;0;0]"));
          task = buffer1;

          //clean the receiver buffer
          for(int k = 0; k < sizeof (buffer1); k++)
          	buffer1[k] = {0};

          bag_of_tasks.push_back(task);

          cout << "Bag of tasks: ";
          for (int k = 0; k < bag_of_tasks.size(); k++)
          	cout << bag_of_tasks.at(k) << " ";

          send(new_socket , response, strlen(response) , 0);

          cout << endl;
          cout << "Mensagem recebida: " << task << endl;

          char *p = &task.at(0);

          char *message;
          message = new char[1024];

          message = p;
          createConnection_5000(message);
        }
    return 0;
}
