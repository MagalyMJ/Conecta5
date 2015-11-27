#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

void createPopulation(int population[][4], int pieces[][10]); // create the population
void calculateFitness(int population[][4], int values_user[][12], int values_computer[][12]);
void calculateProbabilities(int population[][4], double probabilities[][4]);
void selection(double probabilities[][4], int population[][4], int momentary_population[][4], int pieces[][10], int position);
void cruce(int gen1[], int gen2[], int momentary_population[][4], int pieces[][10], int position);
void mutacion(int gen[], int momentary_population[][4], int pieces[][10], int position);
void changeValues(int x, int y, int values[][12]);
bool checkGame(bool user, int x, int y, int pieces[][10]);

int main()
{
	int values_user[10][12]; //save how many pieces lack to do a well game for the user
	int values_computer[10][12]; //save how many pieces lack to do a well game for the computer
	int pieces[8][10] = {0}; //save the position ocuppied for a piece (1 - computer, 2 - user)
	int population[10][4]; //save que population and his probability {x, y, computer, user}
	bool game_ended; //save if the game has finished
	double probabilities[11][4]; //save the probability for selection {computer, user}
	int momentary_population[10][4]; //save the individuals while create the new population
	int file, column; //save each coordinate of the user's game

	srand(time(NULL)); //initialize the seed

	//initialize values_computer and values_user with five
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			values_computer[i][j] = 5;
			values_user[i][j] = 5;
		}
	}

	//start the game
	//while(game_ended != true)
	for(int k = 0; k < 5; k++)
	{

		//GET THE USER COORDINATES
		file = 0; //initialize file
		column = 0; //initialize column
		printf("Dame las coordenadas de tu jugada: \n");
		while(file > 8 || file < 1)
		{
			printf("fila = ");
			scanf("%d", &file);
		}
		printf("\ncolumna = ");
		scanf("%d", &column);


		pieces[file - 1][column - 1] = 2; // save the user pivot
		changeValues(column, file, values_user); //change the values to recalculate the fitness

		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 12; j++)
			{
				printf("%d", values_user[i][j]);
			}
			printf("\n");
		}

		//CREATES THE POPULATION TO START THE GENETIC ALGORITHM
		createPopulation(population, pieces); //create the population each time the computer has to play
		calculateFitness(population, values_user, values_computer);
		/*for(int i = 0; i < 10; i++) // print the start population
			printf("%d %d %d %d\n", population[i][0], population[i][1], population[i][2], population[i][3]);*/
		calculateProbabilities(population, probabilities);
		printf("\n");
		

		//IMPLEMENTATION OF THE GENETIC ALGORITHM
		for(int j = 0; j < 15; j++) //It does 15 iterations
		{
			for(int i = 0; i < 10; i += 2) // start the genetic algorithm
			{
				selection(probabilities, population, momentary_population, pieces, i);
			}

			for(int i = 0; i < 10; i++) //replace the old population with the new one
			{
				for(int j = 0; j < 4; j++)
					population[i][j] = momentary_population[i][j];
			}
			calculateFitness(population, values_user, values_computer);
			/*for(int i = 0; i < 10; i++) //print the new generated population
				printf("%d %d %d %d\n", population[i][0], population[i][1], population[i][2], population[i][3]);*/
		}
		//END OF GENETIC ALGORITHM

		game_ended = true;
	}

	return 0;
}

/*Creates the new population each time the computer is going to play*/
void createPopulation(int population[][4], int pieces[][10])
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
void calculateFitness(int population[][4], int values_user[][12], int values_computer[][12])
{
	for(int i = 0; i < 10; i++)
	{
		population[i][2] = values_computer[population[i][1] + 1][population[i][0] + 1]
							+ values_computer[population[i][1] + 1][population[i][0]]
							+ values_computer[population[i][1] + 2][population[i][0]]
							+ values_computer[population[i][1] + 2][population[i][0] + 1]
							+ values_computer[population[i][1] + 2][population[i][0] + 2]
							+ values_computer[population[i][1] + 1][population[i][0] + 2]
							+ values_computer[population[i][1]][population[i][0] + 2]
							+ values_computer[population[i][1]][population[i][0] + 1]
							+ values_computer[population[i][1]][population[i][0]]; //gets the fitness to win
		population[i][3] = values_user[population[i][1] + 1][population[i][0] + 1]
							+ values_user[population[i][1] + 1][population[i][0]]
							+ values_user[population[i][1] + 2][population[i][0]]
							+ values_user[population[i][1] + 2][population[i][0] + 1]
							+ values_user[population[i][1] + 2][population[i][0] + 2]
							+ values_user[population[i][1] + 1][population[i][0] + 2]
							+ values_user[population[i][1]][population[i][0] + 2]
							+ values_user[population[i][1]][population[i][0] + 1]
							+ values_user[population[i][1]][population[i][0]]; //gets the fitness to block
	}
}

