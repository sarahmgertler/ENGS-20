/* this file goes with packers.txt */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAXLEN 100
#define STRLEN 20

/* define player structure */
typedef struct{
	char fname[STRLEN];
	char lname[STRLEN];
	int tottackles;
	int solo;
	int assist;
	float sacks;
	int fumbles;
} player_t;

/* function to print the file info, if the file entered is either the one i provide
	or if user selects default option */
	
void fileInfo(char filename[]);

/* function to count entires in the file */
int countEntries(FILE *ifile);

/* function to fill the sort array with numbers */
void fillSort(int numfiles, int sort[]);

/* readjunk functions*/
void readJunk(FILE *ifile);

void readJunkUser(void);

/* function to read from file and fill structure array */
void fillStructArray(FILE *ifile, int numfiles, player_t structArray[], int sort[]);

/* menu function */
int menu(int numfiles, int numstart, player_t players[], int sort[], int foundArray[]);

/* scan num function - this checks if the user entered a number when they were supposed to.
	I scan input as a string, and then this function will convert it to the appropriate number.
	If default option [ENTER] is entered, this outputs 1. If the user enters nonsense,
	the output is -1. (Occasionally I convert the -1 to something else in my program) 
	Output is a float, because that can later be converted to an int as well. */
/* Note: I decided to make this function because I wanted to be able to deal with
	all types of user input cases, any time the program calls for user input. */
float scanNum(char input[]);

/* search function. Input structure array, member number being searched, sort array, and found array */
int search(int numfiles, player_t structArray[], int membernum, \
	int sort[], int foundArray[]);

/* function to display search matches */
void dispFound(int numfiles, player_t structArray[], int foundArray[], int numFound, int sort[]);

/* sort function - sorts depending on which number is input */
void sortData(int numfiles, player_t structArray[], int membernum, int sort[]);

/* display all data as currently sorted */
void dispData(int numfiles, player_t structArray[], int sort[]);

/* function to add an entry to the file */
void addEntry(int *numfiles, int *numstart, player_t structArray[], int sort[]);

/* function to delete entry */
void deleteEntry(int *numfiles, int numstart, player_t structArray[], int sort[], int foundArray[]);

/* function to save data to a file */
void saveFile(int numfiles, player_t structArray[], int sort[]);

/* MAIN FUNCTION */
	int main(void){
	int foundArray[MAXLEN], sort[MAXLEN];
	int numfiles, feedback, numstart;
	char filename[STRLEN];
	player_t players[MAXLEN];
	FILE *ifile;
	
	
	/* initialize filename in case there was already something there */
	strcpy(filename, "\0");

	/* prompt user to enter filename */
    printf("Please enter the file you wish to scan:\n");
    printf("Note: default file [ENTER] is defensive statistics for the Green Bay Packers.\n");
    printf("(Type packers.txt anyway and you will not regret it.)\n\n");
    
    /* below notation allows computer to scan spaces in a string */
    scanf("%[^\n]", filename);
    /* readJunk - read enter */
    readJunkUser();
        
	/* open file */ 
    ifile = fopen(filename, "r");

	/* if file doesn't exist, check if user selected default [ENTER]. If so, print
		out message indicating this selection. If not, print error message but assign
		default option anyway. */
    if(ifile==NULL) {
   	 	if (strcmp(filename,"\0")==0) printf("OK, going with the default option!\n\n");
	 	else printf("The file you entered does not exist! So you are in luck...\n\n");
    	strcpy(filename, "packers.txt");
    	/* open the default file */
    	ifile = fopen(filename, "r");
    }

    /* call fileinfo which prints info about packers if user enters packers.txt (or default) */    
    fileInfo(filename);
    
    /* open and close file to count how many numbers */
    ifile = fopen(filename, "r");
    
    /* assign output of countEntries, file counting players, to 2 variables. I will
    	later use and modify both in different ways throughout the program. */
    	
    /* numfiles: tracks the exact number of entries in the array,
    	so if the user added or deleted one it will go up or down. */
    	
    /*numstart: tracks the number of players ever entered into the system - 
    		it increments if the user adds a player, but it will not decrease if the user
    		deletes a player. */
    		
	numfiles = countEntries(ifile);
	numstart = numfiles;


	/* fill the sort array */
	fillSort(numfiles, sort);
	/* open file (it was closed in the countEntries function) */
	ifile = fopen(filename, "r");

	/* read from file and scan into array */
	fillStructArray(ifile, numfiles, players, sort);
	
	/* User can respond to a menu prompt after they opt to quit. With a certain 
	response they'll get the following enthusiastic message about the packers! */
	
	feedback = menu(numfiles, numstart, players, sort, foundArray);
	if(feedback==0) printf("Well, you are wrong.\n\n");
	else {	
		printf("\n\nGo pack go!\n\n");
		printf("  ___________    \n");
		printf(" /           \\   \n");
		printf("|       ______   \n");
		printf(" \\___________/   \n");
	}
	printf("\n");
	printf("Either way, glad you got a chance to learn about the greatest team in football!\n");

	fclose(ifile);

	
	return(0);
}

