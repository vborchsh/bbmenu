/*
 * Menu.c - a simple user interface, portable (I hope)
 *
 * R. Paoletti - 1997
 * M. Barcelo - 2005 :P
 * Vladislav Borshch - 2022
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "BBmenu.h"
#include "BBmenuPrivate.h"

menuPtr defineMenu, masterMenu;
menuPtr	yesNoMenu, onOffMenu, trueFalseMenu;

#define EQS(a,b) (strcmp(a,b)==0)

int2 menuDebugLevel=1;
#define True 1
#define False 0
char *delims;

void *allocMem( size_t);
void freeMem( void *);
void *MyAlloc( size_t, char *, char *);
void MyFree( void *, char *, char *);
void *allocMemWhatBy( size_t, char *, char *);
void freeMemWhatBy( void *, char *, char *);
/* 
 * Disable this definition if you want to use the standard memory routines
 */
#define malloc(a,b,c) malloc(a)
#define   free(a,b,c)  free(a)

/* 
 * Disable this definition if you want to use the standard gets
 */

//#define gets gethists


#define GETH_ALT 1
#define GETH_ESC 2
#define GETH_FUN 4
#define GETH_SFT 8
#define GETH_ACC 16
#define GETH_ESP 32
#define GETH_ESCW 64

#define MAX_HIST 256
#define MASK_HIST 0xFF

// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
#define BBMENU_STRING_LEN 160
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<
int2 AllowInput=0;

/* 
 * Disable this definition if you want to use the standard strtok
 */


#define strtok strtock

int2 ndlmtr=4;
char dlmtrs[]={ '"', '\'', '{', '$'};
char dlmtre[]={ '"', '\'', '}', '$'};

/*this is a version of gets that allows for correcting the input data and
  has some history of commands :P */




/*
 * This is my own version of strtok, to take care or " " strings, and 
 * to avoid problems with other routines calling strtok
 * What this routine must do is:
 *   - split the string in tokens as from "delims" (as strtok does)
 *   - treat everything between " as a single string
 */
char *strtock( char *string, char *delims)
{
	char *pnt;
	int2 i, j;
  int2 lenstr;
  int2 lendelim;
	char endc;
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	char tmp[80];
	char tmp[BBMENU_STRING_LEN];
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<

  lendelim = strlen(delims);
/*
 * When string is not NULL let's make a copy and save it
 */
	if(string!=NULL)
	{
		if(sc!=NULL)
		{
			sprintf((char *)tmp,"stringCopy[%ld]",theStream);
			free(sc,(char *)tmp,"strtock");sc=NULL;
		}
		sprintf((char *)tmp,"stringCopy[%ld]",theStream);

    lenstr = strlen(string);
//		sc=(char *)malloc(sizeof(char)*(strlen(string)+1),(char *)tmp,"strtock");
		sc=(char *)malloc(sizeof(char)*(lenstr+1),(char *)tmp,"strtock");
		strcpy(sc,string);
//    printf("MY STRINGS:  %s %s\n",(char *)sc, string);
		for(i=0;i<lenstr;i++)
		{
			for(j=0;j<lendelim;j++)
			{
				if( *(sc+i) == *(delims+j) ) 
          *(sc+i)=' ';
			}
		}
		sl=lenstr;
		so = 0;
	}
	if(so>=sl)return NULL;
	/*
	 * Skip leading blanks
	 */
	i=0;
	while(*(sc+so+i)==' ')
	{
		i++; if((so+i)>sl) return NULL;
	}
	so+=i;
/*
 * Skip the comment lines 
 */
	if(*(sc+so)=='!') return NULL;
/*
 * Look for end  of string
 */
	endc=' ';
	for(i=0;i<ndlmtr;i++)
	{
		if(*(sc+so)==dlmtrs[i])
		{
			endc=dlmtre[i];
			so++;
		}
	}
/*
 * Scan the string until the end_character is found
 * If we reach the end-character then return the string as is.
 */
	i=0;
	while(*(sc+so+i)!=endc)
	{
/*		if(endc=='}')printf("char = %c\n", *(sc+so+i));	*/
		i++; 
		if((so+i)>sl) 
		{
			pnt=sc+so;
			so+=i+1;
			return pnt;
		}
	}
/* We found the end-character. Terminate the string and raise pointer by i+1
 * so next time we start from the right place.
 */
	*(sc+so+i)='\0';
	pnt=sc+so;
	so+=i+1;
	return pnt;
}

