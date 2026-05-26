#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
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
  char buffer[256] = {};

  sockaddr_in serv_addr{}, cli_addr{};

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
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port_number);

  if (bind(sock_file_descriptor, (sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    error("Error on binding!");
  
  listen(sock_file_descriptor, 5); // 5 is the number of socket connections that will be accepted

  client_length = sizeof(cli_addr);

  newsock_file_descriptor = accept(sock_file_descriptor, (sockaddr*) &cli_addr, &client_length);

  if (newsock_file_descriptor < 0)
    error("Error on accept!");

  size_t n = read(newsock_file_descriptor, buffer, sizeof(buffer) - 1);
  // n < 0 = read failed
  // n == 0 = client disconnected
  // Else read successful 
  if (n < 0) {
    std::cerr << "Read failed!" << std::endl;
  } else if (n == 0) {
    error("Client Disconnected");
  } else { // success 
    std::string client_message(buffer, n);
    std::cout << "Recieved " << n << " bytes: " << client_message << std::endl;
  }

  const char* message = "Message Received";
  n = write(newsock_file_descriptor, message, sizeof(message));

  if (n < 0) {
    error("Error writing to socket!");
  } else {
    std::cout << "Message sent successfully!" << std::endl;
  }

  close(newsock_file_descriptor);
  close(sock_file_descriptor);

  return 0;
}

