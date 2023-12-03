//Amit Neeman 207388794
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>	// in order to use "time" function
#include <stdbool.h>

//The define
#define MAX_SIZE_NAME 20
#define NO_COLOR_FOR_THIS_CARD -1
#define MAX_SIZE_CARD_TYPE 10
#define START_CARDS 4
#define MAX_LETTER_OF_CARD_TYPE 7
#define SIZE_COLOR_OPTION 4
#define NOT_EXIST '0'
#define PLUS_CARD_INDEX 9
#define STOP_CARD_INDEX 10
#define CHANGE_DIRECTION_CARD_INDEX 11
#define TAKI_CARD_INDEX 12
#define  CHANGE_COLOR_CARD_INDEX 13
#define CARD_FROM_STOCK 0
#define NO_CARDS 0
#define LAST_CARD 1
#define CARDS 14

//The struct 
typedef struct card_type {
	int indexCardType;
	int countForStatistic;
}CARD_TYPE;

typedef struct Card {
	int cardType; //The index of the card type array 
	int color; // The index of the card color array
}CARD ;

typedef struct participant {
	char Name[MAX_SIZE_NAME];
	int logSize;
	int phySize;
	CARD* cards;
}PARTICIPANT;

//consts
const int CountOfCardType = 14;
const int CountOfColorOption = 4;
const int CountOfRegularCard = 9;
const int CountOfStars = 9;

//const array
const char* CardTypesInGame[CARDS] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", " + ", "STOP", "<->", "TAKI", "COLOR"};
const char OptionOfColors[SIZE_COLOR_OPTION] = { 'R', 'B', 'G', 'Y'};

//The function
void printStartMessage();
void getParticipantDetails(PARTICIPANT* participants, int numberOfplayers, CARD_TYPE* StatisticCards);
int amountPaticipants();
PARTICIPANT* CreatePaticipantsData(int numberOfPlayers);
void AddCardsToParticipantArray(int count, CARD* card_detail, CARD_TYPE* StatisticCards);
CARD getRandomCard(int countCardType);
void printCard(CARD card);
void printAllPlayerCards(int size, CARD* card);
//void checkSpaceInCardArray(CARD* cards, int logSize, int PhySize);
void startGame(PARTICIPANT* participants, int countNumberOfPalyers, CARD UpperCardOnTheTable, CARD_TYPE* StatisticCards);
void preparePlayerForNextTurn(int* i, int countOfPlayerInTheGame, bool* isPlayerFinishTurn,
	bool* isReverse, bool* isAnotherTurn, bool* isNextlayerStop);
void checkSpaceAndGetCardFromStock(PARTICIPANT* participant, CARD_TYPE* StatisticCards);
void RemoveCardFromCardArray(int indexToRemove, PARTICIPANT* participant);
void SpecialCardsInGame(int numPlayerCard, int* numberOfPlayer, CARD* UpperCardOnTheTable, PARTICIPANT* participant,
	bool* isPlayerFinishTurn, bool* isReverse, bool* isAnotherTurn, bool* isNextPlayerStop, bool* isThereIsWinner, CARD_TYPE* StatisticCards);
void PlusCardFunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant,
	bool* isPlayerFinishTurn, bool* isAnotherTurn, CARD_TYPE* StatisticCards);
void StopCardFunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant, int* numberOfPlayers,
	bool* isPlayerFinishTurn, bool* isAnotherTurn, bool* isNextPlayerStop, CARD_TYPE* StatisticCards);
void ChangeDirectionCardFunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant,
	bool* isPlayerFinishTurn, bool* isReverse);
void TakiCardfunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant, int* numberOfPlayer,
	bool* isPlayerFinishTurn, bool* isReverse, bool* isAnotherTurn, bool* isNextPlayerStop, bool* isWinner, CARD_TYPE* StatisticCards);
void MakeMoveInTheGame(PARTICIPANT* participant, CARD* UpperCard, int PlayerChooseCard, int* numberOfPlayers,
	bool* isPlayerFinishTurn, bool* isReverse, bool* isAnotherTurn,
	bool* isNextPlayerStop, CARD_TYPE* StatisticOfCards);
void printColorChangeMessage();
int TakiCardMessage(int logSize);
void AddCardToStatisticCount(int cardType, CARD_TYPE* StatisticCards);
CARD_TYPE* merge(CARD_TYPE arr1[], int size1, CARD_TYPE arr2[], int size2);
void copyArr(CARD_TYPE dest[], CARD_TYPE src[], int size);
void mergeSort(CARD_TYPE StatisticCards[], int size);
void printStatisticArray(CARD_TYPE StatisticCards[], int size);
void freeArray(PARTICIPANT* participants, int size);


