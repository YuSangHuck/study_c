#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void
print_usage(const char *prog_name) {
    printf("Usage: %s <gid> <uid> <script_path>\n", prog_name);
}

// Function to fork and execute a script
int
fork_and_execute(const char *script_path) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed to fork");
        return -1;
    }
    else if (pid == 0) {
        // Child process: Execute the script
        execl("/bin/sh", "/bin/sh", script_path, (char *)NULL);
        perror("Failed to execute script");
        exit(1);
    }

    // Parent process: Continue without waiting for the child
    printf("Script is running asynchronously with PID %d\n", pid);
    return 0;
}

int
main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    // Parse GID, UID, and script path from arguments
    gid_t gid = (gid_t)atoi(argv[1]);
    uid_t uid = (uid_t)atoi(argv[2]);
    const char *script_path = argv[3];

    // Execute the script as the current process (default GID/UID)
    if (fork_and_execute(script_path) != 0)
        return 1;

    // Change GID
    if (setgid(gid) != 0) {
        perror("Failed to set GID");
        return 1;
    }

    // Change UID
    if (setuid(uid) != 0) {
        perror("Failed to set UID");
        return 1;
    }

    // Execute the script with new GID/UID
    if (fork_and_execute(script_path) != 0)
        return 1;

    // Allow the parent process to continue while children run
    sleep(10);
    return 0;
}