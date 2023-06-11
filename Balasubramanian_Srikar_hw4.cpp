#include <iostream>
#include <stdio.h>
#include <cmath>
//basic file operations
#include <fstream>
#include <iomanip>
using namespace std;
int** makeParticleMap(int, int, int, int, int, int, int, int, ofstream&);
int findMax(int**, int, int);
char normalizeMap(int**, int, int, int, int, int, int);
bool moveExists(int**, int, int, int, int);
bool checkdropZoneCoordinate(int, int, int, int); 
void startParticleIslandGeneratorProcess(); 
double frand();

/*
Name: Srikar Balasubramanian
Date: 11/4/2022
Description: Option #2: Procedural Island Generator 
This option has you creating an island/continent out of a method known as a “Particle Roll” Algorithm. 
Usage: exe s- seed
*/

//This function returns a list of valid moves that the particle can make. 
//Takes in parameters *arr, height, width, updated_x_coor, updated_y_coor, array of all valid moves, and length of valid moves array.
//Checks if x-coordinate and y-coordinate generated through the circle is < height and < width.
//If so, x-coordinate is added to randomly generated value from valid moves array
//and y-coordinate is added to randomly generated value from valid moves array. 
void listvalidmoves(int** arr, int height, int width, int updated_x_coor, int updated_y_coor, int valid_moves[8][2], int& size_of_valid_moves)
{

	const int arr_moves[8][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {-1, 1}, {1, 1}, {-1, -1}, {1, -1} };
	int index = 0;
	for (int i = 0; i < 8; i++)
	{
		//check if valid and push into valid_moves

		int move_x = arr_moves[i][0];
		int move_y = arr_moves[i][1];
		if (updated_x_coor + move_x >= 0 && updated_y_coor + move_y >= 0 &&
			updated_x_coor + move_x < height && updated_y_coor + move_y < width &&
			arr[updated_x_coor + move_x][updated_y_coor + move_y] <= arr[updated_x_coor][updated_y_coor])
		{
			valid_moves[index][0] = updated_x_coor + move_x;
			valid_moves[index][1] = updated_y_coor + move_y;
			size_of_valid_moves++;
			index++;
		}

	}

}

//This selects a random number which equates to the index that will be accessed in the array. 
//Basically, the particle will know which of the valid moves to choose based on this index. 
int select_randomMove(int sizeofValidMoves)
{

	return rand() % (sizeofValidMoves);

}


