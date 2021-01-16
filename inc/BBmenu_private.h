
#define True  1
#define False 0

#define Ptr void *

typedef struct _menu_struct {
 	char *title;				/* the menu title */
	char *prompt;				/* the menu prompt */
 	Ptr verb;				/* pointer to first verb in menu */
 	} *menuPtr, menuData;

typedef struct _verb_struct_ {
 	char *response;				/* verb response */
 	char *description;			/* verb description	*/
	Ptr verb;				/* pointer to next verb in menu */
	Ptr qual;				/* pointer to qualifier */
	} *verbPtr, verbData;

typedef struct _qual_struct_ {
 	char *response;				/* qualifier response */
	char *type;				/* qualifier type (integer, real, logical, ƒ) */
 	char *description;			/* qualifier description	*/
	char *data;				/* pointer to qualifier data */
	Ptr   qual;				/* Pointer to next qualifier */
	} *qualPtr, qualData;

#define VERSION "*** BBmenu, version 2.0 ***"

int4 theStream=0;
FILE *fileStreams[NSTREAMS];
char *menuStrings[NSTREAMS];
char *menuToken;

int4 stringOffset[NSTREAMS];
char *stringCopy[NSTREAMS];
int4 stringLength[NSTREAMS];

int4 nMenus=0;
char  *nameMenus[NMENUS];
menuPtr ptrMenus[NMENUS];

#define menuString menuStrings[theStream]
#define so stringOffset[theStream]
#define sc stringCopy[theStream]
#define sl stringLength[theStream]

char *get_menu_verb( menuPtr , int4 *);
menuPtr define_menu( char *, char *);
verbPtr define_verb( menuPtr, char *, char *);
int4 define_qual( verbPtr, char *, char *, char *);
int4 dump_menu( menuPtr );
void freeMem( Ptr);
Ptr mallocMem( size_t);
void flush_buffers(void);

int4 BBasync=0;
void set_async(int4 );
