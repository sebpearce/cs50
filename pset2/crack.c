#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// location of the dictionary file to use
#define DICT "/usr/share/dict/words"

int testword(char *pass, char *plaintext);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Please enter a password to crack.\n");
        return 1;
    }

    int passwds = 0;    // number of passwords tried so far
    int result;         // return value of testword(), will be zero or nonzero
    char buf[30];       // buffer to hold dictionary word
    FILE *file;         // infile pointer

    // open the dictionary file
    file = fopen(DICT, "r");
    if (!file) {
        printf("Problem opening file.\n");
        return 1;
    }

    // get each line of dict file and extract the word
    while (fgets(buf, 30, file) != NULL) {
        int i = 0;
        while (buf[i] != '\n' && buf[i] != '\r') i++;
        buf[i] = '\0';

        // test the word and check if it's a match
        result = testword(argv[1], buf);
        if (result) {
            printf("Password found after testing %d passwords.\n", passwds);
            printf("Password: %s\n", buf);
            return 0;
        }
        // update the number of passwords tried
        passwds++;
        // print progress every 500 passwords so user can see it's working
        if (passwds % 500 == 0) {
            printf("%d...\n", passwds);
        }
    }

    // if there are no matches, we were unsuccessful
    printf("Tried %d passwords but could not find a match.\n", passwds);
    return 0;
}

// function to test a given dictionary word against a password & 4096 salts
int testword(char *pass, char *plaintext)
{
    int i = 0;
    int j = 0;
    char *ciphertext;   // pointer to result of crypt function
    // our set of possible characters:
    char fullset[] = 
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
    char salt[2];       // 2-char buffer to try different salt combos
     
    // try out 4096 different salt combinations
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {

            salt[0] = fullset[i];
            salt[1] = fullset[j];

            // run crypt with the current dictionary word and current salt
            ciphertext = crypt(plaintext, salt);  

            // check for a match
            if (strcmp(ciphertext, pass) == 0) {
                return i;
            }
        }
    }
    return 0;
}
