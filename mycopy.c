/*
 * Regis University - CS431 - Operating System Design and Analysis
 * Author - Cody Register
 * Date - 20211023
 * Description - Copies files on a *UNIX system. Implements error handling.
 * 
 */
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char** argv) {
    int readArg, writeArg; //Which argv holds the read and write file names
    int inFile, outFile; //The files to be written and read 
    char buffer[2048]; //Buffer which holds the data being copied. Buffer size can be modified. 
    int DEBUG = 0; //DEBUG FLAG
    int readCount, writeCount, total = 0; //Holds the number of bytes being copied in various steps. 

    //Checks whether the appropriate number of arguments were given. 
    //Can be expanded to allow for options to be given before the file names.
    if (argc < 3) {
        printf("Error, Invalid number of arguments. You entered %d arguments.\n Expected usage is mycopy <Source> <Destination>", argc);
        return -1;
    }
    if (argc == 3) {
        readArg = 1;
        writeArg = 2;
    } else {
        readArg = argc - 2;
        writeArg = argc - 1;
    }

    //Read the source file.
    inFile = open(argv[readArg], O_RDONLY, 0);
    if (inFile < 0) {
        printf("Unable to open %s: %s\n", argv[readArg], strerror(errno));
        exit(-1);
    }
    //Open the destination file. Create it if needed. 
    outFile = open(argv[writeArg], O_WRONLY | O_CREAT, 0666);
    if (outFile < 0) {
        printf("Error opening %s: %s\n", argv[writeArg], strerror(errno));
        exit(-1);
    }

    //Main loop for copying data. 
    do {
        readCount = read(inFile, buffer, sizeof (buffer));
        if (DEBUG > 0) {
            printf("Current Read Count: %d\n", readCount);
        }
        writeCount = write(outFile, buffer, readCount);
        if (DEBUG > 0) {
            printf("Current Write Count: %d\n", writeCount);
        }
        total += writeCount;
        if (errno != 0) {
            printf("Error %d: %s\n ", errno, strerror(errno));

        }

    } while (readCount > 0);

    //Success message. 
    if (errno == 0) {
        printf("Copied %d bytes from file %s to %s.\n", total, argv[readArg], argv[writeArg]);

    } else {
        printf("Error %d: %s\n ", errno, strerror(errno));
        return -1;
    }
    //Cleanup
    close(inFile);
    close(outFile);
    return 0;
}
