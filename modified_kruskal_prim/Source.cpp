#include <stdio.h>
#include <string.h>

#pragma region Initialize Stuff

//defined values
#define N 50
//used variables
int countConnections = 0;
int nodeSize = 0;
char node[N];
int distance[N];
char connection[N][N];
int matrix[N][N];
//initialize
char dump;
char c[N];
//conex checking
int marked[N];
int enterOnce = 0;
//output
FILE *out;

#pragma endregion

#pragma region Initialize Functions

//initializing stuff
void initializeOutput()
{
	out = fopen("output.txt", "w");
	if (out == NULL)
	{
		printf("Error opening file!\n");
	}
}

void closingOutput()
{
	printf("~~~~~~~Output written to file \"output.txt\"~~~~~~~\n\n");
	fclose(out);
}

int checkEquality(char a, char b)
{
	if (a == b)return 1;
	return 0;
}

void readFromFile()
{
	FILE *file;

	file = fopen("conex.txt", "r");
	if (file)
	{
		fscanf(file, "%d", &nodeSize);

		int i = 0;
		int count = 0;
		while (count < nodeSize)
		{
			if (i % 2 == 1)
			{
				fscanf(file, "%c", &node[count]);
				count++;
			}
			else fscanf(file, "%c", &dump);

			i++;
		}

		//delete null stuffies
		fgets(c, 100, file);

		while (fgets(c, 100, file) != NULL)
		{
			connection[countConnections][0] = c[0];
			connection[countConnections][1] = c[2];
			distance[countConnections] = (int)(c[4] - '0');
			countConnections++;
		}

		fclose(file);
	}
}

void buildMatrix()
{
	int i, j, k;

	for (i = 0; i < nodeSize; i++)
		for (j = 0; j < nodeSize; j++)
		{
			int current_i = i + 'A';
			int current_j = j + 'A';

			int ok = 0;
			for (k = 0; k < countConnections; k++)
			{
				if (checkEquality(connection[k][0], current_i) && checkEquality(connection[k][1], current_j) == 1)
				{
					matrix[i][j] = distance[k];
					ok = 1;
					break;
				}
				else if (checkEquality(connection[k][0], current_j) && checkEquality(connection[k][1], current_i) == 1)
				{
					matrix[i][j] = distance[k];
					ok = 1;
					break;
				}

			}

			if (ok == 0)
				matrix[i][j] = 0;
		}
}

