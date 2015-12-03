#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

void createPopulation(int population[][3], int pieces[][10]); // create the population
void calculateFitness(int population[][3], int values[][12]);
void selection(int population[][3], int momentary_population[][3], int pieces[][10], int position);
void cruce(int gen1[], int gen2[], int momentary_population[][3], int pieces[][10], int position);
void mutacion(int gen[], int momentary_population[][3], int pieces[][10], int position);
void changeValues(int x, int y, int values[][12]);
bool checkGame(int player, int x, int y, int pieces[][10]);
bool topLine(int player, int x, int y, int pieces[][10]);
bool rightLine(int player, int x, int y, int pieces[][10]);
bool bottomLine(int player, int x, int y, int pieces[][10]);
bool leftLine(int player, int x, int y, int pieces[][10]);
bool topRightDiagonal(int player, int x, int y, int pieces[][10]);
bool topLeftDiagonal(int player, int x, int y, int pieces[][10]);
bool bottomRightDiagonal(int player, int x, int y, int pieces[][10]);
bool bottomLeftDiagonal(int player, int x, int y, int pieces[][10]);

int main()
{
	int values[10][12]; //save how many pieces lack to do a well game for the computer
	int pieces[8][10] = {0}; //save the position ocuppied for a piece (1 - computer, 2 - user)
	int population[10][3]; //save que population and his probability {x, y, computer, user}
	bool game_ended; //save if the game has finished
	int momentary_population[10][3]; //save the individuals while create the new population
	int file, column; //save each coordinate of the user's game
	int match; //saves the computer's match
	int computer_x, computer_y; //save the coordinates of the computer match

	srand(time(NULL)); //initialize the seed

	//initialize values with five
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 12; j++)
			values[i][j] = 5;
	}

	//start the game
	while(game_ended != true)
	{
		//PRINT THE PIECES'S MATRIX
		printf("  1 2 3 4 5 6 7 8 9 10\n");
		for(int i = 0; i < 8; i++)
		{
			printf("%d", i + 1);
			for(int j = 0; j < 10; j++)
				printf(" %d", pieces[i][j]);
			printf("\n");
		}


		//GET THE USER COORDINATES
		printf("Dame las coordenadas de tu jugada: \n");
		do{
			file = 0; //initialize file
			column = 0; //initialize column
			while(file > 8 || file < 1)
			{
				printf("fila = ");
				scanf("%d", &file);
			}
			while(column > 10 || column < 1)
			{
				printf("columna = ");
				scanf("%d", &column);
			}
		}while(pieces[file - 1][column - 1] != 0);


		pieces[file - 1][column - 1] = 2; // save the user pivot
		if(checkGame(2, column - 1, file - 1, pieces))
		{
			printf("%s\n", "Ganaste");
			game_ended = true;
		}
		changeValues(column, file, values); //change the values to recalculate the fitness


		//CREATES THE POPULATION TO START THE GENETIC ALGORITHM
		createPopulation(population, pieces); //create the population each time the computer has to play
		calculateFitness(population, values);
		/*for(int i = 0; i < 10; i++) // print the start population
			printf("%d %d %d %d\n", population[i][0], population[i][1], population[i][2], population[i][3]);*/
		printf("\n");
		

		//IMPLEMENTATION OF THE GENETIC ALGORITHM
		for(int j = 0; j < 100; j++) //It does 15 iterations
		{
			for(int i = 0; i < 10; i += 2) // start the genetic algorithm
			{
				selection(population, momentary_population, pieces, i);
			}

			for(int i = 0; i < 10; i++) //replace the old population with the new one
			{
				for(int j = 0; j < 4; j++)
					population[i][j] = momentary_population[i][j];
			}
			calculateFitness(population, values);
			/*for(int i = 0; i < 10; i++) //print the new generated population
				printf("%d %d %d\n", population[i][0], population[i][1], population[i][2]);*/
		}
		//END OF GENETIC ALGORITHM

		//SELECTING COMPUTER'S MATCH
		match = 0;
		for(int i = 1; i  < 10; i++)
		{
			if(population[i][2] < population[match][2])
				match = i;
		}
		computer_x = population[match][0];
		computer_y = population[match][1];
		pieces[computer_y][computer_x] = 1; // save the user pivot
		if(checkGame(2, computer_x, computer_y, pieces))
		{
			printf("%s\n", "Perdiste");
			game_ended = true;
		}
		changeValues(column, file, values); //change the values to recalculate the fitness
	}

	//PRINT THE PIECES'S MATRIX
	printf("  1 2 3 4 5 6 7 8 9 10\n");
	for(int i = 0; i < 8; i++)
	{
		printf("%d", i + 1);
		for(int j = 0; j < 10; j++)
			printf(" %d", pieces[i][j]);
		printf("\n");
	}

	return 0;
}