void main() {
	srand(time(NULL)); //for random
	int countOfPlayers; //for the count of player in this game
	CARD UpperCardOnTheTable; // the upper card on the table
	PARTICIPANT* participants; //array of participants
	//about to reset the statistic cards array to index cads with the count 0 
	CARD_TYPE StatisticOfCards[CARDS] = { {0,0}, {1,0}, {2,0}, { 3,0 }, { 4,0 }, { 5,0 }, { 6,0 }, { 7,0 }, { 8,0 }, { 9,0 }, { 10,0 }, { 11,0 }, { 12,0 }, {13,0} };

	
	printStartMessage();
	countOfPlayers = amountPaticipants(); //get the amount of participants from the user
	participants = CreatePaticipantsData(countOfPlayers); //create array for the participants

	// about to get the participant name and his random cards to start the game
	//get 4 cards
	getParticipantDetails(participants, countOfPlayers, &StatisticOfCards);

	//get the start card for the game
	UpperCardOnTheTable = getRandomCard(CountOfRegularCard);

	//let start the game, finish when we have a winner
	startGame(participants, countOfPlayers, UpperCardOnTheTable, &StatisticOfCards);
	
	// merge the statistic array and print
	mergeSort(StatisticOfCards, CARDS);
	printStatisticArray(StatisticOfCards, CARDS);

	//free the array
	freeArray(participants, countOfPlayers);
}

// print message start
void printStartMessage() 
{
	printf("************  Welcome to ");
	printf("\x1b[32m"); //GREEN
	printf("T");
	printf("\x1b[31m"); // RED
	printf("A");
	printf("\x1b[33m"); //YELLOW
	printf("K");
	printf("\x1b[34m"); //BLUE
	printf("I ");
	printf("\33[0m"); //BLACK
	printf("game !!! ***********");
}

//this function get from the user the number of participant for this game
//check the number is valid
//return the number of the player
int amountPaticipants() {
	int numOfPlayers;
	printf("\nPlease enter the number of players:\n");
	scanf("%d", &numOfPlayers);

	while (numOfPlayers <= 0) 
	{
		printf("Invali number!\n");
		printf("\nPlease enter the number of players:\n");
		scanf("%d", &numOfPlayers);
	}
	return numOfPlayers;
}

// this function create dynamic arrays for the participants
//the function get the number of participant
// check that the adress isn't NULL (didn't work) - finish with exit
//return the adress of the array in heap
PARTICIPANT* CreatePaticipantsData(int numberOfPlayers)
{
	PARTICIPANT* participants;
	participants = (PARTICIPANT*)malloc(numberOfPlayers * sizeof(PARTICIPANT));

	//Check if allcation failed
	if (participants == NULL) {
		exit(1);
		//finish the progrem
	}
	return participants;
}
 
// this function get the count of cards we want to add for each player 
//also get the array card of the player and the adress of the statistic cards to add the new cards
//the function create card array for every player with the size 4 (the count of cards for the beginning)
// //check the address of the array is valid (if not - finish with error)
//at the end return the adress of the array by reference
void AddCardsToParticipantArray(int count, CARD** card_detail, CARD_TYPE* StatisticCards)
{
	int i;
	CARD* res;
	res = (CARD*)malloc(START_CARDS * sizeof(CARD));
	if (res == NULL)
		exit(1);

	for (i = 0; i < count; i++) {
		res[i] = getRandomCard(CountOfCardType);
		AddCardToStatisticCount(res[i].cardType, StatisticCards);
	}	

	*card_detail = res;
}


//This function get the details for the participant in the game
// get the name of every participant
//get for every participant the 4 start random card
//update the log and phy size of the array to the count of cards
//phy size - the total size of the array
//log size - the actual size of the array
//add the cards to statistic array
void getParticipantDetails(PARTICIPANT* participants, int numberOfplayers, CARD_TYPE* StatisticCards)
{
	int i, j;
	for (i = 0; i < numberOfplayers; i++) {
		//get the name of the participant
		printf("\nPlease enter the first name of player #%d \n", (i+1));
		scanf("%s", participants[i].Name);	
		
		//get 4 cards from stock 
		AddCardsToParticipantArray(START_CARDS, &participants[i].cards, StatisticCards);
		//update the array size after adding the cards
		participants[i].logSize = START_CARDS;
		participants[i].phySize = START_CARDS;

	}
}

// this function get the max number of the random range 
// return the random number between 1 to the rangeofRandom 
//for all cards - the range is 0-14
//for regular cards - the range is 0-9
//for color - the range is 0-3
int randomNumber(int rangeOfRandom) 
{
	int num;
	//srand(time(NULL));
	num = rand() % rangeOfRandom;
	
	return num;
}

