/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 *
 * Solution by Seb Pearce.
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // pointer to the file with raw data
    FILE* infile;
    infile = fopen("card.raw", "r");

    // The first 4 bytes of a JPEG can be either of:
    // 0xff    0xd8    0xff    0xe0
    // 0xff    0xd8    0xff    0xe1

    char buf[512] = {0};                        // byte buffer
    char jpeg[4] = {0xff, 0xd8, 0xff, 0xe0};    // JPEG demarcation
    char jpeg2[4] ={0xff, 0xd8, 0xff, 0xe1};    // JPEG demarcation
    char filename[30];                          // filename to be used
    int filenumber = 0;                         // keep track of file numbers
    FILE* outfile;                              // pointer to the outfile
    int c = 0;

    while (c != EOF) {

        // fill the byte buffer with 1 block (512 bytes)
        for (int i = 0; i < 512; i++) {
            buf[i] = c = fgetc(infile);
        }

        if (memcmp(buf, jpeg, 4) == 0 || memcmp(buf, jpeg2, 4) == 0) {

            // close the last outfile if there was one
            if (outfile) fclose(outfile);

            // create filename for next outfile (up to 3 leading zeros)
            int n;
            filenumber++;
            n = sprintf(filename, "%03d.jpg", filenumber);

            // open a new outfile
            outfile = fopen(filename, "w");

            // catch errors
            if (!outfile) {
                printf("Error opening outfile.\n");
                return 1;
            }

            // write the first 512 bytes
            fwrite(buf, 1, 512, outfile);

        } else if (outfile) {

            // if the outfile is still open, write the current block
            fwrite(buf, 1, 512, outfile);
        }
    }

    // close all files
    fclose(infile);
    if (outfile) fclose(outfile);

    printf("Done.\n");

    return 0;
}