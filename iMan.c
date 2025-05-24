#include "headers.h"

void extract_information(const char *html) {
    const char *start, *end;

    // find "NAME" section
    start = strstr(html, "NAME\n");
    if (start) {
        // find the "SEE ALSO" section after "NAME"
        end = strstr(start, "SEE ALSO");
        if (end) {
            printf("\n");
            fwrite(start, 1, end - start, stdout);
        }
    }
}

void fetch_man_page(const char *command) {
    int sockfd;
    struct addrinfo hints, *res;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_received;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("man.he.net", "80", &hints, &res) != 0) {
        perror("getaddrinfo failed");
        return;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket failed");
        freeaddrinfo(res);
        return;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect failed");
        close(sockfd);
        freeaddrinfo(res);
        return;
    }

    freeaddrinfo(res);

    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n\r\n",
             command);

    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send failed");
        close(sockfd);
        return;
    }

    int header_ended = 0;
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';

        printf("Received:\n%s\n", response);
        // extract_information(response);
    }

    if (bytes_received < 0) {
        perror("recv failed");
    }

    // Close the socket
    close(sockfd);
}

int iMan(int argc, char* argv[]) {
    fetch_man_page(argv[1]);
    return 0;
}