//this function get the range of the random 
//the function calculate the card type and color accodring to the random num and the card type array and color array
//if the card type is change color - add to color the number -1 - no color for this card
//The function return the card (struct - cardType and color)
CARD getRandomCard(int countCardType)
{
	CARD card;
	card.cardType = randomNumber(countCardType);
	
	if (card.cardType == CHANGE_COLOR_CARD_INDEX) {
		card.color = NO_COLOR_FOR_THIS_CARD;
	}
	else {
		card.color = randomNumber(CountOfColorOption);
	}
	return card;
}

//this function print a card
//the function get one card and print the card type and the color by the format
void printCard(CARD card)
{
	int i;
	printf("*********\n");
	printf("*       *\n");
	int countType = strlen(CardTypesInGame[card.cardType]);
	if (card.cardType < 9) {
		for (i = 0; i < CountOfStars; i++) {
			if (i == 0 || (i == CountOfStars - 1)) {
				printf("*");
			}
			else if (i == ((CountOfStars - countType - 1) / 2)) {
				printf("%s", CardTypesInGame[card.cardType]);
			}
			else {
				printf(" ");
			}
		}
		printf("\n*  %c    *\n", OptionOfColors[card.color]);
		printf("*       *\n");
		printf("*********\n");
	}
	else {
		switch (card.cardType) {
		case PLUS_CARD_INDEX:
			printf("* ");
			printf("%s", CardTypesInGame[card.cardType]);
			printf("   *");
			break;
		case STOP_CARD_INDEX:
			printf("* ");
			printf("%s", CardTypesInGame[card.cardType]);
			printf("  *");
			break;
		case CHANGE_DIRECTION_CARD_INDEX:
			printf("* ");
			printf("%s", CardTypesInGame[card.cardType]);
			printf("   *");
			break;
		case TAKI_CARD_INDEX:
			printf("* ");
			printf("%s", CardTypesInGame[card.cardType]);
			printf("  *");
			break;
		case CHANGE_COLOR_CARD_INDEX:
			printf("* ");
			printf("%s", CardTypesInGame[card.cardType]);
			printf(" *");
			break;
		}
		if (card.cardType == CHANGE_COLOR_CARD_INDEX && card.color == NO_COLOR_FOR_THIS_CARD)
			printf("\n*       *\n");
		else 
			printf("\n*  %c    *\n", OptionOfColors[card.color]);
			
		printf("*       *\n");
		printf("*********\n");
		
	}
}

//this function print a array of card
//the function get array of struct CARD and the size of the array
void printAllPlayerCards(int size,CARD* card)
{
	int i;
	for (i = 0; i < size; i++) {
		printf("\n");
		printf("Card #%d:\n", i + 1);
		printCard(card[i]);
	}
}

// this function check the user enter a valid number
int playerChooseValidation(int logSize) {
	int NumPlayerChoose;
	bool validNum = false;
	printf("Please enter 0 if you want to take a card from the deck\n");
	printf("or 1-%d if you want to put one of your card in the middle:\n", logSize);
	scanf("%d", &NumPlayerChoose);

	while (!validNum) {
		if (NumPlayerChoose >= 0 && NumPlayerChoose <= logSize)
			validNum = true;
		else {
			printf("Invalid Choose! Try again.\n");
			printf("Please enter 0 if you want to take a card from deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", logSize);
			scanf("%d", &NumPlayerChoose);
		}
	}

	return NumPlayerChoose;
}

//The main function of the game
//this function run the game - decide who the next player, what is the card says, what the next move in the player
//run until the game end when we have a winner
//the function send a few boolean to the function to know what to do
//also update the statistic count
void startGame(PARTICIPANT* participants, int countNumberOfPalyers, CARD UpperCardOnTheTable, CARD_TYPE* StatisticOfCards)
{
	int i = 0, playerChooseCard;
	int index;
	int j;
	bool isPlayerFinishTurn = true; //check if we move to next player
	bool isThereIsWinner = false; //stop the function if we have a winner
	bool isReverse = false; //is need to change direction
	bool isAnotherTurn = false; //is the player have another turn
	bool isNextlayerStop = false; //is next player stopped
	//system("CLS");
	while (!isThereIsWinner) {
		
		// print all cards only if new turn of the player
			printf("Upper card:\n");
			printCard(UpperCardOnTheTable);

			//new turn, print the name and all cards
			if (isPlayerFinishTurn) 
				printf("%s's turn:", participants[i].Name);
		//print all player card
		printAllPlayerCards(participants[i].logSize, participants[i].cards);

		//check validation of the number card of the player 
		playerChooseCard = playerChooseValidation(participants[i].logSize);
		if (playerChooseCard == CARD_FROM_STOCK)
		{
			//the player choose to take cards from the stock 
			//finish his turn
			checkSpaceAndGetCardFromStock(&(participants[i]), StatisticOfCards); //check if we have space in cards array
			isPlayerFinishTurn = true; //move to the next player
		}
		else {
			playerChooseCard = playerChooseCard - 1; //the cards start from 0 so remove 1 from player choose
			// make move function 
			MakeMoveInTheGame(&(participants[i]), &UpperCardOnTheTable, playerChooseCard, &countNumberOfPalyers,
				&isPlayerFinishTurn,&isReverse, &isAnotherTurn, &isNextlayerStop, &isThereIsWinner, StatisticOfCards);
		}

		//check if we have a winner
		for (index = 0; index < countNumberOfPalyers; index++)
		{
			if (participants[index].logSize == NO_CARDS)
			{
				isThereIsWinner = true;
				//winnter message
				printf("The winner is... %s! ", participants[index].Name);
				printf("\x1b[31m"); //PRINT IN RED
				printf("\nCongratulations! ! !\n");
				printf("\33[0m"); //BLACK
			}
		}
		//prepare for the next player
		//update the index of function according to the boolean value
		//get the adress of the index, count of player and the boolean (finish, reverse, another turn and next stop)
		preparePlayerForNextTurn(&i, countNumberOfPalyers, &isPlayerFinishTurn, &isReverse, 
			&isAnotherTurn, &isNextlayerStop);
		//system("CLS");
	}
}

