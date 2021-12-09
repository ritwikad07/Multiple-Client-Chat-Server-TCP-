#include <ncurses.h>
#include <unistd.h>  /* only for sleep() */
//#include <stdio.h>
int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

/*int run(void)
{
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);
        if (kbhit()) {
            printw("Key pressed! It was: %d\n", getch());
            endwin();
            return 1;
            refresh();

        } else {
            //printw("No key pressed yet...\n");
            refresh();
            endwin();
            return 0;
            //sleep(1);
        }

}*/
