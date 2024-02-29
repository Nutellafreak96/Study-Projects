#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "my_fifo"

void readFromNamedPipe() {
    int pipefd;
    char buffer[256];
    ssize_t num_bytes_read;

    // Öffnen des FIFOs zum Lesen
    pipefd = open(FIFO_NAME, O_RDONLY);
    if (pipefd == -1) {
        perror("Fehler beim Öffnen des FIFOs zum Lesen");
        exit(1);
    }

    while ((num_bytes_read = read(pipefd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[num_bytes_read] = '\0';
        printf("%s", buffer);
    }

    // Schließen des FIFOs
    close(pipefd);

    exit(0);
}

int main() {
    // readFromNamedPipe aufrufen
    readFromNamedPipe();

    return 0;
}
