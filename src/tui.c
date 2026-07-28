#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

typedef struct {
    char name[50];
    int lang;  // 1: Rust, 2: Go, 3: Python, 4: NPM
    bool remote; //github remote t/f
    int license; // 0: None, 1: MIT, 2: Apache, 3: GPL
    bool readme; // readme.md t/f
}GimbapConfig;

int width = 60;

void draw_logo(WINDOW *win);
void get_name(WINDOW *win, GimbapConfig *config);
void select_lang(WINDOW *win, GimbapConfig *config);
void set_options(WINDOW *win, GimbapConfig *config);
void set_license(WINDOW *win, GimbapConfig *config);
void set_readme(WINDOW *win, GimbapConfig *config);

int main() {
    setlocale(LC_ALL, ""); 
    GimbapConfig config = {"", 0, false, 0, true};

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int start_y = (LINES - 15) / 2;
    int start_x = (COLS - 60) / 2;
    WINDOW *win = newwin(15, 60, start_y, start_x);
    keypad(win, TRUE);

    draw_logo(win);
    get_name(win, &config);
    select_lang(win, &config);
    set_options(win, &config);
    set_license(win, &config);
    set_readme(win, &config);

    endwin();

    printf("=== Gimbap Configuration Result ===\n");
    printf("Project Name : %s\n", config.name);
    printf("Language ID  : %d\n", config.lang);
    printf("Git Remote   : %s\n", config.remote ? "Yes" : "No");
    printf("License ID   : %d\n", config.license);
    printf("Add README   : %s\n", config.readme ? "Yes" : "No");
    printf("==================================\n");

    return 0;
}

void draw_logo(WINDOW *win){
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 2, 11, "████   ██   █   █  ████▄   ▄██▄   ████▄");
    mvwprintw(win, 3, 11, "█▄      ██   █▀▄▀█  █   █  █▄▄▄█  █   █");
    mvwprintw(win, 4, 11, "█▀ ██   ██   █ █ █  ████▀  █▀▀▀█  ████▀");
    mvwprintw(win, 5, 11, "█▄▄▄█  ▄██▄  █   █  █▄▄▄█  █   █  █");
    mvwprintw(win, 7, 15, "- INTEGRATED PROJECT MANAGER -");
    mvwprintw(win, 8, 18, "v1.0.0 (Debian Package)");
    mvwhline(win, 10, 1, ACS_HLINE, width - 2);
    mvwprintw(win, 12, 16, "[ Press any key to start ]");
    wrefresh(win);
    wgetch(win);
}

//step1: enter the project name
void get_name(WINDOW *win, GimbapConfig *config) {
    if (strlen(config->name) > 0) return; // if name is already set, skip input


    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 20, " [ Project Name ] ");
    mvwprintw(win, 3, 4, "enter the project name:");
    mvwprintw(win, 5, 4, "Name: [ %-30s ]", "");
    mvwprintw(win, 8, 4, "(if not entered, current directory name will be used)");
    mvwhline(win, 11, 1, ACS_HLINE, width - 2);
    mvwprintw(win, 12, 2, "[Enter] Next");
    wmove(win, 5, 12);
    echo();
    wgetnstr(win, config->name, 30);
    noecho();
}

//step2: select the programming language
void select_lang(WINDOW *win, GimbapConfig *config) {
    if (config->lang != 0) return; // if language is already selected, skip input

    int choice = 0;
    char *langs[] = {"Rust", "Go", "Python", "NPM"};
    int n_langs = sizeof(langs) / sizeof(char *);
    int ch;

    keypad(win, TRUE);

    while (1) {
        wclear(win);
        box(win, 0, 0);

        mvwprintw(win, 0, (width - 21) / 2, " [ Select Language ] ");
        mvwprintw(win, 2, 4, "Select the programming language/environment:");

        for (int i = 0; i < n_langs; i++) {
            if (i == choice) {
                wattron(win, A_REVERSE);
                mvwprintw(win, 4 + i, 4, " > [ ] %-10s ", langs[i]);
                wattroff(win, A_REVERSE);
            } else {
                mvwprintw(win, 4 + i, 4, "   [ ] %-10s ", langs[i]);
            }
        }

        mvwhline(win, 11, 1, ACS_HLINE, width - 2);
        mvwprintw(win, 12, 2, " [↑/↓] Move   [Enter] Next   [q] Quit ");
        wrefresh(win);

        ch = wgetch(win);
        switch (ch) {
            case KEY_UP:
                choice = (choice - 1 + n_langs) % n_langs;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_langs;
                break;
            case 10: // ENTER (ASCII 10)
                config->lang = choice + 1; // (1: Rust, 2: Go...)
                return;
            case 'q':
                return;
        }
    }
}

