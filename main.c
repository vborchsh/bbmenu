
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "BBmenu.h"

int main()
{
    char *verb;
    int4 iverb;
    char theString[1024];
    //uint2 minor,major,build;
    //uint4 err_code;

    printf("\nExample for BBmenu\n");
    printf("\nPowered by:\n");
    menu_init();

    //allow simulate_input()
    AllowInput=1;

    // Load menu
    //define_file("./../digitizerqusbctrl/QUSB_HPTDC.uic");

    // Main cycle
    while (1) {
        verb = action_menu("main_menu", &iverb);

        if (strcmp(verb, "INIT") == 0) {
//            simulate_input("@./../digitizerqusbctrl/hptdc_init.uic");
        }

        if (strcmp(verb, "HPTDC") == 0) {
//            HPTDC_menu();
        }

        if (strcmp(verb, "QUSB") == 0) {
            //QuickUSB_menu();
        }

        if (strcmp(verb, "MESSAGE") == 0) {
            get_text("Enter message", theString);
            puts(theString);
        }

        if (strcmp(verb, "SLEEP") == 0) {
            long usec = 0;
            get_integer( "Enter uSeconds # to be sleep:", &usec, 0, 0);
            usleep(usec);
        }

        if (strcmp(verb, "EXIT") == 0) {
            puts("Closing device...\n");
            puts("Done!\n");
            break;
        }
    }
    //
    return 1;
}