/* functions that prints out information about the packers file, if that was the
	file entered (or default was entered) */
void fileInfo(char filename[]){
/* print out info about file */
    if(strcmp(filename,"packers.txt")==0) {
    	printf("GREEN BAY PACKERS DEFENSE 2016-2017\n");
    	printf("-----------------------------------\n");
    	printf("This file holds defensive statistics for the Green Bay Packers 2016-7\n");
		printf("season.\n");
	    printf("If you aren't familiar with the Packers, they have won four superbowls.\n");
	    printf("This includes the first two superbowls, and the superbowl in 2011.\n");
	    printf("And one with Brett Favre too. Not to mention all the championships\n");
	    printf("BEFORE the superbowl even existed.\n");
	    printf("Sorry, I could go on for a while.\n");
	    printf("Anyway, each entry represents one player that ever made a defensive \n");
	    printf("play last season.\n");
	    printf("For each player, this file provides his (unfortunately there are no \n");
    	printf("'her's on this team)\n");
    	printf("number of total tackles, solo tackles, tackle assists, sacks \n");
		printf("(when they tackle the quarterback!), \n");
    	printf("and number of fumbles the player forced.\n");
    	printf("You will be able to sort, search, add, delete, print, save, and players.\n");
   		 printf("I hope you enjoy!\n");
   	 	printf("-----------------------------------\n\n");
    }
}

/* function that fills sort array with numbers corresponding to indices in main array */
void fillSort(int numfiles, int sort[]){
	/* loop through sort array and fill with 1-numentries*/
	int i;
	for(i=0;i<numfiles;i++){
	sort[i] = i;
	}
}

/* function that counts number of players in file */
int countEntries(FILE *ifile){
	/* loop through file and count entries*/
	/* do so by counting line breaks - same way as LA1 */
	int numplayers;
	/* declare character */
	char c;
	/* set word count to 0, so start at 0 for first word because there is a line break
	at the end of file*/
	numplayers = 1;
	/* loop through file until end */
	while(fscanf(ifile,"%c",&c)!=EOF){	
		/* check if line break, then increment */
		if (c==10){
		numplayers++;
		}
	}
	/* close file */
	fclose(ifile);
	/* return number of players */
	return(numplayers);
}

/* two readjunk functions: one for a file, and one for user input */
void readJunk(FILE *ifile){
	char c;
	fscanf(ifile, "%c", &c);
}

void readJunkUser(void){
	char c;
	scanf("%c", &c);
}

/* function that loops through file and inputs into structure array */
void fillStructArray(FILE *ifile, int numfiles, player_t structArray[], int sort[]){
/* nested loop from 0 through numfiles-1, and then scan string into
i.name, int into i.tackles, etc */
/* REMEMBER to change these var names when submitting checkin */
	int i;
	char junk;
	for(i=0; i<numfiles; i++){
	
	/* scan string or number until comma, as that is how file is organized */
	/* and readjunk after every scan to read the comma*/
	
	fscanf(ifile, "%[^,]", structArray[sort[i]].fname);
	readJunk(ifile);
	fscanf(ifile, "%[^,]", structArray[sort[i]].lname);
	readJunk(ifile);
	
	fscanf(ifile, "%d",&structArray[sort[i]].tottackles);
	readJunk(ifile);
	fscanf(ifile, "%d", &structArray[sort[i]].solo);
	readJunk(ifile);
	fscanf(ifile, "%d", &structArray[sort[i]].assist);
	readJunk(ifile);	
	fscanf(ifile, "%f", &structArray[sort[i]].sacks);
	readJunk(ifile);
	fscanf(ifile, "%d", &structArray[sort[i]].fumbles); 
	readJunk(ifile);
	}
}

