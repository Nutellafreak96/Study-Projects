#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <setjmp.h>

#define TIMEOUT_SECONDS 60
#define INTERRUPT_SECONDS 10

jmp_buf jump_buffer;

void handleSignal(int signal) {
    if (signal == SIGUSR1) {
        printf("Received SIGUSR1 from Parent. PID: %d, PPID: %d\n", getpid(), getppid());
        fflush(stdout);
        kill(getppid(), SIGUSR1);
    }
}

void handleTimeout() {
    printf("No interaction from Parent. Generating a prompt for interaction.\n");
    fflush(stdout);
    longjmp(jump_buffer, 1);
}

int main() {
    signal(SIGUSR1, handleSignal);
    signal(SIGALRM, handleTimeout);

    if (setjmp(jump_buffer) != 0) {
        // Prompt for interaction after timeout
        alarm(INTERRUPT_SECONDS);
        pause();
    }

    alarm(TIMEOUT_SECONDS);

    while (1) {
        // Do some work
        sleep(1);
    }

    return 0;
}
