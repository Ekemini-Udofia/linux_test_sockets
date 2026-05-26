#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

inline static void error(const std::string& msg) {
  std::cerr << msg << ": " << std::strerror(errno) << std::endl;
  std::exit(1);
}

int main(int argc, char *argv[]) {
  int sock_file_descriptor, newsock_file_descriptor, port_number;
  socklen_t client_length;
  char buffer[256];

  sockaddr_in serv_addr{}, cli_addr{};
  int n;
  if (argc < 2) {
    std::cerr << "ERROR: no port provided" << std::endl;
    std::exit(1);
  }
  sock_file_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (sock_file_descriptor < 0)
    error("Error opening socket!");
  
  // bzero((char*) &serv_addr, sizeof(serv_addr));  /* This is useless now because initializing serv_addr{} with the {} sets all the memory to zero */

  port_number = std::stoi(argv[1]); // This should be put between a try and catch statement to prevent errors

  if (port_number < 0 || port_number > 65535)
    error("Error: Port out of range!");

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_number);
}