/* menu function */
int menu(int numfiles, int numstart, player_t players[], int sort[], int foundArray[]){
	char again, jnk; 
	char input[STRLEN], feedback[STRLEN];
	int menuchoice, match, output;
	float searchfloat;
	again = 'y';
	do {
		printf("\n\nWelcome to the MENU for the Green Bay Packers Defensive Statistics 2016-2017.\n\n");
		printf("Here are all the options of what you can do!:\n\n");
		 printf("Sort list by:\n");
	  	printf("first (1)\n last (2)\n total tackles (3)\n solo tackles (4)\n");
	  	printf("assist tackles (5)\n sacks (6)\n fumbles (7)\n");
	 	printf("Display list (8)\n");
	 	printf("Add player (9)\n");
	 	printf("Delete player (10)\n");
	 	printf("Search list by:\n");
	 	 printf("First (11)\n Last (12)\n Total Tackles (13)\n Solo Tackles (14)\n");
	 	 printf("Assist Tackles (15)\n Sacks (16)\n Fumbles (17)\n");
	 	printf("Save List (18) \n\n");
	 	printf("Quit (0) \n\n");
	 	printf("Enter choice 0 through 18 [or press ENTER for default (1 - sort by first name)]: \n");
 		/* initialize input in case it was something else before */
		strcpy(input, "\0");
	
		 /* scan string and read junk */
		scanf("%[^\n]", input);
		readJunkUser();
	
		/* input scanned string into scanNum function (check if number, or assign default)
			and output the correct number [1 is default]. If nonsense, output error */
		searchfloat = scanNum(input);
		/* assign to integer */
		menuchoice = searchfloat; 

		/* if user entered a float, or other nonsense [besides ENTER (default)], 
			output error and force to try again.*/
		
		if (((float) menuchoice) < searchfloat) menuchoice = -1;
		
		/* if user didn't input one of the options, print an error and force them to try again */
		if (menuchoice<0||menuchoice>18) {
			printf("\n\nThat wasn't an option! Please try again.\n\n\n\n");	
			menuchoice=-1;
			again = 'y';
		}
	
		/* GO THROUGH MENU OPTIONS */
		/* have to include 2 conditionals on ranges otherwise could pick up
		 weird numbers like negatives */
		 
		 /* if user tried to quit, end loop */
		 if(menuchoice==0) again='n';
	 
	 	/* otherwise do commands based on what user input */
 	
	 	/*options 1-7 sort the data, each number is a different member of the structure */ 
		 else if(menuchoice>0&&menuchoice<=7) sortData(numfiles, players, menuchoice, sort);
		else if(menuchoice==8) dispData(numfiles, players, sort);
		/* options 9 and 10 add or delete */
		else if (menuchoice==9) addEntry(&numfiles, &numstart, players, sort);
		else if (menuchoice==10) deleteEntry(&numfiles, numstart, players, sort, foundArray);
	
		/* options 11-17 search the data, depending on which member selected */
		else if(menuchoice>10&&menuchoice<=17) {
	
			/* search function loops through array and fills a new array with all the matches.
			It outputs the number of matches */
			match = search(numfiles, players, menuchoice, sort, foundArray);
		
			/*dispFound loops through the foundArray match times, and prints */
			dispFound(numfiles, players, foundArray, match, sort);
		}
		/* if person chooses to quit in menu, then this ends do-while loop */
		else if (menuchoice==18) saveFile(numfiles, players, sort);
		
		/* give option to go again or quit before seeing menu  */
		if(menuchoice>0){
			 printf("\n\nAgain? Press 'n' to quit, 'y' to continue [y]:");
			 scanf("%c", &again);
			 if (again == '\n') again = 'y';
			 else scanf("%c", &jnk);
		}
	} while (again != 'n');


/* ask for feedback */
	/*initialize feedback */
	strcpy(feedback, "\0");
	
	printf("\nNow do you agree that the green bay packers are the best team in football?\n");
	printf("[1 YES, 0 NO]\n");
	printf("Can you guess what the default option is?\n\n");
	
	/* initialize feedback */
	
	scanf("%[^\n]", feedback);
	readJunkUser();
	
	/* initialize output */
	output = scanNum(feedback);
	return(output);
}

/* function that takes user input and figures out if it is actually a number.
	If it isn't, this function returns 1 for ENTER (default) or -1 for nonsense. */
