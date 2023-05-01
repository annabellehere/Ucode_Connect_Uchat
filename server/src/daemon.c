#include "../inc/uchat.h"

void mx_daemonize(void) {
    pid_t pid = 0;
    pid = fork();
    //on fork fail
    if (pid < 0) exit(1);
    //terminate parent
    if (pid > 0) exit(0);
    //child becomes session leader
    if (setsid() < 0) exit(1);

    //second fork to not be session leader
    pid = fork();
    //on fork fail
    if (pid < 0) exit(1);
    //terminate parent
    if (pid > 0) exit(0);

    printf("Uchat Server PID: %i\n", getpid());

    //close stdin/out/err
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

}
