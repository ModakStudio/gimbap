#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "rs_structure_cargo.h"

#define PATH_MAX 4096

//check if rustc and cargo are installed in the system
static int is_rust_environment_ready(void) {
    // 1. Check if the Rust compiler 'rustc' exists in the system
    int rustc_status = system("command -v rustc > /dev/null 2>&1");

    // 2. Check if the Cargo package manager 'cargo' exists in the system
    int cargo_status = system("command -v cargo > /dev/null 2>&1");


    if (rustc_status == 0 && cargo_status == 0) {
        return 1; // Both rustc and cargo are successfully installed
    }
    else if(rustc_status !=0 && cargo_status !=0){
        fprintf(stderr, "Error: Both 'rustc' and 'cargo' are missing. Please install Rust and Cargo.\n");
        return 0; // Both rustc and cargo are missing
    }
    else if(rustc_status !=0){
        fprintf(stderr, "Error: 'rustc' is missing. Please install the Rust compiler.\n");
        return 0; // rustc is missing
    }
    else if(cargo_status !=0){
        fprintf(stderr, "Error: 'cargo' is missing. Please install the Cargo package manager.\n");
        return 0; // cargo is missing
    }

    return 0;
}

void setup_rust_cargo_project(GimbapConfig *config) {
    char original_dir[PATH_MAX];

    // 1. Get current working directory to restore it later
    if (getcwd(original_dir, sizeof(original_dir)) == NULL) {
        perror("Error: Failed to get current directory");
        return;
    }

    // 2. Check if the Rust environment is ready (both rustc and cargo are installed)
    if (!is_rust_environment_ready()) {
        fprintf(stderr, "Error: Rust environment is not ready.\n");
        return;
    }

    printf("Creating Rust project directory: %s...\n", config->name);

    // 3. Create and enter the project directory
    if (mkdir(config->name, 0755) != 0) {
        perror("Error: Failed to create project directory");
        return;
    }
    if (chdir(config->name) != 0) {
        perror("Error: Failed to enter project directory");
        return;
    }

    // 4. Initialize cargo project based on user input (binary or library)
    char init_command[256];
    int project_type;

    while(1){
        printf("Select project type (0: Binary, 1: Library): ");
        if (scanf("%d", &project_type) != 1 || (project_type != 0 && project_type != 1)) {
            fprintf(stderr, "Invalid input. Please enter 0 for Binary or 1 for Library.\n");
            // Clear the input buffer
            while(getchar() != '\n');
        } else {
            break; // Valid input received
        }
    }

    if (project_type == 1) {
        snprintf(init_command, sizeof(init_command), "cargo init --lib");
    } else {
        snprintf(init_command, sizeof(init_command), "cargo init --bin");
    }

    printf("Initializing Rust project with 'cargo init'...\n");

    if (system(init_command) != 0) {
        fprintf(stderr, "Error: '%s' failed. Rollback initialized.\n", init_command);
        chdir(original_dir);
        return;
    }

    // 5. Create standard Cargo layout
    printf("Creating standard Cargo layout subdirectories (tests, examples)...\n");
    
    if (mkdir("tests", 0755) != 0) {
        perror("Warning: Failed to create 'tests' directory");
    }
    if (mkdir("examples", 0755) != 0) {
        perror("Warning: Failed to create 'examples' directory");
    }

    // 6. Create README.md if requested by the Gimbap configuration
    if (config->readme) {
        FILE *fp = fopen("README.md", "w");
        if (fp != NULL) {
            fprintf(fp, "# %s\n\nThis project was initialized by gimbap in Rust.\n", config->name);
            fclose(fp);
            printf("Generated README.md for the Rust project.\n");
        } else {
            perror("Warning: Failed to write README.md");
        }
    }

    // 7. Restore the original directory context so subsequent program flows run correctly
    chdir(original_dir);
    printf("\nSuccessfully generated Rust project structure!\n");
}