//this fucntion prepare player for the next turn 
//the function change according to the boolean
//change if have anotherturn
//change if the next player is stop
//change if the next player is reverse
//update the index of the player in startGame function
void preparePlayerForNextTurn(int* i, int countOfPlayerInTheGame, bool* isPlayerFinishTurn,
	bool* isReverse, bool* isAnotherTurn, bool* isNextlayerStop)
{
	int indexPlayer = *i;
	if (!(*isPlayerFinishTurn)) {
		return;
	}
	else {
		//if we play with isReverse is false 
		// moving forward
		if (!(*isReverse)) {
			if (*isNextlayerStop) {
				// if the player is the last in array
				//move to the second in the array
				if (indexPlayer == countOfPlayerInTheGame - 1) {
					indexPlayer = 1;
				}
				//if the last is one before the last
				// move to the first in the array
				else if (indexPlayer == countOfPlayerInTheGame - 2){
					indexPlayer = 0;
				}
				else {
					// pass on the next player and move to the next after
					// move 2 index forward
					indexPlayer += 2;
				}
			}
			else {
				//if the player is the last in array
				//move to the first in array
				if (indexPlayer == countOfPlayerInTheGame - 1) {
					indexPlayer = 0;
				}
				else {
					// move to the next player in array
					indexPlayer++;
				}
			}
		}
		//isReverse is true - moving backward 
		else {
			if(*isNextlayerStop) {
				// the player is the first is the array at the backward
				//move to one before the end at the array
				if (indexPlayer == 0) {
					indexPlayer = countOfPlayerInTheGame - 2;
				}
				//if the player is second at the array in backward
				// move to the last in the array
				else if(indexPlayer == 1){
					indexPlayer = countOfPlayerInTheGame - 1;
				}
				else {
					//go 2 back 
					indexPlayer -= 2;
				}
			}
			//isNextPlayerStop is false 
			//move only 1 back
			else {
				if (indexPlayer == 0) {
					indexPlayer = countOfPlayerInTheGame - 1;
				}
				else {
					//go one back
					indexPlayer--;
				}
			}
		}
		*i = indexPlayer;
	}

}

//this function check if the log and phy size equal (need to big the dynamic array)
//input: card array, log size, phy size
//output: big the array size by *2 and add ramdon card to the list 
void checkSpaceAndGetCardFromStock(PARTICIPANT* participant, CARD_TYPE* StatisticCards)
{
	CARD* tmp; //tmp array for this function
	int i, indexForStatistic, type;
	int logS = participant->logSize; //tmp int for this function
	int phyS = participant->phySize; // tmp int for this function

	if (logS == phyS) {
		phyS *= 2;
		tmp = (CARD*)malloc(phyS * sizeof(CARD));
		if (tmp == NULL) {
			printf("Memony rellocation failed!\n");
			exit(1); // finish program with error
		}
		for (i = 0; i < logS; i++) {
			tmp[i] = participant->cards[i];
		}
			free(participant->cards);
			participant->cards = tmp;
	}
	participant->cards[logS] = getRandomCard(CountOfCardType);
	type = participant->cards[logS].cardType;
	AddCardToStatisticCount(type, StatisticCards);
	logS++;

	// return the value to the value in the array
	participant->logSize = logS;
	participant->phySize = phyS;	
	
}

