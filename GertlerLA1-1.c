#include <stdio.h>

/* wordcount is a function that counts the number of words in a file */
int wordcount(void);

/* lettershift is a function that takes a character and shifts it by the correct amount */
char lettershift(char c, int shift);

/* upper2lower is a function that converts an uppercase char to a lowercase char */
char upper2lower(char c);

/* whichword is a function that prompts the user to pick a word */
int whichword(int wordtotal);

/* linejumper is used to open file and get to word selected by user */
void linejumper(FILE *ifile, int wordnum);

/* lettercount counts the letters in the selected word */
int lettercount(int wordnum);

/* decrypt takes the word, unscrambles it, and then stores the letters */
void decrypt(int wordnum, int numletters, char *letter1, char *letter2, char *letter3,\
 char *letter4, char *letter5, char *letter6, char *letter7, char *letter8, \
  char *letter9, char *letter10);

/* game prints number of letters and then starts the traditional hangman game. */
void game(int numletters, char letter1, char letter2, char letter3, char letter4, \
 char letter5, char letter6, char letter7, char letter8, char letter9, char letter10);

int main(void){
	/* wordtotal holds number of words, wordnum holds word number selected, and
		num letters holds the number of letters in the word selected */
	int wordtotal, wordnum, numletters;
	
	/* 10 variables to hold each letter */
	char letter1, letter2, letter3, letter4, letter5, \
	 letter6, letter7, letter8, letter9, letter10;

	/* declare and open file to test. If it doesn't exist, program can't run, so return -1*/
	FILE *ifile;
	ifile = fopen("wordbank.txt", "r");
	if (ifile==NULL){
		printf("Error. I can't think of any words!\n");
		return(-1);
	}
	else {
		fclose(ifile);
	}
	
	/* assign wordtotal variable to wordcount output (function that counts number of words
		in the file). need to use wordtotal var later */
	wordtotal = wordcount();

	/* print out number of words */ 
	printf("There are %d words in the file.\n", wordtotal);
	
	/* assign word number to output of whichword,
		a function that prompts user to pick word */
	wordnum = whichword(wordtotal);	
	
	/* input lettercount function into numletters variable */
	numletters = lettercount(wordnum);
		
	/* initialize letters  */
	letter1 = letter2 = letter3 = letter4 = letter5 = \
		letter6 = letter7 = letter8 = letter9 = letter10 = 0;

	/* decrypt word, input letter address for modifying */
	decrypt(wordnum, numletters, &letter1, &letter2, &letter3, &letter4, &letter5, \
		&letter6, &letter7, &letter8, &letter9, &letter10);
	
	/* call game function to prompt for guess, check if matches letters, and print game.
		It does this until  win */
	game(numletters, letter1, letter2, letter3, letter4, letter5, letter6, letter7, \
		letter8, letter9, letter10);	
	
	return(0);
}

/* this function goes through the file and counts how many words */
int wordcount(void){
	/* declare numwords which is word count, filler to check each var */
	int wordtotal;
	
	/* declare character */
	char c;
	
	/* declare file */
	FILE *ifile;
		
	/* open file for reading */
	ifile = fopen("wordbank.txt", "r");
	
	/* set word count to 1, so start at 1 for first word */
	wordtotal = 1;
	
	/* loop through file until end */
	while(fscanf(ifile,"%c",&c)!=EOF){
		/* check if line break, then increment */
		if (c==10){
			wordtotal++;
		}
	}
	
	/* close file */
	
	fclose(ifile);
	
	/* return value */
	
	return(wordtotal);
	
}

char lettershift(char c, int shift) {
	/* shift c by shift */ 
	char newletter;
	/*printf("lettershift check");*/
	newletter = c-shift;
	/* loop back if past 122, which is z */
	while(newletter<97) {
		/* subtract difference between 97 and newchar */
		/* add 123, which gets you to the end of the alphabet */
		newletter = 123-(97-newletter);
	}
	return(newletter);
}
	
