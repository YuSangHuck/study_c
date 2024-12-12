#include <stdio.h>
#include <stdlib.h>

int
main() {
    char *command = "/home/sanghuck-yu/_/study_c/cwd/another/path/to/a.sh";
    printf("command: %s\n", command);
    int ret = system(command);
    if (ret == -1) {
        perror("Failed to execute script with system");
        return -1;
    }

    return 0;
}