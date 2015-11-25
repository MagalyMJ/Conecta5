#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

void createPopulation(int population[][4], int pieces[][10]); // create the population
void calculateFitness(int population[][4], int values_user[][10], int values_computer[][10]);
void calculateProbabilities(int population[][4], double probabilities[][4]);
void selection(double probabilities[][4], int population[][4], int momentary_population[][4], int pieces[][10], int position);
void cruce(int gen1[], int gen2[], int momentary_population[][4], int pieces[][10], int position);
void mutacion(int gen[], int momentary_population[][4], int pieces[][10], int position);

int main()
{
	int values_user[8][10]; //save how many pieces lack to do a well game for the user
	int values_computer[8][10]; //save how many pieces lack to do a well game for the computer
	int pieces[8][10] = {0}; //save the position ocuppied for a piece
	int population[10][4]; //save que population and his probability {x, y, computer, user}
	bool game_ended; //save if the game has finished
	double probabilities[11][4]; //save the probability for selection {computer, user}
	int momentary_population[10][4]; //save the individuals while create the new population

	srand(time(NULL)); //initialize the seed

	//initialize values_computer and values_user with zero
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			values_computer[i][j] = 5;
			values_user[i][j] = 5;
		}
	}

	//start the game
	while(game_ended != true)
	{
		createPopulation(population, pieces); //create the population each time the computer has to play
		calculateFitness(population, values_user, values_computer);
		for(int i = 0; i < 10; i++)
			printf("%d %d %d %d\n", population[i][0], population[i][1], population[i][2], population[i][3]);
		calculateProbabilities(population, probabilities);
		printf("\n");

		for(int i = 0; i < 10; i += 2) // start the genetic algorithm
		{
			selection(probabilities, population, momentary_population, pieces, i);
		}

		for(int i = 0; i < 10; i++) //replace the old population with the new one
		{
			for(int j = 0; j < 4; j++)
				population[i][j] = momentary_population[i][j];
		}
		for(int i = 0; i < 10; i++)
			printf("%d %d %d %d\n", population[i][0], population[i][1], population[i][2], population[i][3]);
		game_ended = true;
	}

	return 0;
}

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

