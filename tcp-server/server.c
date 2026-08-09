#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

static void die(const char* m) {
    perror(m);
    exit(1);
}

int main(void) {
    int s=socket(AF_INET,SOCK_STREAM,0);
    if (s < 0) die("socket");

    int opt = 1;
    if (setsockopt
    (s, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) < 0) {
        die("setsocket");
    }


    struct sockaddr_in a = { 0 };
    a.sin_family = AF_INET;
    a.sin_addr.s_addr=htonl(INADDR_ANY);
    a.sin_port = htons(8080);

    if (
        bind(s, (struct sockaddr*)
            &a, sizeof(a)) < 0) {
        die("bind");
    }
    if (listen(s, 8) < 0) die("listen");

    const char* msg = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Hello, from server!\n";

    for (;;) {
        int c = accept(s, NULL, NULL);
        if (c < 0) continue;
        send(c, msg, strlen(msg), 0);
        close(c);
    }

    close(s);
    return 0;

}