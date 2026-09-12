#ifndef TUI_H
#define TUI_H

#include <ncurses.h>
#include "config.h"

// run the TUI to gather project initialization settings
void tui_run(GimbapConfig *config);

// print logo to the window
void draw_logo(WINDOW *win);
// step1: get the project name
void get_name(WINDOW *win, GimbapConfig *config);
// step2: select the programming language
void select_lang(WINDOW *win, GimbapConfig *config);
// step3: set options for the selected language
void set_options(WINDOW *win, GimbapConfig *config);
// step4: select the license
void set_license(WINDOW *win, GimbapConfig *config);
// step5: set readme.md template generation option
void set_readme(WINDOW *win, GimbapConfig *config);
// print the summary of the gathered settings
void print_summary(GimbapConfig *config);

#endif