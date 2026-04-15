/*
You will create a menu driven program that allows the user to enter up to 1000 words. 
The menu will have options to add more words and to create a text document with all words.
The program will have an option to allow the user to dump all words from the program.
The program will automatically look for an existing binary file (assuming the code was run before).
If found, the program will load all words from the binary file into the program. 
Each time the program ends, all words will be saved/loaded into the binary file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define pause system ("pause");

void FLUSH() { //Flush function to get rid of any characters from stdin
    while (getchar() != '\n');
}


void addText(FILE *src){ //Function to add text to a file
    char text[300]; //Character array that stores up to 300 characters
    printf("Enter text to add to the file: ");
    FLUSH(); //Clear stdin
    fgets(text, sizeof(text), stdin); //Get your input, even if it contains spaces
    fputs(text,src); //Place it in the file
    printf("Text added.\n");
}
void convertHexToText(FILE *hexFile, FILE *textFile) { //Converts hex text from a file
    unsigned int byte; //A variable that will store the converted value of each hex byte

    if (hexFile == NULL) { //Null check
        printf("Cannot open file.\n");
        pause;
        exit(-1);
    }

    if (textFile == NULL) { //Null check
        printf("Cannot open file.\n");
        pause;
        exit(-1);
    }

    // Read hex values and convert to text
    while (fscanf(hexFile, "%2x", &byte) == 1) {
        //While loop reads 2 hex characters from the hex file and converts them to ints, placing them in byte
        //Which is then converted to ASCII and is placed in the text file.
        fputc(byte, textFile);
    }
}

void makeFile(FILE *binFile, FILE *textFile){
    unsigned char buffer[1024]; //Buffer character array of 1024 bytes
    size_t bytesRead; //Stores how many bytes were read in each iteration of the loop
    
    textFile = fopen("IO.txt", "a"); //Opens the text file for appending
    if(textFile == NULL){ //Null check
        printf("Cannot open file.\n");
        pause;
        exit(-1);
    }
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), binFile)) > 0) {
        //This first loop reads bytes up to the size of the buffer and returns how many bytes were read
        //The loop keeps going until it returns zero, indicating the end of the file.
        for (size_t i = 0; i < bytesRead; i++) { //This loop goes over the read bytes, and for each one, it prints its hex value in the text file + a space
            fprintf(textFile, "%02x ", buffer[i]);
        }
        fprintf(textFile, "\n"); //After processing each line, a newline character is added to it.
    }
    fclose(textFile); //Close the text file
    textFile = fopen("IO.txt", "r"); //Reopen it for reading mode
    FILE *new = fopen("yourText.txt", "w"); //Make a new file to store the actual text and not the hex values
    if(new == NULL){ //Null check
        printf("Cannot open file.\n");
        pause;
        exit(-1);
    }
    convertHexToText(textFile, new); //Converts the file from hex to text.
    printf("Binary data added to text file successfully.\n");
    printf("Your file with your text should be called yourText.txt.");
    fclose(new);
    fclose(textFile);
}

int main(){
    char input; //Input for the switch
    FILE *text, *binary; //Initialize the files
    binary = fopen("IObin.bin","ab+"); //Open the binary file in append mode for adding words to it
    if(binary == NULL){ //Null check
        printf("Cannot open file.\n");
        binary = fopen("IObin.bin", "wb"); //Makes a binary file for you if one is not existent
        printf("A binary file has been made for you :)\n");
        printf("You may run the program again with no issues\n");
        pause;
        exit(-1);
    }
    printf("A binary file exists. You may edit it.\n");
    do{ //Menu
        printf("Here are your options:\n");
        printf("Type A to add more words to a file\n");
        printf("Type B to dump all words from the program\n");
        printf("Type C to make a text file with your current words\n");
        printf("Type Q to quit\n");
        printf("Please enter your selection: ");
        scanf(" %c", &input); //Takes in the input character
        switch(input){
            case 'A': //Adds text
                addText(binary);
                break;
            case 'B': //Clears the binary file
                fclose(binary);
                binary = fopen("IObin.bin","wb");
                fclose(binary);
                binary = fopen("IObin.bin", "ab+");
                printf("File cleared\n");
                break;
            case 'C': //Makes a text file
                makeFile(binary, text);
                break;
            case 'Q': //Quits
                fclose(binary);
                break;
            default: //Default case
                printf("Invalid input");
                break;
        }

    } while(input!='Q');
    return 0;
}