#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5003

void handle_request(int client) {
    char buffer[1024];
    char response[2048];
    time_t now;
    char time_str[100];
    
    // Read request
    read(client, buffer, sizeof(buffer)-1);
    
    // Get current time
    time(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    // Create response
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n"
        "{\n"
        "  \"status\": \"OK\",\n"
        "  \"server\": \"C HTTP Server\",\n"
        "  \"timestamp\": \"%s\",\n"
        "  \"message\": \"C server is working! 🚀\",\n"
        "  \"port\": %d\n"
        "}\n",
        time_str, PORT
    );
    
    // Send response
    write(client, response, strlen(response));
    close(client);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Allow reuse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Configure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    // Listen
    listen(server_fd, 3);
    
    printf("✅ C Server running on port %d\n", PORT);
    printf("📍 Test: curl http://localhost:%d/api/c/health\n", PORT);
    
    // Main loop
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        if (fork() == 0) {
            close(server_fd);
            handle_request(client_fd);
            exit(0);
        }
        close(client_fd);
    }
    
    return 0;
}