/*
 * Routine to print a menu. Mainly used for debugging
 */
int4 dump_menu( menuPtr theMenuPtr)
{
	verbPtr vp;
	qualPtr qp;

	printf("\nDump for menu: %s \n\n",
		theMenuPtr->title);
	vp=(verbPtr )theMenuPtr->verb;
	while(vp!=NULL)
	{
		printf("\t[%p] %s \t [%p] %s\n",
				vp->response,vp->response,
				vp->description,vp->description);
		qp=(qualPtr )vp->qual;
		{
			while(qp!=NULL)
			{
				printf("\t\t %s \t %s \t %s \t %s\n",
						qp->response,
						qp->description,
						qp->type,
						qp->data);
				qp=(qualPtr )qp->qual;
			}
		}
		vp=(verbPtr )vp->verb;
	}
	return 0;
}

int4 menu_delims( char *new)
{
	if(new!=NULL)
	  {
	    if(strlen(new)!=strlen(delims))
	    {
			free(delims,"delims","menu_delims");
			delims=(char *)malloc(sizeof(char)*(strlen(new)+1),"delims","menu_delims");
	    }
	    strcpy(delims,new);
	  }
	return 0;
}

int4 menu_init( void)
{
	verbPtr vp;

	printf("\n%s\n\n",VERSION);

	delims=(char *)malloc(sizeof(char)*3,"delims","menu_init");
	strcpy(delims," \t");
	menuStrings[0]=(char *)malloc(FBUFSIZ*sizeof(char),"menuStrings[0]","menu_init");

	defineMenu = define_menu( "Define Menu", "Define>> ");
	vp=define_verb( defineMenu, "Menu", "Define new menu");
		define_qual( vp, "Prompt", "Text", "Define menu prompt");
		define_qual( vp, "Title", "Text", "Define menu title");
	vp=define_verb( defineMenu, "Verb", "Define new verb");
		define_qual( vp, "Response", "Text", "Define verb response");
		define_qual( vp, "Description", "Text", "Define verb description");
	vp=define_verb( defineMenu, "Qualifier", "Define new qualifier");
		define_qual( vp, "Response", "Text", "Define verb response");
		define_qual( vp, "Type", "Text", "Define qualifier type");
		define_qual( vp, "Description", "Text", "Define verb description");
	define_verb( defineMenu, "End", "Exit this menu");

	masterMenu = define_menu( "Master Menu", "Master>> ");

	return 0;
}

void to_upper_case(char *string)
{
	int2 i;
	for(i=0;i<strlen(string);i++)
		if((*(string+i)>='a')&&(*(string+i)<='z'))
			*(string+i)+='A'-'a';
}

void remove_new_line( char *string)
{
	int2 i;

	for(i=0;i<strlen(string);i++)
	{
		if(*(string+i)=='\n')
		{
			*(string+i)='\0'; return;
		}
	}
	return;
}


menuPtr get_menu_Ptr( char *name)
{
	char *str;
	int2 i;

	str=(char *)malloc(sizeof(char)*(strlen(name)+1),"str","get_menu_Ptr");
	strcpy(str,name);
	to_upper_case(str);
	for(i=0;i<nMenus;i++)
	{
		if(EQS(str,nameMenus[i])) 
		{
			free(str,"str","get_menu_Ptr");
			return ptrMenus[i];
		}
	}
	free(str,"str","get_menu_Ptr");
	return NULL;
}