char upper2lower(char c) {
	/* if letter is in fact uppercase - ASCII 65-90 - add 32 */
	/* this will convert it to lowercase, which are 97-122 */
	if (c<=90&&c>=65){
		c += 32;
	}
	/* return possibly updated value */
	return(c);
}
	
int whichword(int wordtotal){
	/* initialize word number variable */
	int wordnum;
	/* print prompt asking user which word he/she wants to guess */
	printf("Please enter which word you would like to play: \n");
	/* scan to word number variable */
	scanf("%d", &wordnum);
	/* If number not in range, assign a number in correct range */
	if(wordnum<1||wordnum>wordtotal){
		/* assign to last word */
		wordnum = wordtotal;
		printf("Invalid input. I'll choose word number %d.\n", wordnum); 
	}
	/* return word number */
	return(wordnum);
}

int lettercount(int wordnum){
	/* initialize numletters (output), endflag (to end counting), filler
		to scan number */
	int numletters, endflag, filler;
	/* initialize placeholder for characters scanned */
	char placeholder;
	/* initialize file pointer */
	FILE *ifile;
	/* open file */
	ifile = fopen("wordbank.txt", "r");
	/* initialize lettercount to 1 */
	numletters = 0;
	/* initialize end flag, a flag for done with word, to 0 */
	endflag = 0;
	/* loop through file until linenum = wordnum, incrementing 
		linenum each time it scans an ascii line break. do so by calling linejumper */
	linejumper(ifile, wordnum);
	/* scan digit and space */
	fscanf(ifile, "%d%c", &filler, &placeholder);
	/* loop through word until end of word or file and count each letter */
	while(!endflag&&fscanf(ifile, "%c", &placeholder)!=EOF){
		/* scan letter if not last word */
		/* increment letter count if it is a letter */
		if(placeholder>=97&&placeholder<=122){
			numletters++;
		}
		else {
			endflag = 1;
		}
	}
	/* close file */
	fclose(ifile);
	
	/* return letter count */
	return(numletters);
}

/* a function that reads the file until you get to the word you want */
void linejumper(FILE *ifile, int wordnum){
	int linenum;
	char linebreak;
	linenum = 1;
	/* loop through file until numline is one less than wordnum, this marks
		the beginning of the line */
	while (linenum<wordnum) {
		/* scan through file */
		fscanf(ifile, "%c", &linebreak);
		/* increment when hit line break to count through lines */
		if(linebreak==10){
			linenum++;
		}
	}
}
	

void decrypt (int wordnum, int numletters, char *letter1, char *letter2, char *letter3, \
 char *letter4, char *letter5, char *letter6, char *letter7, char *letter8, char *letter9, \
 char *letter10) {
	/* initialize flag for if its a letter, which will tell when to stop loop */
	/* also initialize counter for place of letter, and shift scan */
	int letterplace, shift;
	char placeholder;
	/* initialize file using pointer */
	FILE *ifile;
	/* open file */
	ifile = fopen("wordbank.txt", "r");
	/* initialize letterplace to 0 */
	letterplace = 0;
	/* call function to jump to word player picked */
	linejumper(ifile, wordnum);	
	/* scan shift amount and space */
	fscanf(ifile, "%d%c", &shift, &placeholder);
	/* Keep scanning until you hit end of word */
	while (letterplace<=numletters) {	
		/* 10 if statements matching letter scanned with correct variable */
		if (letterplace==1) {
			*letter1 = lettershift(placeholder, shift);
		}
		else if (letterplace==2) { 
			*letter2 = lettershift(placeholder, shift);
		}
		else if (letterplace==3) { 
			*letter3 = lettershift(placeholder, shift);
		}
		else if (letterplace==4) { 
			*letter4 = lettershift(placeholder, shift);
		}
		else if (letterplace==5) { 
			*letter5 = lettershift(placeholder, shift);
		}
		else if (letterplace==6) { 
			*letter6 = lettershift(placeholder, shift);
		}
		else if (letterplace==7) { 
			*letter7 = lettershift(placeholder, shift);
		}
		else if (letterplace==8) { 
			*letter8 = lettershift(placeholder, shift);
		}
		else if (letterplace==9) { 
			*letter9 = lettershift(placeholder, shift);
		}
		else if (letterplace==10) { 
			*letter10 = lettershift(placeholder, shift);
		}
	
		/* scan letter and increment lettercount. put this at end of loop so that
			it stops immediately after letterplace surpasses numletters. */
		fscanf(ifile, "%c", &placeholder);
		
		letterplace++;

	}
	fclose(ifile);
}