/*Calculate the probabilities adding all the fitness of each column and then dividing each between 
the total. At the same time it creates an acumulate of all the after probabilities generated and 
saves it in a new column each time it adds a new one*/
void calculateProbabilities(int population[][4], double probabilities[][4])
{
	double total_fitness_user = 0;
	double total_fitness_computer = 0;
	double add_computer = 0;
	double add_user = 0;
	for(int i = 0; i < 10; i++)
	{
		total_fitness_user += population[i][3];
		total_fitness_computer += population[i][2];
	}
	probabilities[10][0] = total_fitness_computer;
	probabilities[10][2] = total_fitness_user;
	for(int i = 0; i < 10; i++)
	{
		probabilities[i][0] = (double)population[i][2] / (double)total_fitness_computer;
		add_computer += probabilities[i][0];
		probabilities[i][1] = add_computer;
		probabilities[i][2] = (double)population[i][3] / (double)total_fitness_user;
		add_user += probabilities[i][2];
		probabilities[i][3] = add_user;
	}
}

/*Selects two gens creating a random number and selecting each gen if the acumulate probabilitie is 
greater than the random number*/
void selection(double probabilities[][4], int population[][4], int momentary_population[][4], int pieces[][10], int position)
{
	int i, j, h = 0, gen1[4], gen2[4];
    double r1, r2, g1, g2;
    r1 = rand()%101;
    g1 = r1 / 100;
    g2 = g1;
    j = 0;
    while(g1 > probabilities[j][1])
        j++;
    for(i = 0; i < 4; i++)
        gen1[i] = population[j][i];
    while(h == j || h == 0)
    {
        r2 = rand()%101;
        g2 = r2 / 100;
        h = 0;
        while(g2 > probabilities[h][1])
            h++;
        for(i = 0; i < 4; i++)
            gen2[i] = population[h][i];
    }
    cruce(gen1, gen2, momentary_population, pieces, position);
}

void cruce(int gen1[], int gen2[], int momentary_population[][4], int pieces[][10], int position)
{
    int i, temp;
    double cruce, rnd;
    rnd = rand()%100;
    cruce = rnd / 100;
    if(cruce <= 0.8) //just do the cruce if the probabilitie us less than the 80%
    {
        temp = gen1[1];									//change the Y
        gen1[1] = gen2[1];								//coordinates of
        gen2[1] = temp;									//each gen
        if(pieces[gen1[1]][gen1[0]] != 0)
    		mutacion(gen1, momentary_population, pieces, position);
    	else if(pieces[gen2[1]][gen2[0]] != 0)
    		mutacion(gen2, momentary_population, pieces, position + 1);
    	else
    	{
    		for(int i = 0; i < 4; i ++)
    		{
				momentary_population[position][i] = gen1[i];
				momentary_population[position + 1][i] = gen2[i];
    		}
    	}
    }
}

void mutacion(int gen[], int momentary_population[][4], int pieces[][10], int position)
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
	for(int i = 0; i < 4; i ++)
		momentary_population[position][i] = gen[i];
}

void changeValues(int x, int y, int values[][12])
{
	values[y][x] -= 1;
	values[y - 1][x] -= 1;
	values[y - 1][x + 1] -= 1;
	values[y][x + 1] -= 1;
	values[y + 1][x + 1] -= 1;
	values[y + 1][x] -= 1;
	values[y + 1][x - 1] -= 1;
	values[y][x - 1] -= 1;
	values[y - 1][x - 1] -= 1;
}

bool checkGame(bool user, int x, int y, int pieces[][10])
{
	return true;
}