verbPtr get_verb_Ptr( menuPtr theMenuPtr, char *verb)
{
	verbPtr vp;

	vp = theMenuPtr->verb;
	while(vp!=NULL)
	{
/*
 *		if(menuDebugLevel)printf("Checking %s with %s\n", vp->response, verb);
 */
		if(strcmp(vp->response,verb)==0)
			return vp;
		vp=vp->verb;
	}
	return NULL;
}

qualPtr get_qual_Ptr( verbPtr theVerbPtr, char *qual)
{
	qualPtr qp;

	
	qp = theVerbPtr->qual;
	while(qp!=NULL)
	{
		if(strcmp(qp->response,qual)==0)
			return qp;
		qp=qp->qual;
	}
	return NULL;
}

int4 get_qualifier( char *menu, char *verb, char *qual, Ptr value)
{
	menuPtr	mp;
	verbPtr vp;
	qualPtr qp;

	mp = get_menu_Ptr( menu);
	if(mp==NULL) return 1L;

	vp = get_verb_Ptr( mp, verb);
	if(vp==NULL) return 1L;

	qp = get_qual_Ptr( vp, qual);
	if(qp!=NULL)
	{
		if(qp->data!=NULL)
		{
/*
 *			printf("response=%s type=%s data=%s \n",
 *				qp->response, qp->type, qp->data);
 */
			if(strncmp(qp->type,"INTEGER",3)==0)
				sscanf( qp->data, "%ld", (int4 *)value);
			if(strncmp(qp->type,"HEXADECIMAL",3)==0)
				sscanf( qp->data, "%lX", (int4 *)value);
			if(strcmp(qp->type,"REAL")==0)
				sscanf( qp->data, "%f", (float *)value);
			if(strncmp(qp->type,"TEXT",4)==0)
				sscanf( qp->data, "%s", (char *)value);
			return 0L;
		}
	}

	return 1L;
}

char *fgetline( char *str, int2 nmax, FILE *fp)
{
	char c;
	int2 n=0;

	c = fgetc(fp);
	if(c==EOF)return NULL;
	while(n<nmax)
	{
/*
 *		if(menuDebugLevel>1)printf("fgetline: reading char %c\n", c);
 */
		switch(c)
		{
			case '\n':
				return str;
				break;
			default:
				*(str+n)=c; 
				n++;
				*(str+n)='\0'; 
		}
		c = fgetc(fp);
		if(c==EOF)return str;
	}
	return str;
}

char *get_string( char *prompt)
{
  char tmp[FBUFSIZ];

getverb:

  if(theStream==0)
  {
    if((menuToken = strtok( NULL, delims))==NULL)
    {
      if(BBasync==0) printf("%s",prompt);
getv1:			if(BBasync==1) {
              tmp[0]=0;
              usleep(10);
            }
      else
        gets((char *)tmp);

      if(*tmp=='!')goto getv1;

      while (!AllowInput)
      {
        usleep(10);
      }

      simulate_input( (char *)tmp);
      menuToken = strtok( NULL, delims);
//mingw      funlockfile(stdin);
    }
  }
  else
  {
    if((menuToken = strtok( NULL, delims))==NULL)
    {
getv2:		if(fgets((char *)tmp,FBUFSIZ,fileStreams[theStream])==NULL) 
            /*
             * getv2:			if(fgetline((char *)tmp,FBUFSIZ,fileStreams[theStream])==NULL)
             */
          {
            fclose(fileStreams[theStream]); 
            sprintf((char *)tmp,"menuStrings[%ld]",theStream);
            free(menuString,(char *)tmp,"get_string");menuString=NULL;
            sprintf((char *)tmp,"stringCopy[%ld]",theStream);
            free(sc,(char *)tmp,"get_string");sc=NULL;
            theStream--;
            goto getverb;
          }
          if(*tmp=='!')goto getv2;
          remove_new_line((char *)tmp);
          simulate_input( (char *)tmp);
          menuToken = strtok( NULL, delims);
    }
  }
  return menuToken;
}