void calculateFitness(int population[][4], int values_user[][10], int values_computer[][10])
{
	for(int i = 0; i < 10; i++)
	{
		if(population[i][0] == 0)
		{
			if(population[i][1] == 0)
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1]][population[i][0] + 1]
								+ values_computer[population[i][1] + 1][population[i][0] + 1]
								+ values_computer[population[i][1] + 1][population[i][0]]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] + 1]
								+ values_user[population[i][1] + 1][population[i][0] + 1]
								+ values_user[population[i][1] + 1][population[i][0]]; //gets the fitness to block
			}
			else if(population[i][1] == 7)
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1]][population[i][0] + 1]
								+ values_computer[population[i][1] - 1][population[i][0] + 1]
								+ values_computer[population[i][1] - 1][population[i][0]]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] + 1]
								+ values_user[population[i][1] - 1][population[i][0] + 1]
								+ values_user[population[i][1] - 1][population[i][0]]; //gets the fitness to block
			}
			else
			{
				population[i][2] = values_computer[population[i][1]][population[i][1]]
								+ values_computer[population[i][1]][population[i][1] + 1]
								+ values_computer[population[i][1] - 1][population[i][1] + 1]
								+ values_computer[population[i][1] - 1][population[i][1]]
								+ values_computer[population[i][1] + 1][population[i][1] + 1]
								+ values_computer[population[i][1] + 1][population[i][1]]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] + 1]
								+ values_user[population[i][1] - 1][population[i][0] + 1]
								+ values_user[population[i][1] - 1][population[i][0]]
								+ values_user[population[i][1] + 1][population[i][0] + 1]
								+ values_user[population[i][1] + 1][population[i][0]]; //gets the fitness to block
			}
		}
		else if(population[i][0] == 9)
		{
			if(population[i][1] == 0)
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1]][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0]]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0]]; //gets the fitness to block
			}
			else if(population[i][1] == 7)
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1]][population[i][0] - 1]
								+ values_computer[population[i][1] - 1][population[i][0] - 1]
								+ values_computer[population[i][1] - 1][population[i][0]]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] - 1]
								+ values_user[population[i][1] - 1][population[i][0] - 1]
								+ values_user[population[i][1] - 1][population[i][0]]; //gets the fitness to block
			}
			else
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1] - 1][population[i][0]]
								+ values_computer[population[i][1] - 1][population[i][0] - 1]
								+ values_computer[population[i][1]][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0]]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1] - 1][population[i][0]]
								+ values_user[population[i][1] - 1][population[i][0] - 1]
								+ values_user[population[i][1]][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0]]; //gets the fitness to block
			}
		}
		else 
		{
			if(population[i][1] == 0)
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1]][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0]]
								+ values_computer[population[i][1] + 1][population[i][0] + 1]
								+ values_computer[population[i][1]][population[i][0] + 1]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0]]
								+ values_user[population[i][1] + 1][population[i][0] + 1]
								+ values_user[population[i][1]][population[i][0] + 1]; //gets the fitness to block
			}
			else if(population[i][1] == 7)
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1]][population[i][0] - 1]
								+ values_computer[population[i][1] - 1][population[i][0] - 1]
								+ values_computer[population[i][1] - 1][population[i][0]]
								+ values_computer[population[i][1] - 1][population[i][0] + 1]
								+ values_computer[population[i][1]][population[i][0] + 1]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] - 1]
								+ values_user[population[i][1] - 1][population[i][0] - 1]
								+ values_user[population[i][1] - 1][population[i][0]]
								+ values_user[population[i][1] - 1][population[i][0] + 1]
								+ values_user[population[i][1]][population[i][0] + 1]; //gets the fitness to block
			}
			else
			{
				population[i][2] = values_computer[population[i][1]][population[i][0]]
								+ values_computer[population[i][1]][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0] - 1]
								+ values_computer[population[i][1] + 1][population[i][0]]
								+ values_computer[population[i][1] + 1][population[i][0] + 1]
								+ values_computer[population[i][1]][population[i][0] + 1]
								+ values_computer[population[i][1] - 1][population[i][0] + 1]
								+ values_computer[population[i][1] - 1][population[i][0]]
								+ values_computer[population[i][1] - 1][population[i][0] - 1]; //gets the fitness to win
				population[i][3] = values_user[population[i][1]][population[i][0]]
								+ values_user[population[i][1]][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0] - 1]
								+ values_user[population[i][1] + 1][population[i][0]]
								+ values_user[population[i][1] + 1][population[i][0] + 1]
								+ values_user[population[i][1]][population[i][0] + 1]
								+ values_user[population[i][1] - 1][population[i][0] + 1]
								+ values_user[population[i][1] - 1][population[i][0]]
								+ values_user[population[i][1] - 1][population[i][0] - 1]; //gets the fitness to block
			}
		}
	}
}

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
    printf("gen1 %d %d %d %d\n", gen1[0], gen1[1], gen1[2], gen1[3]);
    printf("gen2 %d %d %d %d\n", gen2[0], gen2[1], gen2[2], gen2[3]);
    cruce(gen1, gen2, momentary_population, pieces, position);
}

void cruce(int gen1[], int gen2[], int momentary_population[][4], int pieces[][10], int position)
{
    int i, temp;
    double cruce, rnd;
    rnd = rand()%100;
    cruce = rnd / 100;
    printf("cruce %f\n", cruce);
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
    	printf("gen1 %d %d %d %d\n", gen1[0], gen1[1], gen1[2], gen1[3]);
    	printf("gen2 %d %d %d %d\n", gen2[0], gen2[1], gen2[2], gen2[3]);
    	printf("\n");
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