float scanNum(char input[]){
	/* initialize vars */
	int floatdigit, hasDec, length, count, i, digit;
	/* initialize searchfloat */
	float searchfloat = 0.0;
	/* float digit is the index of the . and hasDec is a dummy for if input was a float */
	floatdigit = hasDec = 0;
	/* calculate length of input */
	length = strlen(input);
	/* return default, 1, if user presses enter */ 
	if (strcmp(input, "\0") == 0) searchfloat = 1.0;
	else { 
		/* first check if float */
		for(i=0;i<length;i++){
			/* if input has a . , indicating the input was a float */
			if (input[i] == '.'){
			/* assign floatdigit to that index */
				floatdigit = i;
			/* turn on hasDec flag */
				hasDec++;
			}
		}
		count=0;
		while(length>0){
			/* convert char to digit */
			digit = input[count]-48;
			/* have this additional check in case user enters negative, which is not possible
			with this data */
			/* also make sure 2 decimals weren't entered */
			if(digit>=0&&digit<=9&&hasDec<2){
			
				/* if no decimal multiply each digit by respective power of 10 and add */
				if(hasDec==0) searchfloat += digit*pow(10,length-1);
				
				/* if has decimal, multiply digits before . by correct amount, and digits
					after by correct amount (have to do in two expressions) */
					
					/* digits before - multiply by 10^(floatdigit index - digit's index - 1)
					digits after - multiply by 10^(floatdigit index - digit's index)
						so for example: 35.6's floatdigit index is 2. So it would be
							3*10^(2-0-1) + 5*10^(2-1-1) + 6*(2-3) = 30 + 5 + 0.6 = 35.6
							*/
							
				else if(count<floatdigit) {
					searchfloat += digit*pow(10, floatdigit-count-1);
				}
				else if(count>floatdigit) {
					searchfloat += digit*pow(10, floatdigit-count);
				}
			}
			
			/* then unless scanning a decimal, end loop and assign default */
			else if (digit!=-2) {
				length=0;
				searchfloat=-1.0;
			}
			
			/* increment position in number */
			count++;
			
			/* decrease length for integer calculation and to stop loop */
			length--;
		}
	}
	
	/* return float that was generated */
	return(searchfloat);
}

 
/* search function - take number of member and search through array to find matches */
int search(int numfiles, player_t structArray[], int membernum, \
	int sort[], int foundArray[]){
	/* initialize vars */
	int i, match, searchint, length, floatdigit, digit, dec, numdec, count;
	float searchfloat;
	char searchword[STRLEN], searchnum[STRLEN];
	char jnk;
	match=0;

	/* prompt for search term */
	/* if searching for name, prompt for string */
	if(membernum==11|membernum==12) {
		printf("\n\nEnter the name you'd like to find. Default [ENTER] is Gertler (I'm curious too). \n");
		/* initialize input to Gertler, and if user enters anything else it will write over */
		strcpy(searchword, "Gertler");
		/* scan input */
		scanf("%[^\n]", searchword);
		/* read enter*/
		readJunkUser();
		printf("Searching for %s...\n", searchword);
	}
	/* otherwise prompt for number */
	else if(membernum>=13&&membernum<=17) {
		/* initialize input incase there is something already there */
		strcpy(searchnum, "\0");
		/* prompt for search number and provide default */
		printf("\n\nEnter the number you'd like to search for. Default [ENTER] is 1. \n");
		scanf("%[^\n]", searchnum);
		readJunkUser();
		/* take output of scanNum function (check if number, or assign default which is 1) */
		searchfloat = scanNum(searchnum);
		if(searchfloat==-1) searchfloat=1;
		/* assign to integer if user was looking for an integer value */
		searchint = searchfloat;
		if(membernum==16) printf("Searching for %.2f...\n", searchfloat);
		else printf("Searching for %d...\n", searchint);
	}
	/* loop through array and check if there is an instance of member that matches 
	search term */
	for(i=0; i<numfiles; i++){
		/* use string compare - when it equals zero add to foundArray, which has the same length
		as the number of files (in case they all match search criteria */
		if(membernum==11){
			if(strcmp(searchword, structArray[sort[i]].fname)==0){
				/* assign element in found array to index of match */
				foundArray[match] = sort[i];
				/* increment index of found array, which is also count of matches */
				match++;
			}
		}
		else if(membernum==12){
			if(strcmp(searchword, structArray[sort[i]].lname)==0){
				/* assign element in found array to index of match */
				foundArray[match] = sort[i];
				/* increment index of found array, which is also count of matches */
				match++;
			}
		}
		/* use number compare for the rest */
		else if(membernum==13){
			if(searchint==structArray[sort[i]].tottackles){
				/* assign element in found array to index of match */
				foundArray[match] = sort[i];
				/* increment index of found array, which is also count of matches */
				match++;
			}
		}
		else if (membernum==14){
			if(searchint==structArray[sort[i]].solo){
				/* assign element in found array to index of match */
				foundArray[match] = sort[i];
				/* increment index of found array, which is also count of matches */
				match++;
			}
		}
		else if (membernum==15){
			if(searchint==structArray[sort[i]].assist){
				/* assign element in found array to index of match */
				foundArray[match] = sort[i];
				/* increment index of found array, which is also count of matches */
				match++;
			}
		}
		else if (membernum==16){
			if(searchfloat==structArray[sort[i]].sacks){
				/* assign element in found array to index of match */
				foundArray[match] = sort[i];
				/* increment index of found array, which is also count of matches */
				match++;
			}
		}
		else if (membernum==17){
			if(searchint==structArray[sort[i]].fumbles){
				/* assign element in found array to index of match */
				foundArray[match] = sort[i];
				/* increment index of found array, which is also count of matches */
				match++;
			}
		}
	}
	/* return count of matches (1 less than total) so dispFound knows how many to display */
	return(match);
}

