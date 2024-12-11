#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void
print_usage(const char *prog_name) {
    printf("Usage: %s <gid> <uid> <script_path>\n", prog_name);
}

// Function to execute a script using system()
int
system_execute(const char *script_path) {
    char command[256];

    // Construct the command to run the script
    snprintf(command, sizeof(command), "/bin/sh %s", script_path);

    // Execute the command
    int ret = system(command);
    if (ret == -1) {
        perror("Failed to execute script with system");
        return -1;
    }

    printf("Script executed with system, return code: %d\n", ret);
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
    if (system_execute(script_path) != 0)
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
    if (system_execute(script_path) != 0)
        return 1;

    // Allow the parent process to continue while scripts run
    sleep(10);
    return 0;
}
