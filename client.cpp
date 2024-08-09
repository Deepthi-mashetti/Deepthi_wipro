#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    // Set address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect failed");
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    while (true) {
        // Enter command
        std::string command;
        std::cout << "Enter command (GET_STOCK_PRICES/BUY <symbol>/SELL <symbol>): ";
        std::getline(std::cin, command);

        // Send request to server
        send(sockfd, command.c_str(), command.size(), 0);

        // Receive response from server
        char buffer[1024] = {0};
        int bytesReceived = recv(sockfd, buffer, 1024, 0);
        if (bytesReceived < 0) {
            perror("recv failed");
            return 1;
        }

        // Print received response
        std::string response(buffer, bytesReceived);
        std::cout << "Received response: " << response << std::endl;
    }

    // Close socket
    close(sockfd);
    return 0;
}