//This function swap between the index to remove and the last index 
//down the log size with 1
void RemoveCardFromCardArray(int indexToRemove, PARTICIPANT* participant)  
{
	CARD tmp;
	PARTICIPANT player = *participant;
	int logS = (player.logSize);
	if (indexToRemove - 1 == logS - 1) {
		player.logSize=logS - 1;
	}
	else {
		tmp = player.cards[indexToRemove];
		player.cards[indexToRemove] = player.cards[logS - 1];
		player.cards[logS - 1] = tmp;
		player.logSize = logS - 1;
	}
	*participant = player;
}

//this fcuntion do the function of the special cards in game 
//get the num player card, the count of player, the upper cards on the table(the last valid card), the participant detail, the boolean of the winner
//and the array of statistic cards
void SpecialCardsInGame(int numPlayerCard, int* numberOfPlayer, CARD* UpperCardOnTheTable, PARTICIPANT* participant,
	bool* isPlayerFinishTurn, bool* isReverse, bool* isAnotherTurn, bool* isNextPlayerStop, 
	bool* isThereIsWinner, CARD_TYPE* StatisticOfCards)
{
	PARTICIPANT player = *participant; //tmp for this function
	int cardType = player.cards[numPlayerCard].cardType; //get the card type
	switch (cardType) {
	case PLUS_CARD_INDEX:
		//plus card function
		PlusCardFunction(numPlayerCard, UpperCardOnTheTable, participant,
			isPlayerFinishTurn, isAnotherTurn, StatisticOfCards);
		break;
	case STOP_CARD_INDEX:
		//STOP CARD FUNCTION
		StopCardFunction(numPlayerCard, UpperCardOnTheTable, participant, numberOfPlayer,
			isPlayerFinishTurn, isAnotherTurn, isNextPlayerStop, StatisticOfCards);
		break;
	case CHANGE_DIRECTION_CARD_INDEX:
		ChangeDirectionCardFunction(numPlayerCard, UpperCardOnTheTable, participant, 
			isPlayerFinishTurn, isReverse);
		// Change direction function
		break;
	case TAKI_CARD_INDEX:
		TakiCardfunction(numPlayerCard, UpperCardOnTheTable, participant, numberOfPlayer, isPlayerFinishTurn
			,isReverse, isAnotherTurn, isNextPlayerStop, isThereIsWinner, StatisticOfCards);
		//change taki function
		break;
	default:
		//the card is invalid
		//the player have another turn
		printf("Invalid card! Try again.\n");
		*isPlayerFinishTurn = false;
		break;
	}
}

// plus card function
//this function give the player another turn to put card
//if the last card of the player is plus card, he will get another card from the stock automaticlly
//update the values by reference
void PlusCardFunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant,
	bool* isPlayerFinishTurn, bool* isAnotherTurn, CARD_TYPE* StatistiCads)
{
	PARTICIPANT player = *participant; //tmp for this function
	// if the player put + card but it's his last card
	//if the plus card is the last, add to the player another card from the stock without asking him
	if (player.logSize == LAST_CARD) {
		RemoveCardFromCardArray(numPlayerCard, participant);
		checkSpaceAndGetCardFromStock(participant, StatistiCads);
		*isPlayerFinishTurn = true;
		*isAnotherTurn = false;
		*UpperCardOnTheTable = player.cards[numPlayerCard];
	}
	else {
		//the player have another turn
		*isAnotherTurn = true;
		*isPlayerFinishTurn = false;
		*UpperCardOnTheTable = player.cards[numPlayerCard];
		// remove the card from the list player
		RemoveCardFromCardArray(numPlayerCard, participant);

	}
		// return the tmp player to the original player
		*participant = player;
		// remove the card from the list player
		//RemoveCardFromCardArray(numPlayerCard ,participant);
}

//stop card function
//This function stop the next player his turn and contiue to the next one
//if have only 2 players and one on the player his last card is stop, he will get from the stock automaticlly
//return the values by reference 
void StopCardFunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant, int* numberOfPlayers,
	bool* isPlayerFinishTurn, bool* isAnotherTurn, bool* isNextPlayerStop, CARD_TYPE* StatisticOfCards)
{
	PARTICIPANT player = *participant; //tmp player for this function
	int numberOfPlayer = *numberOfPlayers;
	//if the user have last card is stop and only 2 players left in the game
	//add to the player one card from the stock without asking him 
	//update the upper card on the table
	if (player.logSize == LAST_CARD && (numberOfPlayer == 2))
		checkSpaceAndGetCardFromStock(participant, StatisticOfCards); //check space and add card
	
	*UpperCardOnTheTable = player.cards[numPlayerCard];
	*isPlayerFinishTurn = true; //the player finish his turn
	*isAnotherTurn = false; //doesn't have another turn
	*isNextPlayerStop = true;
	//remove the card from the player list
	RemoveCardFromCardArray(numPlayerCard, participant);
}

