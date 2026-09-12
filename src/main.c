#include <stdbool.h>
#include "config.h"
#include "tui.h"
#include "create_project.h"

int main() {
    GimbapConfig config = {"", 0, false, 0, false};

    tui_run(&config);
    create_project(&config);

    return 0;
}