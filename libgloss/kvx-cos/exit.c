/* This file has been modified or added by Kalray, SA. 2010 */

/***************************************************************
 *	exit
 ***************************************************************/

extern void sc_exit(int)        __attribute__((noreturn));


void __gloss_exit(int exitcode)
{
	sc_exit(exitcode);
	/* should never be reached as exit doesn't return. */
}

void _exit(int exitcode){
    __gloss_exit(exitcode);
}