char *get_next_string( char *prompt)
{
	char tmp[FBUFSIZ];

	getverb:

	if(theStream==0)
	{
		if(menuStrings[0]==NULL)menu_init();
	}
	else
	{
		if(menuString==NULL)
		{
			sprintf((char *)tmp,"menuStrings[%ld]",theStream);
			menuString=(char *)malloc(FBUFSIZ*sizeof(char),"menuString","get_string");
		}
	}

	menuToken = get_string( prompt);

	if(menuToken!=NULL&&*(menuToken)=='@')
	{
		if(theStream<NSTREAMS)
		{
			theStream++;
			strcpy((char *)tmp,menuToken);
			if(strstr((char *)tmp,".uic")==NULL)strcat((char *)tmp,".uic");
			if((fileStreams[theStream]=fopen((char *)(tmp+1),"r"))==NULL)
			{
				printf("***** Bad Command Input File Name or Syntax (File: %s) *****\n",menuToken+1);
				printf("***** Illegal Response (%s) *****\n",menuToken);
				theStream--;
			}
			else
			{
/*				printf("Opened stream %ld as %s (%p)\n",theStream,tmp+1,fileStreams[theStream]);
 */
			}
		}
		goto getverb;
	}

	return menuToken;
}

int4 get_text( char *prompt, char *file)
{
	char *temp1, *temp2;

	if((temp1=(char *)malloc(sizeof(char)*FBUFSIZ,"temp1","get_text"))==NULL)
		return 1;

	sprintf(temp1, "%s [%s]: ", prompt, file);
	temp2=get_string( temp1); free(temp1,"temp1","get_text");
	if(temp2==NULL) return 0;
	if(strlen(temp2)>0)strcpy(file,temp2);
	return 0;
}

int4 get_real_data( char *prompt, float *ivalue, float low, float high, char *fmt)
{
	float itmp;
	char *temp1, *temp2;
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	char str[80];
	char str[BBMENU_STRING_LEN];
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<

getme:
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	if((temp1=(char *)malloc(sizeof(char)*80,"temp1","get_real_data"))==NULL)
	if((temp1=(char *)malloc(sizeof(char)*BBMENU_STRING_LEN,"temp1","get_real_data"))==NULL)
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<
		return 1;

	sprintf((char *)str,    "%%s (%s,%s): [%s] ", fmt, fmt, fmt);
	sprintf(temp1, (char *)str, prompt, low, high, *ivalue);
	temp2=get_next_string( temp1); free(temp1,"temp1","get_real_data");
	if(temp2==NULL) return 0;
	itmp=*ivalue;
	sscanf( temp2, fmt, &itmp);
	if(low!=0||high!=0)
	{
		if( itmp<low || itmp>high )
		{
			printf( "***** Value is Outside Limits *****\n");
			goto getme;
		}
	}
	*ivalue=itmp;
	return 0;
}


int4 get_int_data( char *prompt, int4 *ivalue, int4 low, int4 high, char *fmt)
{
	int4 itmp;
	char *temp1, *temp2;
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	char str[80];
	char str[BBMENU_STRING_LEN];
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<

getme:
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	if((temp1=(char *)malloc(sizeof(char)*80,"temp1","get_int_data"))==NULL)
	if((temp1=(char *)malloc(sizeof(char)*BBMENU_STRING_LEN,"temp1","get_int_data"))==NULL)
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<
		return 1;

	sprintf((char *)str,    "%%s (%s,%s): [%s] ", fmt, fmt, fmt);
	sprintf(temp1, (char *)str, prompt, low, high, *ivalue);
	temp2=get_next_string( temp1); free(temp1,"temp1","get_int_data");
	if(temp2==NULL) return 0;
	itmp=*ivalue;
	sscanf( temp2, fmt, &itmp);

	if(low!=0||high!=0)
	{
		if( itmp<low || itmp>high )
		{
			printf( "***** Value is Outside Limits *****\n");
			goto getme;
		}
	}
	*ivalue=itmp;
	return 0;
}

