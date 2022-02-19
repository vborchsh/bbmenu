
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "./../src/BBmenu.h"

#include "cmd_list_menu.h"

int main()
{
    char *verb;
    int4 iverb;
    char theString[1024];

    printf("\nExample for BBmenu\n");
    printf("\nPowered by:\n");
    menu_init();

    //allow simulate_input()
    AllowInput=1;

    // Load menu
    define_file("menu.uic");

    // Main cycle
    while (1) {
        verb = action_menu("main_menu", &iverb);

        if (strcmp(verb, "COMMANDS") == 0) {
            cmd_list_menu();
        }

        if (strcmp(verb, "ECHO") == 0) {
            get_text("Enter message", theString);
            puts(theString);
        }

        if (strcmp(verb, "SLEEP") == 0) {
            long usec = 0;
            get_integer( "Enter uSeconds # to be sleep:", &usec, 0, 0);
            usleep(usec);
        }

        if (strcmp(verb, "EXIT") == 0) {
            puts("Exiting...\n");
            break;
        }
    }

    return 1;
}



