#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000
#define BUFFER_SIZE 4096

int main() {
    int server_fd;
    int client_fd;

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *) &addr, addrlen) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 10) == -1) {
        perror("listen");
        return 1;
    }

    client_fd = accept(server_fd, (struct sockaddr *) &addr, &addrlen);
    if (client_fd == -1) {
        perror("accept");
        return 1;
    }

    printf("Connection established!!!\n");

    char buf[BUFFER_SIZE];
    int n = read(client_fd, buf, sizeof(buf));
    if (n == -1) {
        perror("read");
        return 1;
    }

    printf("received: %.*s\n", n, buf);
    fflush(stdout);

    if (write(client_fd, buf, n) == -1) {
        perror("write");
        return 1;
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