/*Creates the new population each time the computer is going to play*/
void createPopulation(int population[][3], int pieces[][10])
{
	bool good_coordinate;
	for(int i = 0; i < 10; i++)
	{
		good_coordinate = false;
		while(good_coordinate != true)
		{
			population[i][0] = rand() % 10;
			population[i][1] = rand() % 8;
			if(pieces[population[i][1]][population[i][0]] != 1 && pieces[population[i][1]][population[i][0]] != 2)
				good_coordinate = true;
		}
	}
	return;
}

/*Calculates the fitness adding all the values around and in the coordinates saved in population to
use them before*/
void calculateFitness(int population[][3], int values[][12])
{
	for(int i = 0; i < 10; i++)
	{
		population[i][2] = values[population[i][1] + 1][population[i][0] + 1]
							+ values[population[i][1] + 1][population[i][0]]
							+ values[population[i][1] + 2][population[i][0]]
							+ values[population[i][1] + 2][population[i][0] + 1]
							+ values[population[i][1] + 2][population[i][0] + 2]
							+ values[population[i][1] + 1][population[i][0] + 2]
							+ values[population[i][1]][population[i][0] + 2]
							+ values[population[i][1]][population[i][0] + 1]
							+ values[population[i][1]][population[i][0]];
	}
}


/*Selects two gens creating a random number and selecting each gen if the acumulate probabilitie is 
greater than the random number*/
void selection(int population[][3], int momentary_population[][3], int pieces[][10], int position)
{
	int gen1[3], gen2[3];
	double right_limit, left_limit, temp;
	int g1, g2;
	right_limit = rand() % 10;
	left_limit = rand() % 10;
	if(right_limit > left_limit)
	{
		temp = right_limit;
		right_limit = left_limit;
		left_limit = temp;
	}
	g1 = right_limit;
	g2 = right_limit;
	for(int i = right_limit; i <= left_limit; i++)
	{
		if(population[i][2] < population[g1][2])
			g1 = i;
		if(population[i][2] < population[g2][2])
			g2 = i;
	}
	for(int i = 0; i < 3; i++)
		gen1[i] = population[g1][i];
	for(int i = 0; i < 3; i++)
		gen2[i] = population[g2][i];
    cruce(gen1, gen2, momentary_population, pieces, position);
}

void cruce(int gen1[], int gen2[], int momentary_population[][3], int pieces[][10], int position)
{
    int i, temp;
    double cruce, rnd;
    rnd = rand()%100;
    cruce = rnd / 100;
    if(cruce <= 0.5) //just do the cruce if the probabilitie us less than the 50%
    {
        temp = gen1[1];									//change the Y
        gen1[1] = gen2[1];								//coordinates of
        gen2[1] = temp;									//each gen
    }								
    if(pieces[gen1[1]][gen1[0]] != 0)
   		mutacion(gen1, momentary_population, pieces, position);
   	else if(pieces[gen2[1]][gen2[0]] != 0)
   		mutacion(gen2, momentary_population, pieces, position + 1);
   	else
   	{
   		for(int i = 0; i < 3; i ++)
   		{
			momentary_population[position][i] = gen1[i];
			momentary_population[position + 1][i] = gen2[i];
   		}
   	}
}

