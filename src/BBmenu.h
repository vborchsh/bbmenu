/*
 * This is MENU.H
 * Include file for MENU.C
 *
 * Menu is a simple package to provide a simple user interface.
 * It is based on text menus, easily defined in a file.
 * every menu is addresses by a name, that must be unique to avoid interference
 * with other programs that use the same package.
 * Inside a menu one can define verbs and qualifiers. Every one is defined by
 * their name as well. The ambiguity is rare in a menu context so
 * care must be taken only for menu names.
 *
 * The following rules apply everywhere:
 *
 * 1 - single commands are separated by a space " " or a tab "\t".
 *     So far no other delimiters are defined by default but can be easily implemented
 *	   by calling the routine menu_delims
 * 2 - comments lines in files must start with an exclamation point "!"
 * 3 - everything between " is treated as a single text, otherwise will be 
 *     splitted in tokens according to 1)
 *
 * The file used to define menus must have the following syntax:
 *
 * menu the_menu_name "The menu title" "Prompt>> "
 *				>> once a menu is define the verbs that follows are associated to it <<
 * verb verb_response "The verb decription"
 *				>> once a verb is define the qualifiers that follows are associated to it <<
 * qual qualifier_response qualifier_type "The qualifier description"
 *
 * ---------------------------------------------------------------------------
 *
 * Original creation: 31 october 1997 - R. Paoletti (Riccardo.Paoletti@pisa.infn.it)
 *
 * ---------------------------------------------------------------------------
 *
 * Usage summary:
 *
 * menu_init() - initializes the package
 *
 * define_file( char *file) - defines the menus by reading a file
 *
 *	e.g. let us define the menus to build a program that will ask for age,
 *		weight, fiscal code of a person
 *
 *		menu my_first_menu "This is my first menu" "Hi: "
 *			verb last_name 		"the person's last name"
 *			verb first_name 	"the person's first name"
 *			verb birth	"ask for the date of birth"
 *				qual day 	integer "the day"
 *				qual month 	integer "the month"
 *				qual year 	integer "the year"
 *				qual date	 text "the date in form yy-mon-dd"
 *			verb code	"the person's fiscal code"
 *			verb exit	"Exit the program"
 *
 *		end	<-- this command is NECESSARY to tell the package we are done
 *
 * menu_delims( "&=") - define a new set of delimiters
 *
 * action = (char *)action_menu( (char *)menu) - get a verb from the menu
 *
 * status = get_short( char *prompt, short *value, short low, short high)
 * status = get_integer( char *prompt, int *value, int low, int high)
 * status = get_long( char *prompt, long *value, long low, long high)
 * status = get_shorthex( char *prompt, short *value, short low, short high)
 * status = get_hexadecimal( char *prompt, int *value, int low, int high)
 * status = get_longhex( char *prompt, long *value, long low, long high)
 * status = get_real( char *prompt, float *value, float low, float high)
 * status = get_text( char *prompt, char *text) 
 *
 * get_qualifier( char *menu, char *verb, char *qualifier, (void *)value) 
 *
 * simulate_input( char *string) - put a string in the commands queue
 *
 * ---------------------------------------------------------------------------
 *
 * There are 3 structures: menu, verb and qualifier
 *
 * Menu structure:
 *	title
 * 	prompt
 *	verb -------------> Verb structure:
 *						response
 *						description
 *						verb --------------> next verb structure ...
 *						qual
 *						|
 *						+---------> next qualifier structure
 *									response
 *									type
 *									description
 *									data
 *									qual ...
 *
 * The following definitions can be changed according to the user necessities
 *
 * NMENUS	- the maximim number of menus that can be defined
 * NSTREAMS	- maximum number of streams that can be simultaneously opened
 * FBUFSIZ	- buffer size used to read data from file
 *
 * Just be aware that, in the worst case, the total amount of memory allocated
 * for reading the commands can be up to NSTREAMS*FBUFSIZ bytes
 *
 */
#ifndef __BBmenu_libGuard__
#define __BBmenu_libGuard__

#include "BBmenuDefs.h"

#define NMENUS 	1000
#define NSTREAMS 100
#define FBUFSIZ 20480
//#define FBUFSIZ 1024

/*
 * Function prototypes
 */

int4 menu_init(void);
int4 menu_delims( char *);
int4 define_file( char *);
char *action_menu( char *, int4 *);

int4 get_short( char *, int2 *, int2, int2);
int4 get_integer( char *, int4 *, int4, int4);
int4 get_shorthex( char *, int2 *, int2, int2);
int4 get_hexadecimal( char *, int4 *, int4, int4);
int4 get_real( char *, float *, float, float);

int4 get_text(char *, char *);
int4 get_qualifier( char *, char *, char *, void *);
int4 simulate_input(char *);

int2 AllowInput;
#endif