//change direction function
//this function update the new upper card for the new card 
//and change the direction in the game
//update the values by reference
void ChangeDirectionCardFunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant,
	bool* isPlayerFinishTurn, bool* isReverse)

{
	CARD card = participant->cards[numPlayerCard]; //tmp card for this function 
	//about to change the change the direction of the game
	// if true change to false
	//if false change to true
	if (*isReverse) { 
		*isReverse = false;
	}
	else {
		*isReverse = true;
	}

	*isPlayerFinishTurn = true; //finish his turn
	*UpperCardOnTheTable = card; // the new upper card 
	//remove from player list
	RemoveCardFromCardArray(numPlayerCard, participant);
}

//Taki card function
//this function get the num player card, the adress of the upper card, the adress participant detail, number of player, boolean is finish, is reverse,
//is another turn, is next stop, is stop and statistic card array
//the function get cards until the user enter 0 to exit from taki card or change color card
void TakiCardfunction(int numPlayerCard, CARD* UpperCardOnTheTable, PARTICIPANT* participant, int* numberOfPlayer,
	bool* isPlayerFinishTurn, bool* isReverse, bool* isAnotherTurn, bool* isNextPlayerStop, bool* isWinner, CARD_TYPE* StatisticOfCards)
{
	int cardNum, changeColor, playerCardNum;
	CARD tmp_upperCard = *UpperCardOnTheTable; //tmp card for this function
	CARD lastCard;
	PARTICIPANT player = *participant; //tmp player for this function
	bool isFinish = false; 
	bool FunctionLastCard = false;
	// if the taki card is the last card in array cards in player
	//we have a wiiner
	//finish the function
	if (player.logSize == 1) {
		*isWinner = true;
	}
	else {
		RemoveCardFromCardArray(numPlayerCard, &player); //remove card from the array
		printAllPlayerCards(player.logSize, player.cards);
		//get from the user the card number to enter
 		cardNum = TakiCardMessage(player.logSize); //check the user enter a valid number
		if (cardNum == 0) { //to finish the taki card
			*UpperCardOnTheTable = player.cards[numPlayerCard];
			*isPlayerFinishTurn = true;
		}

		while (cardNum != 0 && !isFinish) //run until the user wants to finish - by put change color card or enter 0 to exit
		{
			playerCardNum = cardNum - 1;

			//if the card is change color card
			//change color and finish the turn
			if (player.cards[playerCardNum].cardType == CHANGE_COLOR_CARD_INDEX) {
				printColorChangeMessage();
				scanf("%d", &changeColor);
				player.cards[playerCardNum].color = changeColor - 1;
				tmp_upperCard = player.cards[playerCardNum];
				RemoveCardFromCardArray(playerCardNum, &player);
				isFinish = true; //finish this function
				FunctionLastCard = false;

				//print the cards without the user choose and get the new card
				printAllPlayerCards(player.logSize, player.cards);
				cardNum = TakiCardMessage(player.logSize);

			}
			//if the card is the same color as the taki card 
			// remove card from array 
			//add card to statistic
			else if (tmp_upperCard.color == player.cards[playerCardNum].color || tmp_upperCard.cardType == player.cards[playerCardNum].cardType) {
				tmp_upperCard = player.cards[playerCardNum];
				FunctionLastCard = true;
				RemoveCardFromCardArray(playerCardNum, &player); //remove card from list

				//print the cards without the user choose and get the new card
				printAllPlayerCards(player.logSize, player.cards);
				cardNum = TakiCardMessage(player.logSize);
			}
			else {
				//the user wants to enter invalid card
				//not the same color
				printf("Invalid card! Try again.\n");
			}
			// if the player finish his turn
			//check if the last card is plus
			if (player.logSize == 0)
			{
				if (tmp_upperCard.cardType != PLUS_CARD_INDEX)
				{
					*isWinner = true;
				}
					isFinish = true;
			}
		}
		// return the tmp value to the original
		*participant = player;
		*UpperCardOnTheTable = tmp_upperCard;

		//about to get the last card in the taki "mappal" and do the function of the card
		if (FunctionLastCard && *isWinner == false) {
			// regular cards
			if (tmp_upperCard.cardType < PLUS_CARD_INDEX) {
				RemoveCardFromCardArray(playerCardNum, participant);
				*isPlayerFinishTurn = true;
			}
			else {
				//special cards
				switch (tmp_upperCard.cardType) {
					case PLUS_CARD_INDEX:
						PlusCardFunction(playerCardNum, UpperCardOnTheTable, participant, 
							isPlayerFinishTurn, isAnotherTurn, StatisticOfCards);
						break;
					case STOP_CARD_INDEX:
						StopCardFunction(playerCardNum, UpperCardOnTheTable, participant, numberOfPlayer, 
							isPlayerFinishTurn, isAnotherTurn, isNextPlayerStop, StatisticOfCards);
						break;
					case CHANGE_DIRECTION_CARD_INDEX:
						ChangeDirectionCardFunction(playerCardNum, UpperCardOnTheTable, participant, 
							isPlayerFinishTurn, isReverse);
						break;
					default:
						*isPlayerFinishTurn = true;
						break;
				}
			}
		}
	}
}