void testingPurposes()
{
	printf("TESTING PURPOSES ONLY ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	int i, j;
	//testing purposess, show connections
	printf("nr. of nodes: %d\n", nodeSize);
	for (i = 0; i < countConnections; i++)
	{
		printf("%c %c %d\n", connection[i][0], connection[i][1], distance[i]);
	}
	printf("\nMatrix:\n");

	for (i = 0; i < nodeSize; i++)
	{
		for (j = 0; j < nodeSize; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	printf("END TESTING PURPOSES ONLY ~~~~~~~~~~~~~~~~~~~~~~\n\n");
	//END TESTING PURPOSES
}

int maxDistance()
{
	int maximum = 0;
	int i;
	for (i = 0; i < countConnections; i++)
		if (maximum < distance[i])maximum = distance[i];

	return maximum;
}

#pragma endregion

#pragma region Kruskal

//Kruskal algorithm
int searchConex(int conex[], int countConex, int value)
{
	int i;
	for (i = 0; i < countConex; i++)
	{
		if (conex[i] == value)return 0;
	}

	return 1;
}

void kruskalAlgorithm()
{
	printf("-Kruskal Algorithm:\n");
	//write to file
	fprintf(out, "-Kruskal Algorithm:\n");
	int globalMinimum = 0;
	int i;
	int type = 1;
	//set all visited to -1 (unvisited)
	int countVisited = 0;
	int visited[N];
	for (i = 0; i < N; i++)visited[i] = -1;
	int route[N][N];
	int countRoute = 0;


	while (type != 0)
	{
		int reassignValue = 0;
		int assignValue = 0;
		type = 0;
		int first = -1, second = -1;
		int minimum = maxDistance() + 1;

		for (i = 0; i < countConnections; i++)
		{
			//TYPE 1
			if (visited[connection[i][0] - 'A'] == -1 && visited[connection[i][1] - 'A'] == -1)
			{
				//printf("1 case| %d %d dis: %d\n", connection[i][0] - 'A', connection[i][1] - 'A', distance[i]);

				if (minimum > distance[i])
				{
					type = 1;
					minimum = distance[i];
					first = connection[i][0] - 'A';
					second = connection[i][1] - 'A';
				}
			}

			//TYPE 2
			else if ((visited[connection[i][0] - 'A'] == -1 && visited[connection[i][1] - 'A'] != -1) ||
				(visited[connection[i][0] - 'A'] != -1 && visited[connection[i][1] - 'A'] == -1))
			{
				//printf("2 case| %d %d dis: %d\n", connection[i][0] - 'A', connection[i][1] - 'A', distance[i]);

				if (minimum > distance[i])
				{
					type = 2;
					minimum = distance[i];

					if (visited[connection[i][0] - 'A'] == -1)
					{
						assignValue = visited[connection[i][1] - 'A'];
						first = connection[i][0] - 'A';
						second = connection[i][1] - 'A';
					}

					else
					{
						assignValue = visited[connection[i][0] - 'A'];
						first = connection[i][1] - 'A';
						second = connection[i][0] - 'A';
					}

					//printf("f %d a%d\n", first, assignValue);
				}
			}

			//TYPE 3
			else if (visited[connection[i][0] - 'A'] != -1 && visited[connection[i][1] - 'A'] != -1 &&
				visited[connection[i][0] - 'A'] != visited[connection[i][1] - 'A'])
			{
				//printf("3 case| %d %d dis: %d\n", connection[i][0] - 'A', connection[i][1] - 'A', distance[i]);

				if (minimum > distance[i])
				{
					type = 3;
					minimum = distance[i];

					first = connection[i][0] - 'A';
					second = connection[i][1] - 'A';
					assignValue = visited[connection[i][1] - 'A'];
					reassignValue = visited[connection[i][0] - 'A'];
					//printf("aici %d %d\n", connection[i][0] - 'A', connection[i][1] - 'A');
				}

			}

		}

		//if something changed
		if (type != 0)
		{
			if (type == 1)
			{
				countVisited++;
				visited[first] = countVisited;
				visited[second] = countVisited;
			}

			else if (type == 2)
			{
				visited[first] = assignValue;
			}

			else if (type == 3)
			{
				int j;
				for (j = 0; j < nodeSize; j++)if (visited[j] == assignValue)visited[j] = reassignValue;
			}

			//printf("min %d\n", minimum);
			//for (i = 0; i < nodeSize; i++)printf("%d ", visited[i]);
			//printf("\n");

			route[countRoute][0] = first;
			route[countRoute][1] = second;
			countRoute++;

			//printf("%c %c\n", first + 'A', second + 'A');
			globalMinimum += minimum;
		}
	}

	//for (i = 0; i < nodeSize; i++)printf("%d ", visited[i]);
	//printf("\nfinal minimum: %d\n", globalMinimum);

	//set how many conex nodes are there
	int countConex = 0;
	int conex[N];

	for (i = 0; i < nodeSize; i++)
	{
		if (searchConex(conex, countConex, visited[i]) == 1)
		{
			conex[countConex] = visited[i];
			countConex++;
		}
	}

	int count = 0;
	for (i = 0; i < countConex; i++)
	{
		count++;
		printf("%d.\n", count);
		//write to file
		fprintf(out, "%d.\n", count);
		int j;
		for (j = 0; j < countRoute; j++)
		{
			if (conex[i] == visited[route[j][0]] || conex[i] == visited[route[j][1]])
			{
				printf("%c %c\n", route[j][0] + 'A', route[j][1] + 'A');
				//write to file
				fprintf(out, "%c %c\n", route[j][0] + 'A', route[j][1] + 'A');
			}
		}
		printf("\n");
		//write to file
		fprintf(out, "\n");
	}
}

#pragma endregion

#pragma region Prim

//Prim algorithm
int closestDistance(char c, int visited[])
{
	int i;
	int minimum = maxDistance() + 1;

	for (i = 0; i < countConnections; i++)
	{
		if ((connection[i][0] == c && visited[connection[i][1] - 'A'] != 1) || (connection[i][1] == c&& visited[connection[i][0] - 'A'] != 1))
		{
			if (minimum > distance[i])minimum = distance[i];
		}
	}

	//if the current NODE is already visited then we return -1
	if (minimum > maxDistance())return -1;
	return minimum;
}

char getClosestNode(char c, int visited[])
{
	int minimum = maxDistance() + 1;
	char closest = NULL;

	int i;
	for (i = 0; i < countConnections; i++)
	{
		if ((connection[i][0] == c && visited[connection[i][1] - 'A'] != 1) || (connection[i][1] == c&& visited[connection[i][0] - 'A'] != 1))
		{
			if (minimum > distance[i])
			{
				minimum = distance[i];
				if (connection[i][0] != c)closest = connection[i][0];
				else closest = connection[i][1];
			}
		}
	}

	//if the current NODE is already visited then we return NULL
	if (minimum > maxDistance())return NULL;
	return closest;
}

void prim(char start)
{
	int i;
	int visited[N];
	for (i = 0; i < N; i++)visited[i] = 0;
	//set the starting visited node
	visited[start - 'A'] = 1;

	int globalMinimum = 0;
	int min = 0;

	while (min < maxDistance() + 1)
	{
		//printf("%d", nodeSize);
		//reset min and positions
		min = maxDistance() + 1;
		int position = -1;
		char current = NULL;

		//go through the nodes
		for (i = 0; i < nodeSize; i++)
		{
			//printf("%c ", node[i]);
			if (visited[i] == 1)
			{
				//printf("for %c -> %d %c\n", node[i], closestDistance(node[i], visited), getClosestNode(node[i], visited));
				if (closestDistance(node[i], visited) > -1)
					if (min > closestDistance(node[i], visited))
					{
						min = closestDistance(node[i], visited);
						position = getClosestNode(node[i], visited) - 'A';
						current = node[i];
					}
			}
		}

		//printf("chose %d from %c to %c\n\n", min, current, node[position]);
		printf("%c %c\n", current, node[position]);
		//write to file
		fprintf(out, "%c %c\n", current, node[position]);
		//check visited the visited node
		if (position > -1)visited[position] = 1;
		//add to global min
		if (min < maxDistance() + 1)globalMinimum += min;
	}

	//printf("Shortest route: %d\n", globalMinimum);
}

void depthSearching(int x)
{
	int i;

	marked[x] = 1; //=1
	for (i = 0; i < nodeSize; i++)
		if (matrix[x][i] != 0)
		{
			if (marked[i] == 0) //==0
				depthSearching(i);
		}

}

void depthCrossing()
{
	int i;
	for (i = 0; i < nodeSize; i++)
		marked[i] = 0; //=0

	for (i = 0; i < nodeSize; i++)
		if (marked[i] == 0) //==0
		{
			depthSearching(i);
			enterOnce++;

			printf("%d.\n", enterOnce);
			//write to file
			fprintf(out, "%d.\n", enterOnce);
			//the Prim Algorithm
			prim(node[i]);
		}
}

void checkConex()
{
	depthCrossing();
	//if (enterOnce>1)printf("\n - nu e conexa\n\n");
	//else printf("\n - e conexa\n\n");
}

void primAlgorithm()
{
	printf("-Prim Algorithm:\n");
	//write to file
	fprintf(out, "-Prim Algorithm:\n");
	checkConex();
}

#pragma endregion

int main()
{
	//initialize the output
	initializeOutput();
	//reading from file
	readFromFile();
	//build the matrtix
	buildMatrix();
	//call the Prim algorithm
	primAlgorithm();
	//call the Kruskal algorithm
	kruskalAlgorithm();
	//closing the output file
	closingOutput();

	return 0;
}