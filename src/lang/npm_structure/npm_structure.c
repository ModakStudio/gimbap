#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "npm_structure.h"

// check if NPM is installed in the system
static int is_npm_installed(void) {
    int status = system("command -v npm > /dev/null 2>&1");
    return (status == 0) ? 1 : 0;
}

void setup_npm_project(GimbapConfig *config) {
    char original_dir[1024];

    // Get current working directory to restore it later
    if (getcwd(original_dir, sizeof(original_dir)) == NULL) {
        perror("Error: Failed to get current working directory");
        return;
    }

    // Pre-check NPM toolchain availability before touching disk
    if (!is_npm_installed()) {
        fprintf(stderr, "\n[Error] NPM toolchain was not found in your system PATH!\n");
        fprintf(stderr, "Please install Node.js/NPM and try again.\n\n");
        return;
    }

    printf("Creating NPM project directory: %s...\n", config->name);

    // Create root project directory
    if (mkdir(config->name, 0755) != 0) {
        perror("Error: Failed to create project root directory");
        return;
    }

    // Change execution context into project root directory
    if (chdir(config->name) != 0) {
        perror("Error: Failed to enter project directory");
        return;
    }

    // Initialize package.json automatically using 'npm init -y'
    printf("Initializing package.json using 'npm init -y'...\n");
    if (system("npm init -y > /dev/null 2>&1") != 0) {
        fprintf(stderr, "Error: 'npm init -y' failed. Rolling back working directory...\n");
        chdir(original_dir);
        return;
    }

    // Generate entry point file: index.js
    printf("Creating main entry point (index.js)...\n");
    FILE *fp = fopen("index.js", "w");
    if (fp != NULL) {
        fprintf(fp, "// Main entry point for %s\n", config->name);
        fprintf(fp, "console.log('Hello, World! Welcome to %s!');\n", config->name);
        fclose(fp);
    } else {
        perror("Warning: Failed to create index.js");
    }

    // Generate README.md if configured
    if (config->readme) {
        FILE *readme_fp = fopen("README.md", "w");
        if (readme_fp != NULL) {
            fprintf(readme_fp, "# %s\n\nThis NPM project was initialized by gimbap.\n\n", config->name);
            fclose(readme_fp);
            printf("Generated README.md in package root.\n");
        } else {
            perror("Warning: Failed to write README.md");
        }
    }

    // Restore original working directory context
    chdir(original_dir);
    printf("\nSuccessfully generated NPM project structure!\n");
}