void mutacion(int gen[], int momentary_population[][3], int pieces[][10], int position)
{
	double location;
	double mutation;
	bool good_mutation = false;
	while(good_mutation == false)
	{
		location = rand() % 2;
		if(location == 1)
		{
			mutation = rand() % 8;
			if(pieces[(int)mutation][gen[0]] == 0)
			{
				gen[1] = location;
				good_mutation = true;
			}
		}
		if(location == 0)
		{
			mutation = rand() % 10;
			if(pieces[gen[1]][(int)mutation] == 0)
			{
				gen[0] = location;
				good_mutation = true;
			}

		}
	}
	for(int i = 0; i < 3; i ++)
		momentary_population[position][i] = gen[i];
}

void changeValues(int x, int y, int values[][12])
{
	values[y][x] = 0;
	values[y - 1][x] -= 1;
	values[y - 1][x + 1] -= 1;
	values[y][x + 1] -= 1;
	values[y + 1][x + 1] -= 1;
	values[y + 1][x] -= 1;
	values[y + 1][x - 1] -= 1;
	values[y][x - 1] -= 1;
	values[y - 1][x - 1] -= 1;
	if(x == 0)
	{
		values[y + 1][x - 1] -= 1;
		values[y][x - 1] -= 1;
		values[y - 1][x - 1] -= 1;
	}
	if(x == 9)
	{
		values[y - 1][x + 1] -= 1;
		values[y][x + 1] -= 1;
		values[y + 1][x + 1] -= 1;
	}
	if(y == 0)
	{
		values[y - 1][x] -= 1;
		values[y - 1][x + 1] -= 1;
		values[y - 1][x - 1] -= 1;
	}
	if(y == 7)
	{
		values[y + 1][x + 1] -= 1;
		values[y + 1][x] -= 1;
		values[y + 1][x - 1] -= 1;
	}
}

