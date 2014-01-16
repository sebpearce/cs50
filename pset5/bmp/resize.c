/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 5
 * 
 * Resizes a 24-bit BMP.
 *
 * Solution by Seb Pearce, 2014-01-12.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4) {
        printf("Usage: ./copy resize-factor infile outfile\n");
        return 1;
    }

    // remember resize factor
    float f = atof(argv[1]);

    if (f < 0.0 || f > 100.0) {
        printf("Resize factor must be between 0 and 100.\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL) {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL) {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0) 
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int oldwidth = bi.biWidth;          // width in px of infile
    int oldheight = abs(bi.biHeight);   // height in px of infile

    // change header's size details for outfile
    bi.biWidth *= f;
    bi.biHeight *= f;

    int newwidth = bi.biWidth;          // width in px of outfile
    int newheight = abs(bi.biHeight);   // height in px of outfile
    
    // get padding of infile
    // int oldpadding = (4 - (oldwidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // get padding of outfile
    int padding =  (4 - (newwidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // change image size
    bi.biSizeImage = newheight * ((newwidth * sizeof(RGBTRIPLE)) + padding);

    // change file size
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + 
                    sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // find out how many bytes are in 1 line of the infile
    int linelength = sizeof(RGBTRIPLE) * oldwidth;
    // create temporary array with which to fill scanline pixel data
    RGBTRIPLE* scanline = malloc(linelength);

    int old;    // to remember the last r value (see below)

    // for each scanline of the outfile
    for (int i = 0; i < newheight; i++) {

        // r tells us which line of the infile to use
        int r = (int)floor(i / f);

        // if we know the length of the infile's scanline in bytes,
        // we can just fseek to the correct scanline (that one that == r).
        // line 0 = (# of bytes in scanline) * 0
        // line 1 = (# of bytes in scanline) * 1

        // fill scanline array with values from next scanline of original
        // (otherwise, we'll just use the last scanline again)
        if (r > old || i == 0) {
            // find the start of the infile's scanline that we need
            // (54 bytes comprises the file & info headers)
            fseek(inptr, (54 + (linelength * r)), SEEK_SET);
            // copy the scanline's bytes into our buffer, "scanline"
            for (int x = 0; x < oldwidth; x++) {
                // read RGB triple from infile
                fread((scanline + x), sizeof(RGBTRIPLE), 1, inptr);
            }
        }

        // for each pixel in the outfile's current line
        for (int x = 0; x < newwidth; x++) {
            
            RGBTRIPLE pixel;   // temp storage

            // s tells us which pixel of the infile to use
            int s = (int)floor(x / f);
            pixel = scanline[s];
            // write pixel to the target file
            fwrite(&pixel, sizeof(RGBTRIPLE), 1, outptr);
        }

        // add padding if we need it
        if (padding > 0) {
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr); // fputc(char to be written, FILE*)
            }
        }
        old = r;    // update old for the next iteration
    }

    // free memory used for scanline buffer
    free(scanline);

    // close files
    fclose(inptr);
    fclose(outptr);

    // adios
    return 0;
}