int4 get_short_data( char *prompt, int2 *ivalue, int2 low, int2 high, char *fmt)
{
	int4 itmp;
	char *temp1, *temp2;
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	char str[80];
	char str[BBMENU_STRING_LEN];
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<

getme:
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	if((temp1=(char *)malloc(sizeof(char)*80,"temp1","get_short_data"))==NULL)
	if((temp1=(char *)malloc(sizeof(char)*BBMENU_STRING_LEN,"temp1","get_short_data"))==NULL)
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<
		return 1;

	sprintf((char *)str,    "%%s (%s,%s): [%s] ", fmt, fmt, fmt);
	sprintf(temp1, (char *)str, prompt, low, high, *ivalue);
	temp2=get_next_string( temp1); free(temp1,"temp1","get_short_data");
	if(temp2==NULL) return 0;
	itmp=(int4 )*ivalue;
	sscanf( temp2, fmt, &itmp);
	if(low!=0||high!=0)
	{
		if( itmp<low || itmp>high )
		{
			printf( "***** Value is Outside Limits *****\n");
			goto getme;
		}
	}
	*ivalue=(int2 )itmp;
	return 0;
}


int4 get_real( char *prompt, float *ivalue, float low, float high)
{
	return get_real_data( prompt, ivalue, low, high, "%f");
}

int4 get_short( char *prompt, int2 *ivalue, int2 low, int2 high)
{
	return get_short_data( prompt, ivalue, low, high, "%d");
}

int4 get_shorthex( char *prompt, int2 *ivalue, int2 low, int2 high)
{
	return get_short_data( prompt, ivalue, low, high, "%X");
}

int4 get_integer( char *prompt, int4 *ivalue, int4 low, int4 high)
{
	return get_int_data( prompt, ivalue, low, high, "%ld");
}

int4 get_hexadecimal( char *prompt, int4 *ivalue, int4 low, int4 high)
{
	return get_int_data( prompt, ivalue, low, high, "%08lX");
}


int4 strip_quals( verbPtr verb, char *token)
{
    int2 i;
	qualPtr qp;
    char *data = 0;

/*
 *	printf("Checking qualifiers for verb: %s\n", verb->response);
 */

	qp=verb->qual;
	while(qp!=NULL)
	{
		if(qp->data!=NULL)free(qp->data,"qp->data","strip_quals");
		qp->data=NULL;
		qp=qp->qual;
	}

	for(i=strlen(token);i>0;i--)
	{
		/*
		 * First look for a = sign and store the data
		 */
		if(*(token+i)=='=')
		{
			*(token+i)='\0';
			data=token+i+1;
		}
		if(*(token+i)=='\\')
		{
			*(token+i)='\0';
			/*
			 * Now look for a corresponding response 
			 */
			qp=verb->qual;
			while(qp!=NULL)
			{
/*
 *				printf("Now checking qualifier %s with %s, %ld\n",
 *					qp->response,token+i+1,strlen(token+i+1));
 */
				if(strncmp(token+i+1,qp->response,strlen(token+i+1))==0)
				{
					/*
					 * Release used memory and allocate new one
					 * In fact, we may need to store a inter string...
					 */
					if(qp->data)free(qp->data,"qp->data","strip_quals");
					qp->data=(char *)malloc(sizeof(strlen(data)+1),"qp->data","strip_quals");
                    strcpy(qp->data, data);
/*
 *					printf("qual.response=%s\n",qp->response);
 *					printf("qual.data=%s\n",qp->data);
 */
				}
				qp = qp->qual;
			}
		}
	}

	return 0;
}

