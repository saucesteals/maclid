#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#ifndef TIOCLINUX
#define TIOCLINUX 0x541C
#endif

#define TIOCL_BLANKSCREEN 14
#define TIOCL_UNBLANKSCREEN 4

void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s blank|unblank|sync /dev/ttyX\n", progname);
}

int get_lid_state() {
    FILE *fp = fopen("/proc/acpi/button/lid/LID0/state", "r");
    if (!fp) {
        perror("fopen /proc/acpi/button/lid/LID0/state");
        return -1;
    }

    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        char state[16];
        if (sscanf(line, "state: %15s", state) == 1) {
            fclose(fp);
            if (strcmp(state, "closed") == 0) {
                return TIOCL_BLANKSCREEN;
            } else if (strcmp(state, "open") == 0) {
                return TIOCL_UNBLANKSCREEN;
            }
        }
    }

    fclose(fp);
    fprintf(stderr, "Could not determine LID state\n");
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *command = argv[1];
    const char *tty_path = argv[2];
    char arg;

    if (strcmp(command, "blank") == 0) {
        arg = TIOCL_BLANKSCREEN;
    } else if (strcmp(command, "unblank") == 0) {
        arg = TIOCL_UNBLANKSCREEN;
    } else if (strcmp(command, "sync") == 0) {
        int state = get_lid_state();
        if (state == -1) return EXIT_FAILURE;
        arg = (char)state;
    } else {
        fprintf(stderr, "Invalid command: %s\n", command);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    int fd = open(tty_path, O_RDWR);
    if (fd < 0) {
        perror(tty_path);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, TIOCLINUX, &arg) < 0) {
        perror("ioctl");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}
