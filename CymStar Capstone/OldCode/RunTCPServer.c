#include "TCPServer.c"
int* fd;
int main() {
    fd = createSocket();
    while(BMpull(fd[0])) {
        // loop until termination
    }
    terminateSocket(fd[1],fd[0]);
}