bool checkGame(int player, int x, int y, int pieces[][10])
{
	if(x == 0)
	{
		if(rightLine(player, x, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(rightLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 1)
	{
		if(rightLine(player, x, y, pieces)) return true;
		if(rightLine(player, x - 1, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
		}
		else if(y == 4)
		{
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 2)
	{
		if(rightLine(player, x, y, pieces)) return true;
		if(rightLine(player, x - 1, y, pieces)) return true;
		if(rightLine(player, x - 2, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 3)
	{
		if(rightLine(player, x, y, pieces)) return true;
		if(rightLine(player, x - 1, y, pieces)) return true;
		if(rightLine(player, x - 2, y, pieces)) return true;
		if(rightLine(player, x - 3, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomRightDiagonal(player, x , y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topRightDiagonal(player, x - 3, y + 3, pieces)) return true;
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 3, y - 3, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topRightDiagonal(player, x - 3, y + 3, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 3, y - 3, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 4)
	{
		if(rightLine(player, x, y, pieces)) return true;
		if(rightLine(player, x - 1, y, pieces)) return true;
		if(rightLine(player, x - 2, y, pieces)) return true;
		if(rightLine(player, x - 3, y, pieces)) return true;
		if(rightLine(player, x - 4, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 3, y - 3, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 3, y - 3, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topRightDiagonal(player, x - 3, y + 3, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(topLeftDiagonal(player, x + 3, y + 3, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 5)
	{
		if(leftLine(player, x, y, pieces)) return true;
		if(leftLine(player, x + 1, y, pieces)) return true;
		if(leftLine(player, x + 2, y, pieces)) return true;
		if(leftLine(player, x + 3, y, pieces)) return true;
		if(leftLine(player, x + 4, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomRightDiagonal(player, x, y, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 3, y - 3, pieces)) return true;
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 3, y - 3, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topRightDiagonal(player, x - 3, y + 3, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(topLeftDiagonal(player, x + 3, y + 3, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topRightDiagonal(player, x, y, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topRightDiagonal(player, x , y, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 6)
	{
		if(leftLine(player, x, y, pieces)) return true;
		if(leftLine(player, x + 1, y, pieces)) return true;
		if(leftLine(player, x + 2, y, pieces)) return true;
		if(leftLine(player, x + 3, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		if(y == 1)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 3, y - 3, pieces)) return true;
			if(bottomRightDiagonal(player, x - 1, y - 1, pieces)) return true;
			if(bottomRightDiagonal(player, x - 2, y - 2, pieces)) return true;
			if(bottomRightDiagonal(player, x - 3, y - 3, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(topLeftDiagonal(player, x + 3, y + 3, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topRightDiagonal(player, x - 3, y + 3, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 2, y + 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topRightDiagonal(player, x - 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 7)
	{
		if(leftLine(player, x, y, pieces)) return true;
		if(leftLine(player, x + 1, y, pieces)) return true;
		if(leftLine(player, x + 2, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 2, y + 2, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 2, y - 2, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 8)
	{
		if(leftLine(player, x, y, pieces)) return true;
		if(leftLine(player, x + 1, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true; 
		}
		else if(y == 4)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(bottomLeftDiagonal(player, x + 1, y - 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x + 1, y + 1, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
		}
	}
	else if(x == 9)
	{
		if(leftLine(player, x, y, pieces)) return true;

		if(y == 0)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
		}
		else if(y == 1)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
		}
		else if(y == 2)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
		}
		else if(y == 3)
		{
			if(bottomLeftDiagonal(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y, pieces)) return true;
			if(bottomLine(player, x, y - 1, pieces)) return true;
			if(bottomLine(player, x, y - 2, pieces)) return true;
			if(bottomLine(player, x, y - 3, pieces)) return true;
		}
		else if(y == 4)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
			if(topLine(player, x, y + 3, pieces)) return true;
		}
		else if(y == 5)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
			if(topLine(player, x, y + 2, pieces)) return true;
		}
		else if(y == 6)
		{
			if(topLeftDiagonal(player, x, y, pieces)) return true;
			if(topLine(player, x, y, pieces)) return true;
			if(topLine(player, x, y + 1, pieces)) return true;
		}
		else if(y == 7)
		{
			if(topLine(player, x, y, pieces)) return true;
			if(topLeftDiagonal(player, x, y, pieces)) return true;
		}
	}	
	return false;
}

bool topLine(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y - 1][x] == player && pieces[y - 2][x] == player 
		&& pieces[y - 3][x] == player && pieces[y - 4][x] == player)
		return true;
	else
		return false;
}

bool rightLine(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y][x + 1] == player && pieces[y][x + 2] == player 
		&& pieces[y][x + 3] == player && pieces[y][x + 4] == player)
		return true;
	else
		return false;
}

bool bottomLine(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y + 1][x] == player && pieces[y + 2][x] == player 
		&& pieces[y + 3][x] == player && pieces[y + 4][x] == player)
		return true;
	else
		return false;
}

bool leftLine(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y][x - 1] == player && pieces[y][x - 2] == player 
		&& pieces[y][x - 3] == player && pieces[y][x - 4] == player)
		return true;
	else
		return false;
}

bool topRightDiagonal(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y - 1][x + 1] == player && pieces[y - 2][x + 2] == player 
		&& pieces[y - 3][x + 3] == player && pieces[y - 4][x + 4] == player)
		return true;
	else
		return false;
}

bool topLeftDiagonal(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y - 1][x - 1] == player && pieces[y - 2][x - 2] == player 
		&& pieces[y - 3][x - 3] == player && pieces[y - 4][x - 4] == player)
		return true;
	else
		return false;
}

bool bottomRightDiagonal(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y + 1][x + 1] == player && pieces[y + 2][x + 2] == player 
		&& pieces[y + 3][x + 3] == player && pieces[y + 4][x + 4] == player)
		return true;
	else
		return false;
}

bool bottomLeftDiagonal(int player, int x, int y, int pieces[][10])
{
	if(pieces[y][x] == player && pieces[y + 1][x - 1] == player && pieces[y + 2][x - 2] == player 
		&& pieces[y + 3][x - 3] == player && pieces[y + 4][x - 4] == player)
		return true;
	else
		return false;
}
