#include <stdbool.h>
#include "config.h"
#include "tui.h"

int main() {
    GimbapConfig config = {"", 0, false, 0, false};

    tui_run(&config);

    return 0;
}