//This is the main function where the particle map is created. 
//Takes in parameters width, height, windowX, windowY, radius, numParticles, maxLife, waterLineValue, and an outFile. 
//Dynamic double pointer array which stores an array of pointers that have access to the values inside the array. 
//Therefore, to access those pointer arrays individually, a new array of size width is created for each array of pointers. 
//The function mainly uses two while loops with conditions numParticles > 0 and lifeofParticle > 0. 
//The dropZone coordinate is checked for every particle to ensure that it is not placed out of bounds. 
//Please keep in mind that this code does not support grids that are not of the same size. It will lead to a possible segmentation fault. 
//In the start, the original grid contains zeros. However, after this algorithm, depending on the parameters passed, the grid will contain 
//values > 1. 
//The output is stored through an output file and printed for the raw island, normalized island, and polished island through console/island.txt. 
int** makeParticleMap(int width, int height, int windowX, int windowY, int radius, int numParticles, int maxLife, int waterLineValue, ofstream& outFile) {
	const int SIZE_WIDTH = width;
	const int SIZE_HEIGHT = height;

	int** arr = new int* [SIZE_HEIGHT]; 

	for (int i = 0; i < SIZE_HEIGHT; i++) {
		arr[i] = new int[SIZE_WIDTH];
	}

	
	while (numParticles > 0) {
		//Drop zone calculation
		const double PI = 3.14159265358979323846;
		int the_radius = radius * sqrt(frand());
		double theta = frand() * 2 * PI;
		int updated_x_coor = (int)(windowX + the_radius * cos(theta));
		int updated_y_coor = (int)(windowY + the_radius * sin(theta));
		
		//Verifies that dropZoneCalculation is correct and that particle will not be placed out of bounds.
		if(checkdropZoneCoordinate(updated_x_coor, updated_y_coor, height, width) == false) 
		{
			cout << "-----------------------------------" << endl; 
			cout << "Particle will fall out of the grid." << endl; 
			cout << "Please provide correct x-coordinate, y-coordinate, and radius to place particle inside the grid." << endl;
			cout << "-----------------------------------" << endl; 
			startParticleIslandGeneratorProcess();
		}
		arr[updated_x_coor][updated_y_coor] += 1;
		int lifeofParticle = maxLife;

		while (lifeofParticle > 0)
		{
			int valid_moves[8][2] = { 0 }; // 8 directions , x-y values

			int size_of_valid_moves = 0;
			listvalidmoves(arr, height, width, updated_x_coor, updated_y_coor, valid_moves, size_of_valid_moves);

			if (size_of_valid_moves == 0 || lifeofParticle <= 0)
			{
				numParticles--;
				break;
			}
			else
			{
				int next_random_move = select_randomMove(size_of_valid_moves);

				//make move;
				int next_x = valid_moves[next_random_move][0];
				int next_y = valid_moves[next_random_move][1];
				++arr[next_x][next_y];

				--lifeofParticle;

				//update initial position
				updated_x_coor = next_x;
				updated_y_coor = next_y;

			}
		}
		--numParticles;

	}
	//Prints the raw island to the console and to island.txt
	for (int i = 0; i < SIZE_HEIGHT; ++i) {
		for (int j = 0; j < SIZE_WIDTH; ++j) {
			cout << setw(4) << arr[i][j]; 
			outFile << setw(4) << arr[i][j]; 
		}
		cout << endl;
		outFile << endl; 
	}
	cout << "Raw island shown." << endl; 
	outFile << "Raw island shown." << endl; 
	int maximum = findMax(arr, width, height); 
	//Uses the maximum here to divide by each item in the 2D-array.
	//This is for creating the normalized island. 
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			arr[i][j] = ((static_cast<double>(arr[i][j]) / static_cast<double>(maximum)) * 255); 
			cout << setw(4) << arr[i][j]; 
			outFile << setw(4) << arr[i][j]; 
		}
		cout << endl; 
		outFile << endl; 
	}
	cout << "Normalized island shown." << endl; 
	outFile << "Normalized island shown." << endl; 
	//Prints the normalized island to the console and to island.txt
	char** anArray = new char* [SIZE_HEIGHT]; 
	
	for(int i = 0; i < SIZE_HEIGHT; i++) {
		anArray[i] = new char[SIZE_WIDTH]; 
	}
	
	for (int i = 0; i < SIZE_HEIGHT; ++i) {
		for (int j = 0; j < SIZE_WIDTH; ++j) {
			anArray[i][j] = normalizeMap(arr, width, height, maximum, waterLineValue, i, j); 
			cout << anArray[i][j]; 
			outFile << anArray[i][j]; 
		}
		cout << endl;
		outFile << endl; 
	}
	//Prints the polished island to the console and to island.txt
	cout << "Polished island shown." << endl; 
	outFile << "Polished island shown." << endl; 
	outFile.close(); 
	
	for (int i = 0; i < height; i++) {
		delete[] arr[i];
	}
	delete[] arr; 
	
	for (int i = 0; i < height; i++) {
		delete[] anArray[i];
	}
	delete[] anArray;
	
	return 0;
}

//This function finds the max in the array after the raw island has been generated. 
int findMax(int** map, int width, int height) {
	int max = map[0][0];
	
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			if(map[i][j] > max) {
				max = map[i][j]; 
			}
		}
	}
	return max; 
}

