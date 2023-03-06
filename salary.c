#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//Ian De Leon 1946676
////I affirm that this is 100% my own work
////This program uses command line arguments to take in input
////Then it uses those inputs(bonus percentage, hours worked, hourly rate) to calculate the salary.
int debug = 0;

int
main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int index = 0;
	int pflag=0, hflag=0, bflag = 0;
   	int bonus, hoursWorked;
	double salary = 0.0, hourlyRate = 0.0;
	static char usage[] = "usage: %s [-p percent] -h hoursWorked [-b bonus] hourlyRate\n";
	
	//Deals with the different command line arguments via getopt
   	//-p is the percent, -h is hours worked, -b is hourly rate
   	while ((c = getopt(argc, argv, "ph:b:")) != -1)
		switch (c) {
		case 'p':
            	//p flag will be used as a boolean to check if percent needs to be calculated
			pflag =1;
			break;	
		case 'h':
			hflag =1;
            hoursWorked = atoi(optarg);
			break;	
		case 'b':
			bflag =1;
            bonus = atoi(optarg);
			break;	                        
		case '?':
			err = 1;
			break;
		}
    	//Checks to see if the h parameter has been provided
	if (hflag == 0 ) {	
		printf("Hours worked is mandatory please use -h 'hours worked goes here'\n");
		exit(1);
	} 
   	else if ((optind+1) > argc) {	
		/* need at least one argument (change +1 to +2 for two, etc. as needeed) */

		printf("optind = %d, argc=%d\n", optind, argc);
		fprintf(stderr, "%s: missing hourly rate\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} 
    	else if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	else{
		//Gets hourly rate from argv 
		hourlyRate = atof(argv[optind]);
		for (index = optind; index < argc; index++){
			hourlyRate = atof(argv[index]);
		}
	}
	//Checks if the person worked more than 80 hours, then exits
	if(hoursWorked > 80){
		printf("Hours cannot be more than 80\n");
		return(1);
	}
    	//The logic of the program starts here//
    	salary = hourlyRate * hoursWorked;
    	//Checks if there is a percentage increase, increases salary by ten percent if there is
    	if(pflag == 1){
        	salary *= 1.1;
    	}
    	//bflag checks if there is a bonus
    	if(bflag ==1){
	  	// ensures bonus does not exceed 50% of salary
        	if(bonus < salary * .5){
			salary += bonus;
		}
		else{
			printf("Salary bonus cannot exceed 50%% of the salary\n");
			return (1);
	}
    }
	printf("The salary for this period is $%.2f\n", salary);
	
	exit(0);
}
