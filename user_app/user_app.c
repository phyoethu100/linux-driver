/**************************************************************
* Description: This program is a simple user application for the 
* calculator driver.
**************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define IOCTL_READ _IOR('a', 'a', int *)
#define IOCTL_WRITE _IOW('a', 'b', int *)

int main() 
{
    printf("Opening the driver... \n");
    printf("\nWelcome to the calculator application! \n\n");

    printf("Rule: The calculator only accepts integer values, no decimals allowed! \n");
   
    int fd, value1, value2, result, operator;
    char *symbol;
    char *filename = "/dev/myDriver"; 

    fd = open(filename, O_RDWR); // Open the device file

    if (fd < 0)
    {
        printf("File descriptor: %d \n", fd);
        printf("Cannot open the device file... \n");
        return -1;
    }

    printf("\nOperations to perform calculations: \n");
    printf("\t1. Addition\n \t2. Subtraction\n \t3. Multiplication\n \t4. Division\n");

    // User input for the operator
    printf("\nEnter the number between 1 and 4 for operation: ");
    scanf("%d", &operator);

    // Exit the program if the operator is not between 1 and 4
    if (operator < 1 || operator > 4)
    {
        printf("Please enter the number between 1 and 4 only... \n");
        return 0;
    }

    // Assign the symbol and print out operator name 
    if (operator == 1) 
    {
        printf("You have chosen addition.\n");
        symbol = "+";
    }
    else if (operator == 2)
    {
        printf("You have chosen subtraction.\n");
        symbol = "-";
    }
    else if (operator == 3)
    {
        printf("You have chosen multiplication.\n");
        symbol = "x";
    }
    else if (operator == 4)
    {
        printf("You have chosen division.\n");
        symbol = "/";
    }

    // Write the operator value to the driver
    ioctl(fd, IOCTL_WRITE, (int *) &operator);

    // User input for first value
    printf("\nEnter first value: ");
    scanf("%d", &value1);

    // User input for second value
    printf("Enter second value: ");
    scanf("%d", &value2);

    // Write 2 values to the driver
    ioctl(fd, IOCTL_WRITE, (int *) &value1);
    ioctl(fd, IOCTL_WRITE, (int *) &value2);

    // Read the result from the driver 
    ioctl(fd, IOCTL_READ, &result);
    printf("\n%d %s %d = %d\n", value1, symbol, value2, result);
    printf("\nThe result is: %d \n\n", result);

    // ASCII calculator that will display the result 
    printf(" _____________________\n");
    printf("|  _________________  |\n");
    printf("| |            %d   | |\n", result);
    printf("| |_________________| |\n");
    printf("|  ___ ___ ___   ___  |\n");
    printf("| | 7 | 8 | 9 | | + | |\n");
    printf("| |___|___|___| |___| |\n");
    printf("| | 4 | 5 | 6 | | - | |\n");
    printf("| |___|___|___| |___| |\n");
    printf("| | 1 | 2 | 3 | | x | |\n");
    printf("| |___|___|___| |___| |\n");
    printf("| | . | 0 | = | | / | |\n");
    printf("| |___|___|___| |___| |\n");
    printf("|_____________________|\n");

    printf("\nClosing the driver... \n");
    close(fd);
    
    return 0;
}