/* function that loops through foundArray the amount of times that there was a match 
	and displays each player */
void dispFound(int numfiles, player_t structArray[], int foundArray[], int numFound, int sort[]){
	/* initialize vars */
	int i;
	/* print out found statement */
	printf("There are %d entries which match your search criteria.\n", numFound);
	/* loop through found array until hit numFound-1 index */
	/* in loop, print structure of indexes that were found (the entries
	of the found array) */
	for(i=0; i<numFound; i++){
		printf("%d. First: %s Last: %s Total Tackles: %d Solo: %d Assist: %d Sacks: %.2f ", \
		i+1, structArray[foundArray[i]].fname, structArray[foundArray[i]].lname, structArray[foundArray[i]].tottackles, \
		structArray[foundArray[i]].solo, structArray[foundArray[i]].assist, structArray[foundArray[i]].sacks);
		printf("Fumbles: %d\n", structArray[foundArray[i]].fumbles);
	}
}

/* function that sorts the data depending on how the user requested */
/* bubble sort using sort array and sorting on specific member (using string
	compare) */
void sortData(int numfiles, player_t structArray[], int membernum, int sort[]){
	/* initialize counters and placeholder */
	int i, j;
	int placeholder;
	/*loop through array and bubble sort */
	/* string compare for strings */
	if(membernum==1){
		for(i=0; i<numfiles-1; i++){
			for(j=0; j<numfiles-1; j++){
				if (strcmp(structArray[sort[j]].fname, structArray[sort[j+1]].fname) > 0){
					placeholder = sort[j];
					sort[j] = sort[j+1];
					sort[j+1] = placeholder;
				}
			}
		}
	}
	else if(membernum==2){
		for(i=0; i<numfiles-1; i++){
			for(j=0; j<numfiles-1; j++){
				if (strcmp(structArray[sort[j]].lname, structArray[sort[j+1]].lname) > 0){
					placeholder = sort[j];
					sort[j] = sort[j+1];
					sort[j+1] = placeholder;
				}
			}
		}
	}
	
	/* num compare for numbers */
	else if(membernum==3){
		for(i=0; i<numfiles-1; i++){
			for(j=0; j<numfiles-1; j++){
				if (structArray[sort[j]].tottackles<structArray[sort[j+1]].tottackles){
					placeholder = sort[j];
					sort[j] = sort[j+1];
					sort[j+1] = placeholder;
				}
			}
		}
	}
	else if (membernum==4) {
		for(i=0; i<numfiles-1; i++){
			for(j=0; j<numfiles-1; j++){
				if (structArray[sort[j]].solo< structArray[sort[j+1]].solo){
					placeholder = sort[j];
					sort[j] = sort[j+1];
					sort[j+1] = placeholder;
				}
			}
		}
	}
	else if (membernum==5) {
		for(i=0; i<numfiles-1; i++){
			for(j=0; j<numfiles-1; j++){
				if (structArray[sort[j]].assist< structArray[sort[j+1]].assist){
					placeholder = sort[j];
					sort[j] = sort[j+1];
					sort[j+1] = placeholder;
				}
			}
		}
	}
	else if (membernum==6) {
		for(i=0; i<numfiles-1; i++){
			for(j=0; j<numfiles-1; j++){
				if (structArray[sort[j]].sacks< structArray[sort[j+1]].sacks){
					placeholder = sort[j];
					sort[j] = sort[j+1];
					sort[j+1] = placeholder;
				}
			}
		}
	}
	else if (membernum==7) {
		for(i=0; i<numfiles-1; i++){
			for(j=0; j<numfiles-1; j++){
				if (structArray[sort[j]].fumbles< structArray[sort[j+1]].fumbles){
					placeholder = sort[j];
					sort[j] = sort[j+1];
					sort[j+1] = placeholder;
				}
			}
		}
	}
}