char *get_menu_verb( menuPtr theMenuPtr, int4 *iaction)
{
    int2 i, j, found, ifound = 0, islash;
    verbPtr vp, foundVp = 0;
	qualPtr qp;
    char *foundStr = 0, *tmpstr = 0;
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	char tmp[80];
	char tmp[BBMENU_STRING_LEN];
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<

	*iaction=0;
	if( theMenuPtr==NULL) return NULL;

getverb:

	if(theStream==0)
	{
		if(menuStrings[0]==NULL)menu_init();
	}
	else
	{
		if(menuString==NULL)
		{
			sprintf((char *)tmp,"menuStrings[%ld]",theStream);
			menuString=(char *)malloc(FBUFSIZ*sizeof(char),(char *)tmp,"get_menu_verb");
		}
	}

	menuToken = get_next_string( theMenuPtr->prompt);
/*
 *	if(menuDebugLevel>0)printf("Verb returned by get_string is %\s\n", menuToken);
 */
	if(menuToken==NULL) goto getverb;
	while(strlen(menuToken)==0)
			goto getverb;

/*
 * Are we asked for help?
 */
	if(strcmp(menuToken,"?")==0)
	{
		for(i=0;i<80;printf("-"),i++){};printf("\n");
		for(i=0;i<40-strlen(theMenuPtr->title)/2; printf(" "),i++){};
		printf("%s\n\n", theMenuPtr->title);
		vp=(verbPtr )theMenuPtr->verb;
		j=0;
		while(vp!=NULL)
		{
			j++;
			printf("%3d  [%s]", j, vp->response);
			for(i=0;i<20-strlen(vp->response); printf(" "),i++){};
			printf("%s\n", vp->description);
			vp=(verbPtr )vp->verb;
		}
		printf("\n");
		goto getverb;
	}
/*
 * 
 */
	if(*menuToken=='?')
	{
		to_upper_case(menuToken+1);
		vp=(verbPtr )theMenuPtr->verb;
		while(vp!=NULL)
		{
			if(strncmp(menuToken+1,vp->response,strlen(menuToken+1))==0)
			{
				printf("%s\t\t%s\n",vp->response,vp->description);
				qp=(qualPtr )vp->qual;
				while(qp!=NULL)
				{
					printf("\t%s\t%s\n",qp->response,qp->description);
					qp=(qualPtr )qp->qual;
				}
			}
			vp=(verbPtr )vp->verb;
		}
		goto getverb;
	}
/*
 * Is this a file to read?
 */
	if(*(menuToken)=='@')
	{
		if(theStream<NSTREAMS)
		{
			theStream++;
			tmpstr=(char *)malloc(sizeof(char)*(strlen(menuToken)+5),"tmpstr","get_menu_verb");
			strcpy(tmpstr,menuToken);
			if(strstr(tmpstr,".uic")==NULL)strcat(tmpstr,".uic");
			if((fileStreams[theStream]=fopen(tmpstr+1,"r"))==NULL)
			{
				printf("***** Bad Command Input File Name or Syntax (File: %s) *****\n",menuToken+1);
				printf("***** Illegal Response (%s) *****\n",menuToken);
				theStream--;
			}
			else
			{
/*				printf("Opened stream %ld as %s (%p)\n",theStream,tmp+1,fileStreams[theStream]);
 */
			}
			free(tmpstr,"tmpstr","get_menu_verb");
/*
 *			printf("File %s opened, stream=%ld\n",menuToken+1,theStream);
 */
		}
		goto getverb;
	}
/*
 * Check that this token corresponds to a menu verb
 * First take the qualifiers out
 */
	found=0;
	/*
	 * Conver to upper case
	 */
	to_upper_case(menuToken);

	islash=0;
	for(i=0;i<strlen(menuToken);i++)
	{
		if(*(menuToken+i)=='\\')
		{
			*(menuToken+i)='\0';
			islash=i+1;
			break;
		}
	}
			
	i=0;
	vp=(verbPtr )theMenuPtr->verb;
	while(vp!=NULL)
	{
		if(strncmp(menuToken,vp->response,strlen(menuToken))==0)
		{
			found++; ifound=i;
			foundVp=vp; foundStr=vp->response;
		}
		i++;
		vp=(verbPtr )vp->verb;
	}
	if(found==0)
	{
		printf("***** Illegal Response (%s) *****\n",menuToken);
		goto getverb;
	}
	if(found==1)
	{
		/*
		 * Strip qualifiers from this token
		 */
		if(islash>0)*(menuToken+islash-1)='\\';
		strip_quals( (verbPtr )foundVp, menuToken);
		*iaction=ifound;
		return foundStr;
	}
	if(found>1)
	{
		printf("***** Ambiguous Response (%s) *****\n",menuToken);
		goto getverb;
	}

	return NULL;
}