//This function divides by the maximum in the array found and converts each of the numbers to the corresponding symbols shown below. 
//This could include deep water, shallow water, coast/beach, plains/grass, forests, and mountains. 
char normalizeMap(int** map, int width, int height, int maxVal, int waterLineValue, int i, int j) {
	int landZoneValue = 255 - waterLineValue; 
	if(map[i][j] < (0.5 * waterLineValue)) 
	{
		return '#'; //deep water
	}
	else if(map[i][j] > (0.5 * waterLineValue) && map[i][j] <= waterLineValue) 
	{
		return '~'; //shallow water
	}
	else 
	{
		if(map[i][j] < (waterLineValue + (0.15 * landZoneValue))) 
		{
			return '.'; //coast/beach
		}
		else if( (map[i][j] >= (waterLineValue + (0.15 * landZoneValue))) && (map[i][j] < (waterLineValue + (0.40 * landZoneValue))) )
		{
			return '-'; //plains/grass
		}
		else if( (map[i][j] >= (waterLineValue + (0.40 * landZoneValue))) && (map[i][j] < (waterLineValue + (0.80 * landZoneValue))) )
		{
			return '*'; //forests
		}
		else
		{
			return '^'; //mountains
		}
	}	
}

//This function basically generates a value between 0 and 1. 
//Returns a double in order to generate decimal value. 
double frand() {
	
	return (double)rand() / ((double)RAND_MAX + 1);
}

//This is the function to check the dropZoneCoordinate to check if particle is out of bounds. 
//Returns true if the x coordinate and y coordinate generated are not < height and width 
//If not, then returns false. 
bool checkdropZoneCoordinate(int updated_x_coor, int updated_y_coor, int height, int width) {
	if( (updated_x_coor >= 0 && updated_x_coor < height) && (updated_y_coor >= 0 && updated_y_coor < width) )
	{
		return true; 
	}
	return false; 
}

//This initiates the ParticleIslandGeneratorProcess where the inputs are asked of the user. 
//Defensive checks are given written for the x-coordinate, y-coordinate, and dropZoneRadius to ensure user is placing correct parameters. 
//Finally, after all the inputs have been given, the particle map function is called. 
void startParticleIslandGeneratorProcess() 
{
	cout << "Welcome to Srikar's CSE240 Island Generator." << endl;
	int width;
	int height;

	int x_coordinate;
	int y_coordinate;

	int drop_zoneRadius;
	int numParticles;
	int maxLifeParticles;
	int waterLineValue;
	
	bool checkXcoor = true; 
	bool checkYcoor = true; 
	bool checkDropZoneRadius = true; 
	
	ofstream outFile; 
	outFile.open("island.txt", ios::out); 

	cout << "Enter grid width:";
	cin >> width;
	cout << "Enter grid height:";
	cin >> height;
	
	while(checkXcoor) {
		cout << "Enter drop-zone x-coordinate (0 - <width>): "; 
		cin >> x_coordinate;
		if(x_coordinate < 0 || x_coordinate > height) {
			cout << "Error, width is not between the bounds of 0 and " << width << "specified. " << endl; 
			cout << "Please try again. " << endl; 
			checkXcoor = true;
		}else {
			checkXcoor = false; 
		}
	}
	
	while(checkYcoor) {
		cout << "Enter drop-zone y-coordinate (0 - <height>): ";
		cin >> y_coordinate;
		if(y_coordinate < 0 || y_coordinate > width) {
			cout << "Error, height is not between the bounds of 0 and " << height << "specified." << endl;
			cout << "Please try again. " << endl;
			checkYcoor = true;
		}else {
			checkYcoor = false;
		}
	}
	
	while(checkDropZoneRadius) {
		cout << "Enter drop-zone radius (minimum 2): ";
		cin >> drop_zoneRadius;
		if(drop_zoneRadius < 2) {
			cout << "Error. Please try again. Drop zone radius must be at least 2" << endl;
			checkDropZoneRadius = true;
		}else{
			checkDropZoneRadius = false;
		}
	}
	
	
	cout << "Enter number of particles to drop: ";
	cin >> numParticles;
	
	cout << "Enter max life of particles: ";
	cin >> maxLifeParticles;
	
	cout << "Enter value for waterline (40-200): ";
	cin >> waterLineValue;
	
	makeParticleMap(width, height, x_coordinate, y_coordinate, drop_zoneRadius, numParticles, maxLifeParticles, waterLineValue, outFile); 
}

//This sets the seed of srand at 0. 
//Starts the ParticleIslandGeneratorProcess
int main(int argc, char** argv) {
	srand(time(0)); 
	startParticleIslandGeneratorProcess(); 
	
	return 0;
}