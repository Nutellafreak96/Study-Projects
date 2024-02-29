#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void lsgrep(const char* directory, const char* searchString, const char* outputFile) {
    int pipefd[2];
    pid_t pid1, pid2, pid3;

    // Erste Pipe erstellen
    if (pipe(pipefd) == -1) {
        perror("Fehler beim Erstellen der Pipe");
        exit(1);
    }

    // Erster Kindprozess: ls -lR [directory]
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fehler beim Erstellen des Kindprozesses");
        exit(1);
    } else if (pid1 == 0) {
        // Kindprozess

        // Verbindung der Standardausgabe mit dem Schreibende der ersten Pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Ausführen von "ls -lR [directory]"
        execlp("ls", "ls", "-lR", directory, NULL);
        perror("Fehler bei der Ausführung von ls");
        exit(1);
    }

    // Zweite Pipe erstellen
    if (pipe(pipefd) == -1) {
        perror("Fehler beim Erstellen der Pipe");
        exit(1);
    }

    // Zweiter Kindprozess: sort
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fehler beim Erstellen des Kindprozesses");
        exit(1);
    } else if (pid2 == 0) {
        // Kindprozess

        // Verbindung der Eingabe mit dem Leseende der ersten Pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Verbindung der Standardausgabe mit dem Schreibende der zweiten Pipe (STDOUT_FILENO bleibt unverändert)
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Ausführen von "sort"
        execlp("sort", "sort", NULL);
        perror("Fehler bei der Ausführung von sort");
        exit(1);
    }

    // Dritte Pipe erstellen
    if (pipe(pipefd) == -1) {
        perror("Fehler beim Erstellen der Pipe");
        exit(1);
    }

    // Dritter Kindprozess: grep [searchString]
    pid3 = fork();
    if (pid3 < 0) {
        perror("Fehler beim Erstellen des Kindprozesses");
        exit(1);
    } else if (pid3 == 0) {
        // Kindprozess

        // Verbindung der Eingabe mit dem Leseende der zweiten Pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Ausführen von "grep [searchString]"
        execlp("grep", "grep", searchString, NULL);
        perror("Fehler bei der Ausführung von grep");
        exit(1);
    }

    // Verbindung der Ausgabe mit dem Schreibende der dritten Pipe (STDOUT_FILENO bleibt unverändert)
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);

    // Warten auf die Terminierung des dritten Kindprozesses (grep)
    waitpid(pid3, NULL, 0);

    // Falls ein outputFile angegeben wurde, die Ausgabe umleiten
    if (outputFile != NULL) {
        FILE* outfile = fopen(outputFile, "w");
        if (outfile == NULL) {
            perror("Fehler beim Öffnen der Ausgabedatei");
            exit(1);
        }

        // Verbindung der Standardausgabe mit der Datei
        dup2(fileno(outfile), STDOUT_FILENO);
        fclose(outfile);
    }

    // Ausführen von "ls -lR [directory]"
    execlp("ls", "ls", "-lR", directory, NULL);
    perror("Fehler bei der Ausführung von ls");
    exit(1);
}

int main(int argc, char* argv[]) {
    const char* directory = ".";
    const char* searchString = "";
    const char* outputFile = NULL;

    // Argumente überprüfen und Werte setzen
    if (argc > 1) {
        directory = argv[1];
    }
    if (argc > 2) {
        searchString = argv[2];
    }
    if (argc > 3) {
        outputFile = argv[3];
    }

    // lsgrep aufrufen
    lsgrep(directory, searchString, outputFile);

    return 0;
}
