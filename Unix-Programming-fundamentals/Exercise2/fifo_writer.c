#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "my_fifo"

void writeToNamedPipe(int numEntries, int multipleReadersWriters) {
    int pipefd;
    char message[256];
    int i;

    // Erstellung des benannten Pipe (FIFO)
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("Fehler beim Erstellen des FIFOs");
        exit(1);
    }

    // Öffnen des FIFOs zum Schreiben
    pipefd = open(FIFO_NAME, O_WRONLY);
    if (pipefd == -1) {
        perror("Fehler beim Öffnen des FIFOs zum Schreiben");
        exit(1);
    }

    pid_t pid = getpid();

    for (i = 1; i <= numEntries; i++) {
        snprintf(message, sizeof(message), "Writer-PID: %d -> record-nr: %6d\n", pid, i);

        // Schreiben der Daten in den FIFO
        if (write(pipefd, message, strlen(message)) == -1) {
            perror("Fehler beim Schreiben in den FIFO");
            exit(1);
        }

        sleep(1);
    }

    // Schließen des FIFOs
    close(pipefd);

    // FIFO löschen
    if (unlink(FIFO_NAME) == -1) {
        perror("Fehler beim Löschen des FIFOs");
        exit(1);
    }

    exit(0);
}

int main(int argc, char* argv[]) {
    int numEntries = 10;
    int multipleReadersWriters = 0;

    // Argumente überprüfen und Werte setzen
    if (argc > 1) {
        numEntries = atoi(argv[1]);
    }

    if (argc > 2 && strcmp(argv[2], "-m") == 0) {
        multipleReadersWriters = 1;
    }

    // writeToNamedPipe aufrufen
    writeToNamedPipe(numEntries, multipleReadersWriters);

    return 0;
}
