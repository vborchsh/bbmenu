
#include <cmd_list_menu.h>

void cmd_list_menu(void)
{
    char *verb;
    int4 iverb;

    while (1) {
        verb = action_menu("cmd_list_menu", &iverb);

        if(strcmp(verb, "SHORT")    == 0) get_short();
        // if(strcmp(verb, "INTEGER")  == 0) get_integer();
        // if(strcmp(verb, "LONG")     == 0) get_long();
        // if(strcmp(verb, "SHORTHEX") == 0) get_shorthex();
        // if(strcmp(verb, "HEX")      == 0) get_hexadecimal();
        // if(strcmp(verb, "LONGHEX")  == 0) get_longhex();
        // if(strcmp(verb, "REAL")     == 0) get_real();
        // if(strcmp(verb, "TEXT")     == 0) get_text();

        if(strcmp(verb,"RETURN")    == 0)
            return;
    }
}
