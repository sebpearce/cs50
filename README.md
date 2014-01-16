CS50
====

These are my solutions to some of the CS50 course's problem sets.

CS50 is a fantastic course and I really recommend it, even if you don't enroll (I didn't).

These problem sets are based on the "Hacker Edition."

## Problem Set 2
Problem Set 2 focuses on cryptography. `crack.c` accepts an old-style *nix password from `/etc/passwd` as an argument and attempts a brute-force attack on it using a dictionary file (in this case, the one included in `/usr/share/dict/words`). I've also include a list of the 10,000 most common passwords, stored in `10k.txt`. Just change the code if you want to use it.

## Problem Set 5
This is in two parts. The first is a program that resizes a 24-bit BMP (`resize.c`), and takes the resize factor as an argument. 

The second program, `recover.c`, reads a forensic image (I didn't include the 15mb image here, but you can get it at the [course website]("https://cs50.harvard.edu")) and recovers a bunch of JPEGs from it by looking for the 4 bytes that demarcate a JPEG and routing them to a file.

## Do it
https://cs50.harvard.edu