/* function that loops through data and displays */
void dispData(int numfiles, player_t structArray[], int sort[]){
/* loop through structArray and print out each member for each structure in the array */
	int i;
	for	(i=0; i<numfiles; i++){
		printf("%d. First: %s Last: %s Total Tackles: %d Solo: %d Assist: %d Sacks: %.2f ", \
			i+1, structArray[sort[i]].fname, structArray[sort[i]].lname, structArray[sort[i]].tottackles, \
			structArray[sort[i]].solo, structArray[sort[i]].assist, structArray[sort[i]].sacks);
		printf("Fumbles: %d\n", structArray[sort[i]].fumbles);
	}
}

/* function to add a player to the team */
void addEntry(int *numfiles, int *startsize, player_t structArray[], int sort[]){
/* create dummy integers and floats that are outputs of function that checks if user input a number */
	int intInput;
	float floatInput;
	char strInput[STRLEN];

/* prompt user for info and scan */
	printf("%d\n", *numfiles);
	printf("Please enter the following information for the player:\n");
	printf("Note 1: default for name [enter] is no name. \n");
	printf("Note 2: default for stats is 0");
	printf("Note 2: If you enter a non-integer for tackles (total, solo, or assist) or fumbles, it is assumed\n");
	printf("that you are accounting for non-calls.\n");
	printf("Even though you are probably right, we will unfortunately have to round down. These stats are official.\n");
	
	printf("First name: \n");
	
	/* scan input until enter. input to name. read enter. If user enters nothing, leave name blank (default) */
		
	scanf("%[^\n]", structArray[*startsize].fname);
	readJunkUser();
	
	printf("Last name: \n");
	scanf("%[^\n]", structArray[*startsize].lname);
	readJunkUser();
	
	/* scan input. then input into scanNum function which detects if what the user
		input was a number. Then convert to int or float depending on member type.
			Then assign to member. If the user entered nonsense or pressed enter only, set to 0 (default). */
	
	printf("Total tackles: \n");
	scanf("%[^\n]", strInput);
	readJunkUser();
	floatInput = scanNum(strInput);
	intInput = floatInput;
	if(intInput==-1) intInput= 0;
	structArray[*startsize].tottackles = intInput;
	
	printf("Solo tackles: \n");
	scanf("%[^\n]", strInput);
	readJunkUser();
	floatInput = scanNum(strInput);
	intInput = floatInput;
	if(intInput==-1) intInput= 0;
	structArray[*startsize].solo = intInput;

	printf("Tackle assists: \n");
	scanf("%[^\n]", strInput);
	readJunkUser();
	floatInput = scanNum(strInput);
	intInput = floatInput;
	if(intInput==-1) intInput=0;
	structArray[*startsize].assist = intInput;

	printf("Sacks: \n");
	scanf("%[^\n]", strInput);
	readJunkUser();
	floatInput = scanNum(strInput);
	if(floatInput==-1) floatInput = 0;
	structArray[*startsize].sacks = floatInput;

	printf("Forced fumbles: \n");
	scanf("%[^\n]", strInput);
	readJunkUser();
	floatInput = scanNum(strInput);
	intInput = floatInput;
	if(intInput==-1) intInput=0;
	structArray[*startsize].fumbles = intInput;

	
	/* increment sort array using pointers */
	sort[*numfiles] = *startsize;

	/* increment numfiles using pointers */
	*numfiles = *numfiles + 1;
	*startsize = *startsize + 1;

}

