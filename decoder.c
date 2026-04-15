#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct{ //Dictionary struct that I decided to make to make things easier on myself
    char letter;
    char code;
}DICT;

void makeDict(DICT letters[26]){ //Making the dictionary with the same keys as the word doc
    letters[0].letter = 'A'; letters[0].code = 'H';
    letters[1].letter = 'B'; letters[1].code = 'I'; 
    letters[2].letter = 'C'; letters[2].code = 'V'; 
    letters[3].letter = 'D'; letters[3].code = 'J'; 
    letters[4].letter = 'E'; letters[4].code = 'K'; 
    letters[5].letter = 'F'; letters[5].code = 'W'; 
    letters[6].letter = 'G'; letters[6].code = 'L'; 
    letters[7].letter = 'H'; letters[7].code = 'A'; 
    letters[8].letter = 'I'; letters[8].code = 'M'; 
    letters[9].letter = 'J'; letters[9].code = 'Y'; 
    letters[10].letter = 'K'; letters[10].code = 'N';
    letters[11].letter = 'L'; letters[11].code = 'B';
    letters[12].letter = 'M'; letters[12].code = 'X';
    letters[13].letter = 'N'; letters[13].code = 'O'; 
    letters[14].letter = 'O'; letters[14].code = 'Z';
    letters[15].letter = 'P'; letters[15].code = 'C';
    letters[16].letter = 'Q'; letters[16].code = 'T';
    letters[17].letter = 'R'; letters[17].code = 'P';
    letters[18].letter = 'S'; letters[18].code = 'U';
    letters[19].letter = 'T'; letters[19].code = 'D';
    letters[20].letter = 'U'; letters[20].code = 'Q';
    letters[21].letter = 'V'; letters[21].code = 'S';
    letters[22].letter = 'W'; letters[22].code = 'R';
    letters[23].letter = 'X'; letters[23].code = 'E';
    letters[24].letter = 'Y'; letters[24].code = 'G';
    letters[25].letter = 'Z'; letters[25].code = 'F';
}
void encodeString(char *string, int length, DICT alphabet[26]){ //Function to encode the string
    char copy[200]; //A copy of the original string to be used for any purpose
    strcpy(copy, string); //Copying the original string
    for(int i = 0; i<length; i++){ //Loop through the string
        for(int j = 0; j<26; j++){ //Loop through the indices of the dictionary
            if(copy[i] == alphabet[j].letter){ //Check if the characters are the same
                string[i] = alphabet[j].code; //Change the character to the encoded one if yes
            }
        }
    }
}
int main(){
    DICT alphabet[26]; //Dictionary with 26 spaces for the 26 letters of the alphabet
    makeDict(alphabet); //Make the dictionary
    char string[200]; //Original string. Not used for anything 
    char copy[200]; //Copy of the original string. Used for everything else
    char choice = 'Y'; //User choice on whether they want to keep going
    printf("Please enter a string you'd like to encode (no spaces): "); 
    scanf("%s", &string); //Getting your original string
    strcpy(copy, string); //Copying said string
    do{ //Menu loop
        printf("Do you wish to encode your string?\n");
        printf("Please type Y or N to proceed: ");
        scanf(" %c", &choice);
        switch(choice){
            case 'Y': //Encodes the string every time you say yes
                printf("Encoding string.\n");
                encodeString(strupr(copy), strlen(copy), alphabet);
                break;
            case 'N': //Stops the loop
                break;
            default: //In case you type anything that's not a Y or N
                printf("Input could not be accepted.\n");
        }


    }while(choice !='N');
    printf("Here's your original string: %s\n", string); //Display the OG string
    printf("Here's the encoded string: %s\n", copy); //Display the encoded string
}