//this function make the move in the game
//check first if the card is change color card 
//check if the card that the user wants to put is in the same color as the upper card and if it's so check the card type
//check if the card that the user wants to put is in the same card type as the upper card and if it's so check the functions
//if the user doesn't put 0 (to take card from the stock) the function come here
// this function manage the all player move in the game
void MakeMoveInTheGame(PARTICIPANT* participant, CARD* UpperCard ,int PlayerChooseCard, int* numberOfPlayers,
	bool* isPlayerFinishTurn, bool* isReverse, bool* isAnotherTurn, bool* isNextPlayerStop,
	bool * isThereIsWinner, CARD_TYPE* StatisticOfCards)
{
	//have the same color card
	CARD tmp_card = *UpperCard; //tmo card
	PARTICIPANT ThisPlayer = *participant; 
		//the card is change color card
		//change the color of the card and update the upper card with the new color
		if (ThisPlayer.cards[PlayerChooseCard].cardType == CHANGE_COLOR_CARD_INDEX)
		{
			int colorChange;
			CARD tmp_card = *UpperCard;
			printColorChangeMessage();
			scanf("%d", &colorChange);

			//check the user enter a valid number for color change
			while (colorChange < 1 || colorChange > 5) {
				printf("Invalid choice! Try again.\n");
				printColorChangeMessage();
				scanf("%d", &colorChange);
			}
			// change the change color card with the new color
			tmp_card.color = colorChange - 1;
			tmp_card.cardType = CHANGE_COLOR_CARD_INDEX;
			*UpperCard = tmp_card;
			RemoveCardFromCardArray(PlayerChooseCard, participant);
		}
		//check if the user have the same color on the card as the upper card
		else if(tmp_card.color == ThisPlayer.cards[PlayerChooseCard].color) {
			//check if regular card
			if (ThisPlayer.cards[PlayerChooseCard].cardType < PLUS_CARD_INDEX) {
				tmp_card = ThisPlayer.cards[PlayerChooseCard];
				RemoveCardFromCardArray(PlayerChooseCard, participant); //remove the card
				*isPlayerFinishTurn = true; //finish turn
				*UpperCard = tmp_card; //return the tmp

			}
			else {
				//the same color but special cards
				SpecialCardsInGame(PlayerChooseCard, numberOfPlayers, UpperCard, participant, isPlayerFinishTurn,
					isReverse, isReverse, isAnotherTurn, isThereIsWinner, StatisticOfCards);
			}

		}
		//the same card type but different color
		else if (tmp_card.cardType == ThisPlayer.cards[PlayerChooseCard].cardType)
		{
			//if the card is "regular card" - a card between 1-9
			//remove the card from player list
			// and make this card as Uppercard of the game
			//update the values by reference
			if (ThisPlayer.cards[PlayerChooseCard].cardType < PLUS_CARD_INDEX) {
				tmp_card = ThisPlayer.cards[PlayerChooseCard];
				RemoveCardFromCardArray(PlayerChooseCard, participant);
				*isPlayerFinishTurn = true;
				*UpperCard = tmp_card;

			}
			// the card is special card
			else {
				SpecialCardsInGame(PlayerChooseCard, numberOfPlayers, UpperCard, participant, isPlayerFinishTurn,
					isReverse, isReverse, isAnotherTurn, isThereIsWinner, StatisticOfCards);
			}
		}
		else {
			//the card the user wants to put is incorrect
			//print a meesage
			//and the user doesn't finish this turn
			printf("Invalid card! Try.again.\n");
			*isPlayerFinishTurn = false;
		}
}

//print the color change message
void printColorChangeMessage() {
	printf("Please enter your color choice:\n");
	printf("\x1b[31m"); //RED
	printf("1 - Red\n");
	printf("\x1b[34m"); //BLUE
	printf("2 - Blue\n");
	printf("\x1b[32m"); //GREEN
	printf("3 - Green\n");
	printf("\x1b[33m"); //YELLOW
	printf("4 - Yellow\n");
	printf("\33[0m"); //BLACK
}