menuPtr define_menu( char *tit, char *pro)
{
	menuPtr mP;

	mP = (menuPtr )malloc(sizeof(menuData),"mP","define_menu");
	mP->title=(char *)malloc(sizeof(char)*(strlen(tit)+1),"mP->title","define_menu");
	strcpy( mP->title, tit);
	mP->prompt=(char *)malloc(sizeof(char)*(strlen(pro)+1),"mP->prompt","define_menu");
	strcpy( mP->prompt, pro);
	mP->verb = NULL;

	return mP;
}

verbPtr define_verb( menuPtr mP, char *resp, char *desc)
{
	verbPtr vP, vpnt;

	vP = (verbPtr )malloc(sizeof(verbData),"vP","define_verb");
	vpnt=(verbPtr )mP->verb;
	if( vpnt==NULL) 
		mP->verb=(Ptr )vP;
	else
	{
		while(vpnt->verb!=NULL) 
			vpnt=(verbPtr )vpnt->verb;
		vpnt->verb=(Ptr )vP;
	}
	vP->response=(char *)malloc(sizeof(char)*(strlen(resp)+1),"vP->response","define_verb");
	strcpy( vP->response, resp); to_upper_case(vP->response);
	vP->description=(char *)malloc(sizeof(char)*(strlen(desc)+1),"vP->description","define_verb");
	strcpy( vP->description, desc); 
	vP->qual = NULL;
	vP->verb = NULL;
	return vP;
}

int4 define_qual( verbPtr vP, char *resp, char *type, char *desc)
{
	qualPtr qP, qpnt;

	qP = (qualPtr )malloc(sizeof(qualData),"qP","define_qual");
	qpnt=(qualPtr )vP->qual;
	if( qpnt==NULL) 
		vP->qual=(Ptr )qP;
	else
	{
		while(qpnt->qual!=NULL) 
			qpnt=(qualPtr )qpnt->qual;
		qpnt->qual=(Ptr )qP;
	}
	qP->response=(char *)malloc(sizeof(char)*(strlen(resp)+1),"qP->response","define_qual");
	strcpy( qP->response, resp);
	to_upper_case( qP->response);

	qP->type=(char *)malloc(sizeof(char)*(strlen(type)+1),"qP->type","define_qual");
	strcpy( qP->type, type); 
	to_upper_case( qP->type);

	qP->description=(char *)malloc(sizeof(char)*(strlen(desc)+1),"qP->description","define_qual");
	strcpy( qP->description, desc); 

	qP->data = NULL;
	qP->qual = NULL;

	return 0;
}


int4 simulate_input( char *string)
{
	char *tmp;
	strcpy( menuString, "? ");
	strcat( menuString, string);
	tmp=strtok(NULL,delims);
	while(tmp!=NULL)
	{
		strcat( menuString, " $");
		strcat( menuString, tmp);
		strcat( menuString, "$");
		tmp=strtok(NULL,delims);
	}
	tmp=strtok( menuString, delims);
//	printf(" SIMULATE input: %s\n",menuString);
//	error_logger(__FILE__,1,menuString);
	return 0;
}

