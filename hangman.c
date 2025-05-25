#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

void StartGame(int *);
int GameModeMenu();
int LivesGenerator(int); // Generates number of lives depending on game mode
void GetBuffer(char *); // Gets a random line from the txt file and inserts it in the Buffer
int LineGenerator(); // Chooses a random number from a specific number of lines!
void Instructions(); 
void InitHidden(char *, char *); // Fills the buffer with the "-" character.
void SpacebarDetector(char *, char *, int *); // Spacebar characters don't have to be found by the user
void DisplayWord(char *); // Prints the buffer
void LettersInserted(char *); // Shows the letters inserted by the user
void HistoryModifier(char *, char); // Makes sure the inserted letters won't be displayed twice
int CompareLetter(char, char *, char *, int *); // Compares the letter inserted with each letter in the Buffer
void Countdown();
void Bye();
void ClearScreen(); // Clear screen function including both Linux and Windows command.

int main (int argc, char* argv[])
{
	ClearScreen();
	int winstreak = 0;
	char answer;
	do
	{
		system("clear");
		printf("               WELCOME TO HANGMAN!                                      Win streak: %03d\n", winstreak);
		printf("\n               Start game? (Y/N)");
		printf("\n\nWaiting for answer: ");
		scanf(" %c", &answer);
        answer=toupper(answer);
		if(answer == 'Y')
		{
			StartGame(&winstreak);
		}
	}
	while(answer!= 'N');
	Bye();
	return 0;
}

void StartGame(int *streak)
{
	system("clear");
	unsigned int lives = GameModeMenu();
	system("clear");
	Instructions();
	char *Buffer, *HiddenArray, *InputHistory;
	Buffer = (char*)malloc(50*sizeof(char));
    if(Buffer==NULL)
    {
        ClearScreen();
        perror("Allocation error");
        exit(2);
    }
	GetBuffer(Buffer);
	HiddenArray = (char*)malloc((strlen(Buffer) + 1)*sizeof(char));
    if(HiddenArray==NULL)
    {
        ClearScreen();
        perror("Allocation error");
        exit(2);
    }
	InitHidden(HiddenArray, Buffer);
	InputHistory = (char*)calloc(26, sizeof(char));
    if(InputHistory==NULL)
    {
        ClearScreen();
        perror("Allocation error");
        exit(2);
    }
	char letter;
	int answer;
	int correct = 0;
	SpacebarDetector(Buffer, HiddenArray, &correct);
	while(lives>0 && correct != strlen(Buffer))
	{
		system("clear");
		printf("Lives remaining: %02d", lives);
		DisplayWord(HiddenArray);
		LettersInserted(InputHistory);
		printf("\n\n\nInsert letter: ");
		scanf(" %c", &letter);
        letter=toupper(letter);
	    HistoryModifier(InputHistory, letter);
		answer = CompareLetter(letter, Buffer, HiddenArray, &correct);
		if (answer == 0)
		{
			lives--;
			(*streak) = 0;
		}
	}
	if (lives != 0)
	{
		printf("\n\n\n\nYou won! :3 :3");
		printf("\n\nThe answer was %s", Buffer);
		(*streak)++;
	}
	else
	{
		printf("\n\n\n\nGame over (vanessa said calmly)");
		printf("\nThe answer was %s", Buffer);
	}
	free(Buffer);
	free(HiddenArray);
	Countdown();
}

int GameModeMenu()
{
	int mode;
	do
	{
	ClearScreen();
	printf("                   CHOOSE YOUR GAME MODE\n");
	printf("1) Easy (8 lives)    2) Normal (5 lives)   3) Hard (3 lives)\n\n");
	printf("Insert game mode: ");
	scanf("%d", &mode);
	}
	while(!LivesGenerator(mode));
	return LivesGenerator(mode);
}

int LivesGenerator(int input)
{
	switch(input)
	{
		case 1: return 8; break;
		case 2: return 5; break;
		case 3: return 3; break;
		default: return 0; 
	}
}


void Instructions()
{
	printf("                                                       INSTRUCTIONS:\n");
	printf("\nInsert UPPERCASE characters and find the hidden word. Enjoy the game!");
	printf("\n\n\n\nPress enter to proceed.");
	getchar(); getchar();
}

int LineGenerator()
{
	time_t t;
	srand(time(NULL));
	int line = (rand() % 287 + 1);
	return line;
}

void GetBuffer(char *Buffer)
{
	FILE *fp;
	fp = fopen("ANIMECHARACTERS.txt", "r");
    if (fp == NULL)
    {
        ClearScreen();
        perror("Error loading file.");
        exit(1);
    }
	int i, line;
	rewind(fp);
	line = LineGenerator();
	for (i = 0; i < line; i++ )
	{
		fgets(Buffer, 50, fp);
	}
	Buffer[strlen(Buffer) - 1] = 0;
	fclose(fp);
}

void InitHidden(char *array, char *array2)
{
	int i;
    int length = strlen(array2);
	for(i=0; i < length; i++)
	{
		array[i] = '-';
	}
	array[length] = 0;
}

void SpacebarDetector(char *array1, char *array2, int *correct)
{
	int i;
	for (i=0; i < strlen(array1); i++)
	{
		if(isspace(array1[i]))
		{
			array2[i] = ' ';
			(*correct)++;
		}
	}
}

void DisplayWord(char *array)
{
	int i;
	printf("\n\n\n\n");
	for (i = 0; i<strlen(array); i++)
	{
		printf("%c ", array[i]);
	}
}

void LettersInserted(char *array)
{
	printf("\n\nLetters inserted: ");
	int i;
	int length = strlen(array);
	for (i=0; i<(length); i++)
	{
		printf("%c ", array[i]);
	}
}

void HistoryModifier(char *array, char letter)
{
	int i, f;
	f = 0;
	for (i=0; i<26; i++)
	{
		if (array[i]==letter)
		{
			f = 1;
			break;
		}
	}
	if (!f)
	{
		for (i=0; i<26; i++)
		{
			if (array[i] == 0)
			{
				array[i] = letter;
				break;
			}
		}
	}
}

int CompareLetter(char letter, char *array1, char *array2, int *correct)
{
	int i, f;
	f = 0;
	for (i = 0; i < strlen(array1); i++)
	{
		if (isspace(letter) && isspace(array1[i]))
		{
			array2[i] = ' ';
			f = 1;
			(*correct)++;
		}
		else if(letter == array1[i])
		{
			array2[i] = letter;
			f = 1;
			(*correct)++;
		}
	}
	return f;
}

void Countdown()
{
	int i;
	printf("\n\nWaiting to go back to main screen... ");
	fflush(stdout);
	sleep(1);
    for (i = 5; i > 1; i--)
    {
    	printf("%d, ", i);
	fflush(stdout);
    	sleep(1);
	}
	printf("1...");
	fflush(stdout);
	sleep(1);
}

void Bye()
{
	ClearScreen();
	printf("Goodbye, hope to see you again!");
	sleep(2);
}

void ClearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
