/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Recorder
* Mary Slaven, Monitor
*
* Description:
* This program will perform the following tasks based on user input:
* a.) Create a binary file to store info about up to 100 flights from a txt file (i.e., ”flights.bin”).
* b.) Read input from user until they exit out of the input loop.
* c.) Read and print info about all flight records in the file.
* d.) Count number of airlines for a specified airport.
* e.) Print the number of inbound flights for a specified airport.
* f.) Print the number of inbound flights for each airport.
* g.) Print a sorted list of origin airports based on the number of outbound flights.
* h.) Print a list of origin airports that have at least 2 flights that have a departure time earlier than noon.
*
* Assumptions:
* - Each flight record will be inputted by the user in the form of: 
* AirlineCode FlightNumber OriginAirportCode DestinationAirportCode DepartureDate (e.g., AA43 DFW DTW Wed Jan 6 11:00 2016).
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "flightRec.h"

int main()
{
    char choice;
    char *token;
	char input[5];
    char fNum[10];
	char fNumBak[10];
	char alphas[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char nums[] = {"0123456789"};
	
	flyRec f;

    // Read input from user (i.e., create a menu)
    printf("Enter a corresponding letter to make a selection.\n");
    printf("Choose an option:\n (a)Input flight(s)  \n (b)Print all flight data \n (c)Print flight count for an airport \n");
    printf(" (d)Print number of inbound flights for an airport \n (e)Print number of inbound flights for each airport \n");
    printf(" (f)Print list of origin airports by number of outbound flights\n (g)Print list of origin airports with at least 2 flights before noon.\n");
    printf(" (h) Exit.\n");
	
    scanf("%c",&choice);
    getc(stdin);

	do
	{
		// Read info
		if(choice == 'a')
		{
			printf("Enter the AirlineCode, FlightNumber, OriginAirportCode, DestinationAirportCode, and DepartureDate in the following format:\n");
			printf("AA43 DFW DTW Wed Jan 6 11:00 2016.\n");
				
			scanf("%s %s %s %s %s %d %s %d", fNum, f.orgCode, f.destCode, f.dayOfWeek, f.month, &f.day,f.time, &f.year); // Get input
				
			strcpy(fNumBak, fNum);	// Create backup of aircode
				
			token = strtok(fNum, nums);
			strcpy(f.airCode, token);
				
			// The problem here is that the 1st number in the air code is not included for some reason when using strtok() again. So I made a backup of the original string and tokenized that using capital letters as the delimeters.
			token = strtok(fNumBak, alphas);
			strcpy(f.flyNum, token);
			insertFile(f);
		}
		else if(choice == 'b') // Print info about all flight records in the file
		{
			if (fileSize(FILENAME) != 0)
			{
				printInfo(); // <-------------- This is not working properly call it more than once and you will get errors!!!!!!
			}
			else
			{
				fprintf(stderr, "Error: There are no flight records in %s\n", FILENAME);
			}
			
		}
		else if(choice == 'c') // Count number of airlines for a given airport
		{
		    printf("Enter an airport (Ex. DFW):\n");
		    scanf("%s",input);
		    numForAirport(input);
		}
		else if(choice == 'd') // Print the number of inbound flights for airport x
		{
            printf("Enter destination airport code: ");
            scanf("%s", input);
            printf("\nThere are %d flights going to %s.\n",inBound(input), input);
		}
		else if(choice == 'e')// Print the number of inbound flights for each airport
		{
            inBoundAll();
		}
		else if(choice == 'f') // Print a sorted list of origin airports based on the number of outbound flights
		{
            printSortOrg();
		}
		else if(choice == 'g') // Print a list of origin airports that have at least 2 flights that have a departure time earlier than noon
		{
			printOrgMorn();
		}
		else
		{
			fprintf(stderr, "Error: Wrong input.\nPlease enter one of the options!\n");
		}
		
        printf("\nEnter a corresponding letter to make a selection.\n");
        printf("Choose an option:\n (a)Input flight(s)  \n (b)Print all flight data \n (c)Print flight count for an airport \n");
        printf(" (d)Print number of inbound flights for an airport \n (e)Print number of inbound flights for each airport \n");
        printf(" (f)Print list of origin airports by number of outbound flights\n (g)Print list of origin airports with at least 2 flights before noon.\n");
        printf(" (h) Exit.\n");
		
		printf("Make another choice: \n");
		scanf(" %c",&choice);
		getc(stdin);
		
	}while(choice != 'h');
	
	return 0;
}
