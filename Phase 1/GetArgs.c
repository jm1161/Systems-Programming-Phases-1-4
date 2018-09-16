/*
    This is a simple program to illustrate how to use
    the fgets function (and to some extent, the sscanf
    function).
*/

#include <stdio.h>
#include <string.h>

void breakUp(char *, char *, char *, char *, int *);
int main()
{
    char line[80], comm[10], p1[10], p2[10];
    int len, n;

    printf("Please enter a command: ");

        /* read in a line */
    fgets(line, 80, stdin);

        /* get rid of trailing newline character */
    len = strlen(line) - 1;
    if (line[len] == '\n')
        line[len] = '\0';

        /* Break up the line */
    breakUp(line, comm, p1, p2, &n);

    printf ("%d things on this line\n", n);
    printf ("command: %s\n", comm);
    printf ("parameter 1: %s\n", p1);
    printf ("parameter 2: %s\n", p2);

    return 0;
}

/*
   This function takes a line and breaks it into words.
   The orginal line is in the char array str, the first word
   will go into the char array c, the second into p1, and the
   the third into p2.  If there are no words, the corresponding
   char arrays are empty.  At the end, n contains the number of
   words read.
*/
void breakUp(char *str, char *c, char *p1, char *p2, int *n)
{
    c[0] = p1[0] = p2[0] = '\0';

    /* YOU CANNOT USE THE FOLLOWING!  IT IS A BUILT-IN FUNCTION! */
    *n = sscanf(str,"%s %s %s %*s", c, p1, p2);
}