int4 define_file( char *filnam)
{
	char *tmp;
	menuPtr mp=NULL;
	verbPtr vp=NULL;
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	char prompt[80], descr[256], resp[80], type[80], title[80], name[80];
	char prompt[BBMENU_STRING_LEN], descr[256], resp[BBMENU_STRING_LEN], type[BBMENU_STRING_LEN], title[BBMENU_STRING_LEN], name[BBMENU_STRING_LEN];
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<
	char *action;
	int4 iaction;

	if(*filnam=='@')
	{
		tmp=(char *)malloc(sizeof(char)*(strlen(filnam)+1),"tmp","define_file");
		strcpy(tmp,filnam);
	}
	else
	{
		tmp=(char *)malloc(sizeof(char)*(strlen(filnam)+2),"tmp","define_file");
		strcpy(tmp,"@");
		strcat(tmp,filnam);
	}		
	simulate_input( tmp);
	free(tmp,"tmp","define_file");
	
	while(True)
	{
		*prompt='\0';
		*descr ='\0';
		*resp  ='\0';
		*type  ='\0';
		*title ='\0';
		action=get_menu_verb( defineMenu,  &iaction);
		if(EQS(action,"MENU"))
		{
			get_text( "Enter menu name", (char *)name);
			to_upper_case( (char *)name);
			get_text( "Enter menu title", (char *)title);
			get_text( "Enter menu prompt", (char *)prompt);
			mp = define_menu( (char *)title, (char *)prompt);
			nameMenus[nMenus]=(char *)malloc(sizeof(char)*(strlen(name)+1),"nameMenus[nMenus]","define_file");
			strcpy(nameMenus[nMenus],name);
			ptrMenus[nMenus]=mp;
			nMenus++;
			define_verb( masterMenu, (char *)name, (char *)title);
		}
		if(EQS(action,"VERB"))
		{
			get_text( "Enter verb response", (char *)resp);
			get_text( "Enter verb description", (char *)descr);
			vp = define_verb( mp, (char *)resp, (char *)descr);
		}
		if(EQS(action,"QUALIFIER"))
		{
			get_text( "Enter qualifier prompt", (char *)prompt);
			get_text( "Enter qualifier type", (char *)type);
			get_text( "Enter qualifier description", (char *)descr);
			define_qual( vp, (char *)prompt, (char *)type, (char *)descr);
		}
		if(EQS(action,"END"))break;
	}
	return 0;
}

void destroy_menu( menuPtr mp)
{
	verbPtr vp;
	qualPtr qp;

	vp=mp->verb;
	while(vp!=NULL)
	{
		qp=vp->qual;
		while(qp!=NULL)
		{
			free(qp,"qp","destroy_menu");
			qp=qp->qual;
		}
		free(vp,"vp","destroy_menu");
		vp=vp->verb;
	}
}

void destroy_verb( verbPtr vp)
{
	qualPtr qp;

	while(vp!=NULL)
	{
		qp=vp->qual;
		while(qp!=NULL)
		{
			free(qp,"qp","destroy_verb");
			qp=qp->qual;
		}
		free(vp,"vp","destroy_verb");
		vp=vp->verb;
	}
}

char *action_menu( char *name, int4 *iaction)
{
	int4 i;
	char *action;
	simulate_input(name);
	action=get_menu_verb( masterMenu, &i);
/*
 *	if(menuDebugLevel>0)printf("Querying menu %s: answer=%s\n", name, action);
 */
	action=get_menu_verb( get_menu_Ptr( action), iaction);
	return action;
}

void flush_buffers()
{
// >>>>> changed by M. Fras, 27 Feb 2009 >>>>>
//	char tmp[80];
// <<<<< changed by M. Fras, 27 Feb 2009 <<<<<
	int4 is;

	for(is=theStream;is>0;is--)
	{
	  //		printf("closed stream %ld (%p)\n",is,fileStreams[is]);
		fclose(fileStreams[is]); 
		*menuString='\0';
		*sc='\0';
		sl=0; so=0;
	}
	theStream=0;
	*menuString='\0';
	*sc='\0';
	sl=0; so=0;
}

void set_async(int4 val)
{
	if(val==0)
		BBasync=0;
	else
		BBasync=1;
}