void game(int numletters, char letter1, char letter2, char letter3, char letter4, \
char letter5, char letter6, char letter7, char letter8, char letter9, char letter10){
	/* initialize guess as integer, 10 alreadyguessed variables, 
	mistakes (count), win flag, filler to scan enter character*/
	int alreadyguessed1, alreadyguessed2, alreadyguessed3, alreadyguessed4, \
	 alreadyguessed5, alreadyguessed6, alreadyguessed7, alreadyguessed8, \
	 	alreadyguessed9, alreadyguessed10;
	int mistakes, win;
	char guess, filler;

	/* set alreadyguessed equal to zero for all 10 and numguesses, win, 
		and mistakes equal to zero */
	alreadyguessed1 = alreadyguessed2 = alreadyguessed3 = alreadyguessed4 = \
	alreadyguessed5 = alreadyguessed6 = alreadyguessed7 = alreadyguessed8 =  \
	alreadyguessed9 = alreadyguessed10 = 0;
	/* initialize number of mistakes and win flag to zero */
	mistakes = win = 0;

	/* print number of letters */
	printf("The word has %d letters.\n", numletters);

	/* while numguesses is less than 10, and player has not yet won */
	while (mistakes<10&&!win){
		/* print number of mistakes and number of mistakes left */
		printf("You have made %d mistakes and have %d mistakes left.\n", mistakes, 10-mistakes);
		/* if the number of letters is equal to number of correct guesses, you win! */
		/* this works even if there aren't 10 letters, because it just adds up when 
		there is a 1 for a specific letter. So if there are 7 letters, and 
			alreadyguessed1 through alreadyguessed7 are 1, they sum to 7,
				and it will catch the win! */
		if (numletters==alreadyguessed1+alreadyguessed2+alreadyguessed3+alreadyguessed4+ \
			alreadyguessed5+alreadyguessed6+alreadyguessed7+alreadyguessed8+ \
			alreadyguessed9+alreadyguessed10){
			/* set win equal to 1 so you don't print dashes later on */
			win=1;
			printf("Congratulations!! You win!\n");
		}

		/* print the word is */
		printf("The word is ");
		/* print each letter by using series of if/else if statements (letter or dash). 
			Only print dash if you didn't guess that letter, and if its place
				is less than or equal to the length of the letter. */
		
		if(alreadyguessed1){
			printf("%c", letter1);
		}
		else if (numletters>=1) {
			printf("_");
		}
		if(alreadyguessed2){
			printf(" %c", letter2);
		}
		else if (numletters>=2){
			printf(" _");
		}
		if(alreadyguessed3){
			printf(" %c", letter3);
		}
		else if (numletters>=3) {
			printf(" _");
		}
		if(alreadyguessed4){
			printf(" %c", letter4);
		}
		else if (numletters>=4){
			printf(" _");
		}
		if(alreadyguessed5){
			printf(" %c", letter5);
		}
		else if (numletters>=5) {
			printf(" _");
		}
		if(alreadyguessed6){
			printf(" %c", letter6);
		}
		else if (numletters>=6) {
			printf(" _");
		}
		if(alreadyguessed7){
			printf(" %c", letter7);
		}
		else if (numletters>=7){
			printf(" _");
		}
		if(alreadyguessed8){
			printf(" %c", letter8);
		}
		else if (numletters>=8){
			printf(" _");
		}
		if(alreadyguessed9){
			printf(" %c", letter9);
		}
		else if (numletters>=9){
			printf(" _");
		}
		if(alreadyguessed10){
			printf(" %c", letter10);
		}
		else if (numletters>=10) {
			printf(" _");
		}
		/* print period to end word if won */
		if(win){
			printf(".\n");
		}
		
		
		/* print hangman as mistakes increment */
		if(mistakes==1){
			printf("\nHere is the hangman: \n");
			printf("  --  \n");
		}
		else if(mistakes==2){
			printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|      \n");		}
		else if(mistakes==3){
			printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|    | \n");
		}
		else if(mistakes==4){
		printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|    | \n");
			printf("  __   \n");  
		}		
		else if(mistakes==5){
		printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|    | \n");
			printf("  __   \n");
			printf("  ||   \n");
			printf("  ||   \n");
		}		
		else if(mistakes==6){
			printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|    | \n");
			printf("  __   \n");
			printf("  ||   \n");
			printf("--||   \n");
		}		
		else if(mistakes==7){
		printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|    | \n");
			printf("  __   \n");
			printf("  ||   \n");
			printf("--||-- \n");
		}		
		else if(mistakes==8){
			printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|    | \n");
			printf("  __   \n");
			printf("  ||   \n");
			printf("--||-- \n");
			printf(" /     \n");
			printf("/      \n");
		}		
		else if(mistakes==9){
			printf("\nHere is the hangman: \n");
			printf("  --   \n");
			printf("|    | \n");
			printf("  __   \n");
			printf("  ||   \n");
			printf("--||-- \n");
			printf(" /  \\  \n");
			printf("/    \\ \n");
		}		
		/* 10 mistakes will be coded below */
		
		/* new line, and print guess prompt and scan into guess variable, if haven't won.
	  		Also scan enter character and put into filler! */
		if(!win){
			printf("\n\nGuess a letter: ");
			scanf("%c %c", &filler, &guess);
			
			/* convert guess to lowercase if it is not already */
			guess=upper2lower(guess);
		}
	
		/* if guess is one of the letters (use or), and it has not been guessed yet 
			(alreadyguessed for that letter is zero), then using nested if statements,
			change alreadyguessed for that letter number to 1.
			Otherwise, increment mistakes. */

		if(guess==letter1){
			alreadyguessed1 = 1;
		}
		if (guess==letter2) {
			alreadyguessed2 = 1;
		}
		if (guess==letter3) {
			alreadyguessed3 = 1;
		}
		if (guess==letter4) {
			alreadyguessed4 = 1;
		}
		if (guess==letter5) {
			alreadyguessed5 = 1;
		}
		if (guess==letter6) {	
			alreadyguessed6 = 1;
		}
		if (guess==letter6) {
			alreadyguessed6 = 1;
		}
		if (guess==letter7) {
			alreadyguessed7 = 1;
		}
		if (guess==letter8) {
			alreadyguessed8 = 1;
		}
		if (guess==letter9) {
			alreadyguessed9 = 1;
		}
		if (guess==letter10) {
			alreadyguessed10 = 1;
		}
		/* if guess isn't one of the letters, increment mistakes */
		if (guess!=letter1&&guess!=letter2&&guess!=letter3&&guess!=letter4&& \
			guess!=letter5&&guess!=letter6&&guess!=letter7&&guess!=letter8&& \
			guess!=letter9&&guess!=letter10) {
				mistakes++;
		}

	}
	/* if player hits 10 mistakes, print out losing message */
	if (mistakes==10){
		printf("Sorry! You lose!");
		printf("\nHere is the hangman: \n");
		printf("  --   \n");
		printf("| :( | \n");
		printf("  __   \n");
		printf("  ||   \n");
		printf("--||-- \n");
		printf(" /  \\  \n");
		printf("/    \\ \n");
	}

}
	




