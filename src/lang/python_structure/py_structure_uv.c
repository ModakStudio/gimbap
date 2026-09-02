#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "py_structure_uv.h"

#define PATH_MAX 4096

int is_valid_python_version(const char *version) {
    // Whitelist of supported Python versions
    const char *valid_versions[] = {
        "3.8", "3.9", "3.10", "3.11", "3.12", "3.13", "3.14"
    };
    
    // Automatically calculate the number of elements in the array
    int num_versions = sizeof(valid_versions) / sizeof(valid_versions[0]);

    for (int i = 0; i < num_versions; i++) {
        if (strcmp(version, valid_versions[i]) == 0) {
            return 1; // Valid version found!
        }
    }
    return 0; // No match found in the whitelist
}

void setup_python_uv_project(GimbapConfig *config) {
    char original_dir[PATH_MAX];

    // 1. Get current working directory to restore it later
    if (getcwd(original_dir, sizeof(original_dir)) == NULL) {
        perror("Error: Failed to get current directory");
        return;
    }

    printf("Creating Python uv project directory: %s...\n", config->name);

    // 2. Create and enter the project directory
    // Matches: $ mkdir myproject && cd myproject
    if (mkdir(config->name, 0755) != 0) {
        perror("Error: Failed to create project directory");
        return;
    }
    if (chdir(config->name) != 0) {
        perror("Error: Failed to enter project directory");
        return;
    }

    // 3. Initialize uv project
    // Generates: .python-version, README.md, main.py, pyproject.toml
    printf("Initializing uv project...\n");
    if (system("uv init") != 0) {
        fprintf(stderr, "Error: 'uv init' command failed. Please check if 'uv' is installed.\n");
        chdir(original_dir);
        return;
    }

    // 4. Pin Python version
    // Matches: $ uv python pin any version -> Creates .python-version
    // select Python version to pin, defaulting to 3.12 if no input is provided
    char selected_version[10];
    while(1){
        printf("\nEnter Python version to pin (e.g., 3.12, 3.14) [Default: 3.12]: ");
        fflush(stdout);

        if (fgets(selected_version, sizeof(selected_version), stdin) != NULL) {
            selected_version[strcspn(selected_version, "\r\n")] = '\0';
        }

        if (strlen(selected_version) == 0) {
            strcpy(selected_version, "3.12");
        }

        if (is_valid_python_version(selected_version)) {
            break;
        } else {
            printf("Invalid Python version. Please select from the following supported versions: 3.8, 3.9, 3.10, 3.11, 3.12, 3.13, 3.14\n");
        }
    }

    char pin_command[50];
    snprintf(pin_command, sizeof(pin_command), "uv python pin %s", selected_version);

    printf("Pinning Python version using 'uv python pin %s'...\n", selected_version);
    if (system(pin_command) != 0) {
        fprintf(stderr, "Warning: Failed to pin Python version. Default version will be used.\n");
    }

    // 5. Synchronize environment to generate .venv and uv.lock
    // Matches: $ uv sync -> Generates .venv/ and uv.lock
    printf("Syncing project to create .venv and uv.lock...\n");
    if (system("uv sync") != 0) {
        fprintf(stderr, "Warning: 'uv sync' failed. You may need to run it manually later.\n");
    }

    // 6. Handle README.md based on TUI configuration
    // If the user checked 'No README' in TUI, remove the generated README.md
    if (!config->readme) {
        remove("README.md");
        printf("Removed default README.md as requested by configuration.\n");
    }

    // 7. Restore original working directory
    chdir(original_dir);
    printf("\nSuccessfully generated Python uv project structure based on official guide!\n");
}
