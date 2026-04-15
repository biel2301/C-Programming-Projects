#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct{ // dog struct
    char name[15];
    int payMult;
    int winOdds;
} DOG;

int random(int min_num, int max_num){ //Random number generator, gets a random number between min and max nums
    int result = 0, low_num = 0, hi_num = 0; //Random number result, the minimum and maximum number

    if (min_num < max_num){ //Check to make sure the minimum is really the lowest and the maximum is really the max
        low_num = min_num; //Sets the low num as the minimum number
        hi_num = max_num + 1; // include max_num in output and sets it as the high number
    } else {
        low_num = max_num + 1; // include max_num in output and makes sure that no matter the order, the numbers are correct
        hi_num = min_num; //Same thing as before
    }
    srand(time(NULL)); //Randomizes the seed based on the current time
    result = (rand() % (hi_num - low_num)) + low_num; //makes the result as the modulo of the high-low numbers and adds the low number to it to make it in range
    return result; //returns the result
}

void createDog(DOG *dog, char name[], int payMult, int winOdds){ //Makes a dog with the traits given
    strcpy(dog->name, name);
    dog->payMult = payMult;
    dog->winOdds = winOdds;
}

void getOrStore(int *money, int *bankMoney){ //
    printf("Would you like to deposit or withdraw money?\n");
    printf("Please choose between D or W: ");
    char decision; //User decision
    int amount; //How much they will deposit or withdraw
    while(1){ //Infinite loop that only breaks once the amount is valid
        scanf(" %c", &decision); //Gets the user decision
        if(decision == 'D'){
            printf("Please enter how much money you wish to deposit(note: you have %i dollars to deposit): ", *money);
            scanf("%i", &amount);
            while(amount > *money){ //Check to make sure they have enough money in hand to deposit
                printf("You do not have enough money, please enter a smaller amount. ");
                scanf("%i", &amount);
            }
            *money-= amount; //removes the amount from their hand
            *bankMoney += amount; //adds said amount to the bank
            printf("You now have: %i dollars to spend and %i dollars on reserve\n", *money, *bankMoney);
            break;
        }   
        if(decision == 'W'){
            printf("Please enter how much money you wish to withdraw(note: you have %i dollars): ", *bankMoney);
            scanf("%i", &amount);
            while(amount > *bankMoney){ //Ensures they have enough money on the bank
                printf("You do not have enough money, please enter a smaller amount. ");
                scanf("%i", &amount);
            }
            *bankMoney-= amount; //Gets rid of the bank money
            *money+= amount; //Adds it to their hand money
            printf("You now have: %i dollars to spend and %i dollars on reserve\n", *money, *bankMoney);
            break;
        }
        else{
            printf("Please enter a valid decision: "); //If they type anything that's invalid, send them back
        }
    }
    
}

bool gamble(DOG dogs[], int * money){
    bool didWin; //Boolean to keep track of whether they won or not
    char choice[15]; //Which puppy greg is choosing
    int index; //Dog array index value of the matched puppy
    int amount; //How much he bet
    bool check = true; //Check boolean for valid dog name
    printf("Please pick a dog to bet on\n");
    printf("Our dogs are: \n");
    for(int i = 0; i<9; i++){
        printf("%s\n", dogs[i].name);
    }
    while(1){ //Loop to make sure the dog name is valid
        scanf("%s", &choice);
        for(int i = 0; i<9; i++){
            if(strcmp(choice, dogs[i].name) == 0){
                index = i;
                check = false;
                break;
            }
        }
        if(!check){
            break;
        }
        else{
            printf("Your dog's name is invalid. Please enter a valid name: ");
            scanf("%s", &choice);
        }
    }
    printf("Please enter how much money you wish to bet(note: you have %i dollars to bet with): ", *money);
    scanf("%i", &amount); //Gets the bet amount
        while(amount > *money){ //Making sure they have enough money to bet with
            printf("You do not have enough money, please enter a smaller amount. ");
            scanf("%i", &amount);
        }
    *money-= amount; //Gets rid of the bet money
    int chance = random(1,100); //Int that determines whether they won or not
    if(dogs[index].winOdds >= chance){ //Checks if the win odds of the dog are greater than or equal to the randomly generated number
    //Note that since they're percentages its on a 1 to 100 scale
    //So for example, if the win rate is 40%, any number from 1 to 40 will constitute a win
        printf("Congratulations, your gamble was successful.\n");
        *money += (amount * dogs[index].payMult); //Adds to their money the pay multiplier of their bet
        printf("Your money is now: %i\n", *money);
        didWin = true; //Shows that they won for record keeping
    }
    else{
        printf("You have unfortunately lost your gamble.\n");
        printf("Your money is now: %i\n", *money);
        didWin = false; //Shows they lost for record keeping
    }
    return didWin;
}

void getRaceResults(bool record[], int i){
    for(int j = 0; j<i; j++){ //Prints the race results
        if(record[j] == true){ //If they won, show they won
            printf("Game %i won\n", j+1);
        }
        else if(record[j] == false){ //If they lost, show they lost
            printf("Game %i lost\n", j+1);
        }
    }
}

int main(){
    DOG gabe, jake, mike, jerry, tom, lincoln, ethan, ian, bob; //Making the dogs
    createDog(&gabe, "Gabe", 2, 40);
    createDog(&jake, "Jake", 5, 10);
    createDog(&mike, "Mike", 10, 8);
    createDog(&jerry, "Jerry", 15, 6);
    createDog(&tom, "Tom", 50, 1);
    createDog(&lincoln, "Lincoln", 20, 4);
    createDog(&ethan, "Ethan", 10, 8);
    createDog(&ian, "Ian", 5, 10);
    createDog(&bob, "Bob", 3, 13);
    DOG dogs[9] = {gabe, jake, mike, jerry, tom, lincoln, ethan, ian, bob}; //Dogs array
    int bankMoney = 1000; //Greg's initial bank money
    int money = 100; //Greg's initial hand money
    char choice; //User choice for the menu
    bool didWin[20]; //Boolean array to keep track of their wins
    bool result; //Boolean variable to be added to didWin just to be safe
    int i = 0; //Index value for the didWin array
    do{
        printf("Choose an option:\n"); //Options list
        printf("Type G to bet on a puppy\n");
        printf("Type B to deposit or withdraw money\n");
        printf("Type R to get the race results\n");
        printf("Type L to quit the program\n");
        printf("Please type your choice here: ");
        scanf(" %c", &choice); //Get the character input and put it into input
        switch(choice){
        case 'G': //Gambles
            if(money<=0){ //Check if Greg has enough money on hand to bet
                printf("Greg cannot bet on any puppies. Please withdraw some money.\n");
                break;
            }
            if(i > 20){ //Check for the max number of games
                printf("Max game count reached. Cannot continue forward. \n");
                break;
            }
            result = gamble(dogs, &money);
            didWin[i] = result;
            i++; //Increases index counter by one to prevent replacing of values
            break;
        case 'B': //Deposits or withdraws money from his account
            getOrStore(&money, &bankMoney);
            break;
        case 'R': //Gets the race results
            printf("Here's the race results on whether you won or lost\n");
            getRaceResults(didWin, i);
            break;
        case 'L': //Quits the program
            break;
        default: //Any other input gets sent here and resets the switch
            printf("Your input cannot be accepted.\n");
            break;
        }
    } while(choice != 'L');
}