//printf taki card message
int TakiCardMessage(int logSize)
{
	int cardNum;
	printf("Please enter 0 if you want to finish your turn\n");
	printf("or 1-%d if you want to put one of your cards in the midlle:\n", logSize);
	scanf("%d", &cardNum);
	
	//check the user enter a valid number
	while (cardNum < 0 || cardNum > logSize)
	{
		printf("Invalid card! Try again.\n");
		printf("Please enter 0 if you want to finish your turn\n");
		printf("or 1-%d if you want to put one of your cards in the midlle:\n", logSize);
		scanf("%d", &cardNum);
	}
	return cardNum;
}

//this fucntion get the index of the card type and the statistic cards array
//the function add 1 to the count of the card type index
//the array have the statistic of the cards in this game
void AddCardToStatisticCount(int cardType, CARD_TYPE* StatisticCards)
{
	int i;
	for (i = 0; i < CARDS; i++)
	{
		if (StatisticCards[i].indexCardType == cardType) {
			StatisticCards[i].countForStatistic++;
		}
	}
}

//this function get statistic array and the size of the array
//the function sort 2 part of the array (from 0 to size/2) and (from size/2 to size-size/2)
//the logic is merge sort with regursion
void mergeSort(CARD_TYPE StatisticCards[], int size) {
	CARD_TYPE* tmpArr;
	if (size <= 1)
		return;
	
	mergeSort(StatisticCards, size / 2); //half of the array
	mergeSort(StatisticCards + size / 2, size - (size / 2)); //half of the array
	tmpArr = merge(StatisticCards, size / 2, StatisticCards + size / 2, size - size / 2); //sorted between the 2 sorted array
	if (tmpArr) {
		copyArr(StatisticCards, tmpArr, size); //copy the array values from the tmp to original
		free(tmpArr); //free the tmp array
	}
	else {
		//print error and finish the program if the tmparr adress is null
		printf("Memory allocation failure! ! !\n");
		exit(1);
	}
}
//this function get 2 sorted array, 2 size
//the function check for the smaller value in each array and add the value from the new array(res)
//the function return the adress of the new array - res
CARD_TYPE* merge(CARD_TYPE arr1[], int size1, CARD_TYPE arr2[], int size2) {
	int ind1, ind2, resInd;
	CARD_TYPE* res = (CARD_TYPE*)malloc((size1 + size2) * sizeof(CARD_TYPE));
	if (res)
	{
		ind1 = ind2 = resInd = 0;
		//run while both of the ind smaller than their size
		while ((ind1 < size1) && (ind2 < size2)) {
			if (arr1[ind1].countForStatistic >= arr2[ind2].countForStatistic) {
				res[resInd] = arr1[ind1];
				ind1++;
			}
			else {
				res[resInd] = arr2[ind2];
				ind2++;
			}
			resInd++;
		}
		//run if the ind1 is snaller than size1
		while (ind1 < size1) {
			res[resInd] = arr1[ind1];
			resInd++;
			ind1++;
		}
		//run if the ind2 is snaller than size2
		while (ind2 < size2) {
			res[resInd] = arr2[ind2];
			resInd++;
			ind2++;
		}
	}
	return res;
}


//this fucntion copy the sorted array to the original array
void copyArr(CARD_TYPE dest[], CARD_TYPE src[], int size) {
	int i;
	for (i = 0; i < size; i++) {
		dest[i] = src[i];
	}
}

//this function print the statistic detail array 
//print the most frequency card to the last
void printStatisticArray(CARD_TYPE StatisticCards[], int size)
{
	int i;
	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n\n");
	for (i = 0; i < size; i++)
	{
		int indexType = StatisticCards[i].indexCardType;
		if (StatisticCards[i].indexCardType < 9) {
			printf("   %s  ", CardTypesInGame[indexType]);
			printf(" | ");
		}
		else if (StatisticCards[i].indexCardType == CHANGE_COLOR_CARD_INDEX) {
			printf(" %s ", CardTypesInGame[indexType]);
			printf("| ");
		}
		else if (StatisticCards[i].indexCardType == CHANGE_DIRECTION_CARD_INDEX || StatisticCards[i].indexCardType == PLUS_CARD_INDEX) {
			printf("  %s  ", CardTypesInGame[indexType]);
			printf("| ");
		}
		else if (StatisticCards[i].indexCardType == STOP_CARD_INDEX) {
			printf(" %s  ", CardTypesInGame[indexType]);
			printf("| ");
		}
		else  {
			printf(" %s  ", CardTypesInGame[indexType]);
			printf("| ");
		}
		printf("%d", StatisticCards[i].countForStatistic);
		printf("\n");
	}
	printf("\nGood Bye ! ! !\n");

}


//this function free the array when the game finish
//free first the cards array for each participant
//and than free the participants arrat
void freeArray(PARTICIPANT* participants, int size)
{
	int i;
	for (i = 0; i < size;i++)
	{
		free(participants[i].cards);
	}
	free(participants);
}
