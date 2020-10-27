/*
Program:	    roman.c
Author:         Priscilla M. Peron
Date:           Jan. 18, 2020
Description:    This program takes input of Roman or Arabic numbers from the user
        and converts the numbers in the other format (Roman to Arabic or vice versa).
*/

#include <stdio.h>
#include <stdBool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define USER_INPUT_SIZE 50

char roman[] = { 'M', 'D', 'C', 'L', 'X', 'V', 'I' };
double arabic[] = { 1000.0, 500.0, 100.0, 50.0, 10.0, 5.0, 1.0 };

/*
* Function:		arabicToRoman
* Purpose:		To read the user's input and convert it from arabic to roman numbers when called in the main method
* Accepts:		A float array named val (user's input converted to float)
* Returns:		Nothing
*/
void arabicToRoman(double* val) {
    for (int i = 0; sizeof(arabic) / sizeof(arabic[0]) > i; i++)
    {
        // Declare the variables that divide the value inputed by the user to the index of arabic numbers array
        double result = (double)*val / (double)arabic[i];
        int quotient = (int)result;

        // Declare a variable that calculates the remaining percentual of the division
        double remPerc = (fmod(*val, arabic[i]) / arabic[i]);

        // If the quotient of the first division is between 0 and 3 than the number will be regularly 
        // checked for the correspondent index of the romans number array and printed
        if (quotient > 0 && quotient <= 3)
        {
            for (int j = 0; quotient > j; j++) 
            {
                printf("%c", roman[i]);
                *val = *val - arabic[i];
            }
        }
        else if (quotient > 3) 
        {
            // Cases greater than 3999
            double q = quotient;
            arabicToRoman(&q);
            *val = *val - (int)(quotient * arabic[i]);
            printf("-");
        }
        // Cases like IX, XC, CM: the remaining percentual will be greater than 0.9 AND the division 
        // beteen the index in arabic array and index +1 in the arabic array equals two 
        if (remPerc >= 0.9 && arabic[i] / arabic[i + 1] == 2) 
        {
            printf("%c%c", roman[i + 2], roman[i]);
            *val = *val - (arabic[i] - arabic[i + 2]);
        }
        // Cases like IV, XL, CD:the remaining percentual will be greater than 0.8 AND the division 
        // beteen the index in arabic array and index +1 in the arabic array equals five 
        else if (remPerc >= 0.8 && arabic[i] / arabic[i + 1] == 5) 
        {
            printf("%c%c", roman[i + 1], roman[i]);
            *val = *val - (arabic[i] - arabic[i + 1]);
        }
        else if (*val < 1.0)
        {
            // Print S if the remaining value is 0.5 and if it's less than 0.5 but still greater than 0 prints a .
            if (*val >= 0.5)
            {
                printf("S");
                *val -= 0.5;
            }
            while (*val < 0.5 && *val >= 1.0 / 12)
            {
                printf(".");
                *val -= (1.0 / 12);
            }
        }
    }
}