/* function that deletes every entry that matches user's search */
void deleteEntry(int *numfiles, int startsize, player_t structArray[], int sort[], int foundArray[])
{
	int match, i, j, found, choice;
	char menuchoice;
	char input[STRLEN];
	
	
	/* generate integer that holds pointer value so that when it is passed to functions
	it is not modified */
	
	int currentnumfiles = *numfiles;

	/* prompt user for name of entry it wants deleted */
	printf("What do you want to delete by?\n");
	printf("First name: 1\n");
	printf("Last name: 2\n");
	printf("Total tackles: 3\n");
	printf("Solo tackles: 4\n");
	printf("Assist tackles: 5\n");
	printf("Sacks: 6\n");
	printf("Fumbles: 7\n");
	printf("No! I don't want to delete anything [default - ENTER]: 8\n");
	
	/* scan until line break */
	scanf("%[^\n]", input);
	readJunkUser();
	
	/* set default to 8 */
	if(strcmp(input,"\0")==0) choice=8;
	else {
		/* use scanNum function */
		choice = scanNum(input);
	}
	/* initialize match to 0 in case no matches or decided not to delete */
	match = 0;
	
	/* add 10 so matches up with choices in search function */
	choice = choice+10;

	if(choice>=11&&choice<=17){
		/* search and fill foundArray same way as when searching */
		match = search(currentnumfiles, structArray, choice, sort, foundArray);
		dispFound(currentnumfiles, structArray, foundArray, match, sort);
		if(match>0) printf("Deleted!\n");
	}
	/* other message - if they chose not to delete */
	else printf("I completely understand.\n");

	/*decrement numfiles and int that holds numfiles value */
	currentnumfiles -= match;


	for(i=0; i<match; i++){
	
		/* empty entry */
		strcpy(structArray[foundArray[i]].fname, "\0");
		strcpy(structArray[foundArray[i]].lname, "\0");
		structArray[foundArray[i]].tottackles = 0;
		structArray[foundArray[i]].solo = 0;
		structArray[foundArray[i]].assist = 0;
		structArray[foundArray[i]].sacks = 0.0;
		structArray[foundArray[i]].fumbles = 0;
		
		/* found is a flag for when the index of the player to be deleted is found */
		found=0;
		/* move all subsequent entries backward once you hit the number that matches the one to be deleted */
		for(j=0; j<*numfiles; j++){
			/* turn on flag when find player */ 
			if (sort[j] == foundArray[i]){
				found=1;
			}
			/* all the subsequent players should have their indices shifted back */
			if(found){
				sort[j] = sort[j+1];
			}
		}
	}	
	
	/* adjust numfiles pointer by number of matches because that is number
		that will be deleted */
	*numfiles= *numfiles-match;

}

/* function that saves array as it currently is to a file */
void saveFile(int numfiles, player_t structArray[], int sort[]){
	/* initialize vars */
	int i, done;
	char input[STRLEN], filename[STRLEN];
	char ans;
	FILE *ifile, *ofile;
	/* initialize done to enter loop */
	done = 0;
	/* stop loop if file doesnt exist or if user wants to write over */
	while(!done){
		/* prompt user for a file in loop until enters a unique filename or agrees to write over */
		printf("Please enter a file name that you wish to write to\n");
		printf("(the filename can't have spaces): \n");
		scanf("%[^\n]", filename);
		readJunkUser();
		ifile = fopen(filename, "r");
		/* check if that file exists -if it does, ask for a new one*/
		if(ifile!=NULL){
			fclose(ifile);
			printf("That file already exists. Do you want to write over it? y/n: \n");
			scanf("%c", &ans);
			readJunkUser();
			if(ans==121) done = 1;
			else if (ans==110){
				printf("OK, I won't write over it.\n");
			}
			else {
				printf("Invalid answer! I won't write over your file.\n");
			}
		}
		else done=1;
	}
	/* open file for writing */
	ofile = fopen(filename, "w");
	/* loop through struct array and print players into file in same format
		as they were in the original file */
	for(i=0; i<numfiles; i++){
		fprintf(ofile, "%s,%s,%d,%d,%d,%.2f,%d\n", structArray[sort[i]].fname, \
		structArray[sort[i]].lname, structArray[sort[i]].tottackles, \
		structArray[sort[i]].solo, structArray[sort[i]].assist, structArray[sort[i]].sacks,\
		structArray[sort[i]].fumbles);
	}
	/* close ofile*/
	fclose(ofile);

}
