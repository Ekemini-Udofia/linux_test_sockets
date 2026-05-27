#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

inline static void error(const std::string& msg) {
  std::cerr << msg << std::endl;
  std::exit(1);
}

int main(int argc, char* argv[]) {
  int sock_file_descriptor, port_number;
  size_t n{};
  sockaddr_in serv_addr;
  hostent* server;

  char buffer[256]{};
  std::string send_buffer{}, recv_buffer(256, '\0');
  if(argc < 3) {
    error("Incomplete argument: client hostname port_number");
  }

  port_number = std::stoi(argv[2]);
  sock_file_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock_file_descriptor < 0) 
    error("Error opening socket!");

  server = gethostbyname(argv[1]);
  if (server == NULL) 
    error("No such host(Host not found)!");
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr = *(in_addr*)server->h_addr;

  serv_addr.sin_port = htons(port_number);
  if (connect(sock_file_descriptor, (sockaddr*)&serv_addr, sizeof(serv_addr)) <0)
    error("Error connecting to host!");
  
  std::cout << "Please enter a message: ";
  std::getline(std::cin, send_buffer);
  send_buffer.shrink_to_fit();

  n = write(sock_file_descriptor, send_buffer.c_str(), sizeof(send_buffer));
  if (n < 0) 
    error("Error writing to socket!");
  
  n = read(sock_file_descriptor, recv_buffer.data(), sizeof(recv_buffer));

  if (n < 0)
    error("Error reading from socket!");
  
  recv_buffer.resize(n >= 0? n : 0);

  std::cout << "Received: " << recv_buffer << std::endl;
  close(sock_file_descriptor);
  return 0;
}