//step3: remote repository setting
void set_options(WINDOW *win, GimbapConfig *config) {
    if (config->remote) return;

    int choice = 0; // 0: No, 1: Yes
    int ch;

    keypad(win, TRUE);

    while (1) {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 19, " [ GitHub Remote ] ");
        if (choice == 0) {
                wattron(win, A_REVERSE);
                mvwprintw(win, 4, 6, " (*) No, Local only ");
                wattroff(win, A_REVERSE);
                mvwprintw(win, 5, 6, " ( ) Yes, Create or Connect Remote Repository ");
            } else {
                mvwprintw(win, 4, 6, " ( ) No, Local only ");
                wattron(win, A_REVERSE);
                mvwprintw(win, 5, 6, " (*) Yes, Create or Connect Remote Repository ");
                wattroff(win, A_REVERSE);
            }
            mvwhline(win, 11, 1, ACS_HLINE, width - 2);
            mvwprintw(win, 12, 2, " [↑/↓] Move   [Enter] Next   [q] Quit ");
            wrefresh(win);

        ch = wgetch(win);
            switch (ch) {
                case KEY_UP:
                case KEY_DOWN:
                    choice = !choice;
                    break;
                case 10:
                    config->remote = (choice == 1);
                    return;
                case 'q':
                    return;
        }
    }
}

//step4: select the license
void set_license(WINDOW *win, GimbapConfig *config) {
    if (config->license != 0) return; // if license is already selected, skip input
    char *licenses[] = {
        "None",
        "MIT License",
        "Apache License 2.0",
        "GPL v3"
    };
    int n_licenses = sizeof(licenses) / sizeof(char *);
    int choice = 0;
    int ch;

    keypad(win, TRUE); 
    while (1) {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, (width - 13) / 2, " [ License ] ");
        mvwprintw(win, 2, 4, "select a license for your project:");

        for (int i = 0; i < n_licenses; i++) {
            if (i == choice) {
                wattron(win, A_REVERSE);
                mvwprintw(win, 4 + i, 4, " > [ ] %-20s ", licenses[i]);
                wattroff(win, A_REVERSE);
            } else {
                mvwprintw(win, 4 + i, 4, "   [ ] %-20s ", licenses[i]);
            }
        }
        mvwhline(win, 11, 1, ACS_HLINE, width - 2);
        mvwprintw(win, 12, 2, " [↑/↓] Move   [Enter] Next   [q] Quit ");
        wrefresh(win);

        ch = wgetch(win);
        switch (ch) {
            case KEY_UP:
                choice = (choice - 1 + n_licenses) % n_licenses;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_licenses;
                break;
            case 10: // ENTER 키
                config->license = choice + 1; // (1: None, 2: MIT, ...)
                return;
            case 'q':
                return;
        }
    }
}

//step5: readme.md setting
void set_readme(WINDOW *win, GimbapConfig *config) {
    if(config->readme) return; // if readme is already set, skip input

    int choice = 0; // 0: Yes (Create template), 1: No
    int ch;

    keypad(win, TRUE);

    while (1) {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, (width - 13) / 2, " [ README.md ] ");
        mvwprintw(win, 2, 4, "Do you want to create a README.md template for your project?");

        if (choice == 0) {
            wattron(win, A_REVERSE);
            mvwprintw(win, 4, 6, " > [X] Yes, create a template ");
            wattroff(win, A_REVERSE);
            mvwprintw(win, 5, 6, "   [ ] No ");
        } else {
            mvwprintw(win, 4, 6, "   [X] Yes, create a template ");
            wattron(win, A_REVERSE);
            mvwprintw(win, 5, 6, " > [ ] No ");
            wattroff(win, A_REVERSE);
        }

        mvwprintw(win, 7, 6, "* A basic template including project description and usage instructions will be created.");
        mvwhline(win, 11, 1, ACS_HLINE, width - 2);
        mvwprintw(win, 12, 2, " [↑/↓] Move   [Enter] Finish   [q] Quit ");
        wrefresh(win);

        ch = wgetch(win);
        switch (ch) {
            case KEY_UP:
            case KEY_DOWN:
            case ' ':
                choice = !choice;
                break;
            case 10:
                config->readme = (choice == 0); 
                return;
            case 'q':
                return;
        }
    }
}