/*
* Function:		romanToArabic
* Purpose:		To read the user's input and convert it from roman to arabic numbers when called in the main method
* Accepts:		A char array named v (user's input converted to uppercase letters)
* Returns:		Nothing
*/
void romanToArabic(char v[])
{
    // Declare a variable to store the converted value obtained from user's input, 
    // a variable to count the times brackets appear in the input to be the exponent of Apostrophus system
    // and a variable to hold the value of the first indexof the user's input in case there's a parenthesis
    double n = 0.0;
    int npar = -1, indOne = -1;
    for (unsigned int i = 0; i < strlen(v); i++)
    {
        // if there's a parenthesis, assign the value of I to the variable that counts them
        if (v[i] == ')')
        {
            npar = i;
            // if there's a parenthesis and the previous element is 'I', assign 
            // the value i - 1 to the variable that keeps track of the first 'I' 
            if (i != 0 && v[i - 1] == 'I')
                indOne = i - 1;
        }
    } 
    if (npar >= 0 && indOne >= 0)
    {
        // checks if the index of the first 'I' times two is equal to the number of parenthesis and executes  
        // the balanced Apostrophus system by adding 10 to the power of parenthesis + 1 to the result
        if (indOne * 2 == npar)
            n += pow(10, (double)npar + 1);
        // checks if the index of the first 'I' times two is less than the number of parenthesis and executes  
        // the unbalanced Apostrophus system by adding 10 to the power of parenthesis + 1 to the result
        // and 50 times 10 to the power of the number of extra parenthesis after the beginning of unbalancing
        else if (indOne * 2 < npar && indOne != 0)
        {
            n += pow(10, (double)indOne * 2 + 1);
            n += 50 * pow(10, (double)npar - (double)indOne * 2);
        }
        // if there's no 'C' before 'I' add 50 times 10 to the power of the number of extra parenthesis after 'I'
        else
            n += 50 * pow(10, (double)npar - (double)indOne * 2);
    }

    // for loop to check the user's input by index
    for (unsigned int i = npar +1; i < strlen(v); i++)
    {
        // for loop to check the roman numerals array by index
        for (int j = 0; j < USER_INPUT_SIZE; j++)
        {
            // Check if the letter inputed by the user matches one of the roman letters in the roman array
            if (v[i] == roman[j])
            {
                // Check for 2, 4 and 6 - the indexes that the roman numbers in the array will be sucbtracted
                // if they are less than the following roman number provided by the user
                if ((j == 2 || j == 4 || j == 6) && i != strlen(v) - 1)
                {
                    // if statement that checks if the index v+1 from user and roman j-1 from the array
                    // and adds the equivalent index from the roman array minus the next index
                    if (v[i + 1] == roman[j - 1])
                    {
                        n += arabic[j - 1] - arabic[j];
                        i++;
                        break;
                    }
                    // if statement that checks if the index v+1 from user and roman j-2 from the array
                    // and adds the equivalent index from the roman array minus the current index
                    else if (v[i + 1] == roman[j - 2])
                    {
                        n += arabic[j - 2] - arabic[j];
                        i++;
                        break;
                    }
                    // if the index inputed by the user doesn't fit the cases of a preceding smaller number
                    // just add the equivalent number from the arabic array
                    else
                    {
                        n += arabic[j];
                        break;
                    }
                }
                // if the index inputed by the user doesn't fit the cases of being on indexes 2, 4 or 6
                // just add the equivalent number from the arabic array
                else
                {
                    n += arabic[j];
                    break;
                }
            }
            // if the user inputs - or _, multiply the current number by 1000
            else if (v[i] == '-' || v[i] == '_')
            {
                n *= 1000;
                break;
            }
            // if the user inputs ans S, add 0.5 to the n variable
            else if (v[i] == 'S')
            {
                n += 0.5;
                break;
            }
            // if the user inputs a ., add 1/12.0 to the n variable
            else if (v[i] == '.')
            {
                n += 1 / 12.0;
                break;
            }
            // if the user inputs another letter that's not a roman numeral, just ignore the case
            else
                continue;
        }
    }
    printf("%.6g", n);
}

int main()
{
    // Declare a variable to store the number typed by the user
    char number[USER_INPUT_SIZE] = { 0 };

    // Declare a float variable to store the converted user's input
    double n;

    while(true)
    {
        // Print and introduction and ask the user for a number to convert
        printf("Roman/Arabic numbers converter \nPlease enter your Roman or Arabic number: ");
        scanf_s("%s", number, USER_INPUT_SIZE);

        // Convert the user's input to uppercase letters
        for (unsigned int i = 0; i < strlen(number); i++)
            number[i] = toupper(number[i]);

        // Quits the program if the user types quit
        if (strcmp(number, "QUIT") == 0)
            break;

        // Print the beginning of the answer
        printf("%s is ", number);

        // Reset the numbers of letters and numbers inputed by the user everytime the loop begins again
        int numberN = 0;
        int numberL = 0;

        // Increment the number of letters or numbers according to the user's input
        for (int i = 0; number[i] != '\0'; i++)
        {
            if (isdigit(number[i]))
                numberN++;
            else
                numberL++;
        }
        // Call the functions according to the type of input made by the user
        if (numberL < numberN)
        {
            // Convert the number inputed by the user to int type
            n = atof(number);
            arabicToRoman(&n);
            printf("\n");
        }
        else if (numberL > numberN)
        {
            romanToArabic(number);
            printf("\n");
        }
    }
    // Print a message if the user quits the program
    printf("Thank you!\n");
    return 0;
}