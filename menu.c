//
// Created by csu on 9/21/2018.
//

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>

int main(void) {
    int option = 0;
    while (option != '4') {

        printf("\n\tMENU DEMONSTRATION");
        printf("\n\t------------------------------");
        printf("\n\n\t 1. TEST PROGRAM");
        printf("\n\t 2. HELP");
        printf("\n\t 3. RUN PROGRAM");
        printf("\n\t 4. EXIT");
        printf("\n\n Enter Your Choice: ");
        option = getche();
        switch (option) {
            case '1':
                printf("\n\nYOU SELECTED OPTION 1:TEST PROGRAM\n");
                break;
            case '2':
                printf("\n\nYOU SELECTED OPTION 2: HELP COMMAND\n");
                break;
            case '3':
                printf("\n\nYOU SELECTED OPTION 3:RUN PROGRAM\n");
                break;
            case '4':
                printf("\n\nYOU SELECTED OPTION 4:EXIT\n");
                break;
            default:
                printf("\n\nINVALID SELECTION...Please try again\n");
        }
        (void) getch();
    }
    return 0;
}