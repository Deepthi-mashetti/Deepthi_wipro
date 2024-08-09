#include <iostream>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

std::map<std::string, float> stockPrices = {
    {"AAPL", 150.0},
    {"GOOG", 2500.0},
    {"MSFT", 200.0}
};

std::string handleRequest(const std::string& request) {
    if (request == "GET_STOCK_PRICES") {
        std::string response;
        for (const auto& [symbol, price] : stockPrices) {
            response += symbol + ": $" + std::to_string(price) + "\n";
        }
        return response;
    } else if (request.substr(0, 4) == "BUY ") {
        std::string symbol = request.substr(4);
        if (stockPrices.find(symbol) != stockPrices.end()) {
            // In a real application, you would handle inventory and purchasing logic
            return "Order placed to buy " + symbol;
        } else {
            return "Stock symbol not found.";
        }
    } else if (request.substr(0, 5) == "SELL ") {
        std::string symbol = request.substr(5);
        if (stockPrices.find(symbol) != stockPrices.end()) {
            // In a real application, you would handle inventory and selling logic
            return "Order placed to sell " + symbol;
        } else {
            return "Stock symbol not found.";
        }
    } else {
        return "Invalid command.";
    }
}

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

    // Bind socket to address and port
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    while (true) {
        // Accept incoming connection
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sockfd < 0) {
            perror("accept failed");
            continue;
        }

        std::cout << "Connected to client" << std::endl;

        // Receive request from client
        char buffer[1024] = {0};
        int bytesReceived = recv(client_sockfd, buffer, 1024, 0);
        if (bytesReceived < 0) {
            perror("recv failed");
            close(client_sockfd);
            continue;
        }

        std::string request(buffer, bytesReceived);
        std::cout << "Received request: " << request << std::endl;

        // Handle request and send response to client
        std::string response = handleRequest(request);
        send(client_sockfd, response.c_str(), response.size(), 0);

        // Close socket
        close(client_sockfd);
    }

    close(sockfd);
    return 0;
}





