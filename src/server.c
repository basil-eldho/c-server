#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000
#define BUFFER_SIZE 4096

struct header {
    char *name;
    char *value;
};

struct request {
    char *method;
    char *path;
    int protocol;
    int nheaders;
    struct header headers[32];
};

int parse_request(char *buf, int n, struct request *req) {
    req->method = buf;

    char *sp = memchr(buf, ' ', n);
    if (sp == NULL)
        return -1;
    *sp = '\0';
    req->path = sp + 1;

    sp = memchr(req->path, ' ', n - (req->path - buf));
    if (sp == NULL)
        return -1;
    *sp = '\0';

    char *ver = sp + 1;

    if (ver[5] == '1' && ver[7] == '0')
        req->protocol = 10;
    else if (ver[5] == '1' && ver[7] == '1')
        req->protocol = 11;
    else
        return -1; // unsupported

    return 0;
}

int send_response(int fd, int status_code, char *body) {
    char buf[BUFFER_SIZE];
    int len = snprintf(buf, sizeof(buf),
                       "HTTP/1.1 %d OK\r\n"
                       "Content-Length: %d\r\n"
                       "\r\n"
                       "%s",
                       status_code, (int) strlen(body), body);

    if (write(fd, buf, len) == -1) {
        perror("send response: write");
        return 1;
    }
    return 0;
}

int main() {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server_fd, (struct sockaddr *) &addr, addrlen) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 10) == -1) {
        perror("listen");
        return 1;
    }
    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr *) &addr, &addrlen);
        if (client_fd == -1) {
            perror("accept");
            return 1;
        }

        printf("client: %s:%d\n\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        fflush(stdout);

        char buf[BUFFER_SIZE];
        int n = read(client_fd, buf, sizeof(buf));
        if (n == -1) {
            perror("read");
            return 1;
        }

        struct request req;
        if (parse_request(buf, n, &req) == -1) {
            fprintf(stderr, "parse request\n");
            close(client_fd);
            continue;
        }

        printf("parsed request: %s, %s, %d\n", req.method, req.path, req.protocol);

        send_response(client_fd, 200, "Hello world\n");

        close(client_fd);
    }
    close(server_fd);

    return 0;
}
