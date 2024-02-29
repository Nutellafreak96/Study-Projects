#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 100

pid_t child_pid;

void printProcessStatus() {
    printf("Process Status:\n");
    fflush(stdout);
    system("ps -lef");
}

void printProcessIDs() {
    printf("Parent PID: %d\n", getpid());
    printf("Child PID: %d\n", child_pid);
}

void sendSignal(int signal) {
    kill(child_pid, signal);
}

void handleSignal(int signal) {
    if (signal == SIGUSR1) {
        printf("Received SIGUSR1 from Child.\n");
    } else if (signal == SIGUSR2) {
        printf("Received SIGUSR2 from Child. Terminating...\n");
        exit(0);
    }
}

void waitForChildExit() {
    int status;
    pid_t pid = waitpid(child_pid, &status, 0);
    if (pid == -1) {
        perror("Error waiting for child process");
        exit(1);
    }
    if (WIFEXITED(status)) {
        printf("Child process exited with status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child process terminated by signal: %d\n", WTERMSIG(status));
    }
    exit(0);
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    child_pid = fork();
    if (child_pid < 0) {
        perror("Error forking child process");
        exit(1);
    } else if (child_pid == 0) {
        // Child process
        execl("./loop", "loop", NULL);
        perror("Error executing loop");
        exit(1);
    }

    signal(SIGUSR1, handleSignal);
    signal(SIGUSR2, handleSignal);

    while (1) {
        printf("Enter a command ('pstat', 'whoare', 'send <sig>', 'pong', 'quit'): ");
        fflush(stdout);
        fgets(command, MAX_COMMAND_LENGTH, stdin);

        // Remove trailing newline character
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "pstat") == 0) {
            printProcessStatus();
        } else if (strcmp(command, "whoare") == 0) {
            printProcessIDs();
        } else if (strncmp(command, "send ", 5) == 0) {
            int signal = atoi(command + 5);
            sendSignal(signal);
        } else if (strcmp(command, "pong") == 0) {
            sendSignal(SIGUSR1);
            pause();
        } else if (strcmp(command, "quit") == 0) {
            sendSignal(SIGUSR2);
            waitForChildExit();
        } else {
            printf("Unknown command\n");
        }
    }

    return 0;
}
