#include <stdio.h>
#include <stdlib.h>

extern char **environ; 

int main() {
    printf("HandsOn 15 Write a program to display the environmental variable of the user (use environ).\n Here it come.\nTo chaliye shuru karte hein\n");
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }

    return 0;
}
