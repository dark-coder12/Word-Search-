#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

// to sort words in a descending order to save largest first

void Sort(char grid[][50], int n_Of_Words)
{
	for (int i = 1; i < n_Of_Words; i++)
	{
		for (int j = 0; j < n_Of_Words - i; j++)
		{
			if (strlen(grid[j]) < strlen(grid[j + 1]))                        // sorts words array by descending order
			{
				swap(grid[j], grid[j + 1]);
			}
		}
	}
}

// to save words in a 2D char ptr array for Create Grid Option

char** ListOfWords(ifstream& fileName, int given_Row, int given_Col, int n_Of_Words)
{
	{
		char grid[50][50], word_Buffer[100];                                   // buffer saves iterated words
																			   // grid is a 2D array of words

		int row_Num = 0;                                                       // row iterator (input file)

		while (!fileName.eof())
		{
			fileName.getline(word_Buffer, 100, '\n');


			for (int j = 0, k = 0; j < strlen(word_Buffer); j++, k++)
			{
				if (word_Buffer[k] == ' ')
				{
					k++;
				}

				grid[row_Num][j] = word_Buffer[k];                             // saves word in 2D array

			}

			grid[row_Num][strlen(word_Buffer)] = '\0';                        // null char to mark end of word

			row_Num++;
		}

		Sort(grid, n_Of_Words);

		int largest_Word = strlen(grid[0]);                                  // largest word is in [0] (descending order)

		if (largest_Word > given_Row || largest_Word > given_Col)
		{
			cout << "Grid cannot be formed, press any key to return to the home screen" << endl;
			return 0;
		}

		char** list = new char* [n_Of_Words];                                 // 2D temp words array is copied to heap

		for (int i = 0; i < n_Of_Words; i++)
		{
			int col_Count = 0;

			for (int j = 0; grid[i][j] != '\0'; j++)
			{
				col_Count++;                                                 // ensures no extra memory taken on heap
			}

			list[i] = new char[col_Count + 1];

			for (int j = 0; j < col_Count; j++)
			{
				list[i][j] = grid[i][j];
			}
			list[i][col_Count] = '\0';
		}
		fileName.close();

		return list;                                                        // returns 2D words dynamic array
	}

}


// initializes grid to blank spaces

void Initialize(char** Grid, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			Grid[i][j] = ' ';
		}
		Grid[i][col] = '\0';
	}

}

// saves grid in user given file

void SaveGrid(ofstream& save, char** array, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			save << array[i][j] << "  ";
		}
		save << endl;
	}
}

// following are 8 conditions to save vertically / horizontally and either up / down
// or diagonally up/down and either left / right
// also checks if the pathway for a word is clear in a particular direction

bool DiagonalTopLtoR(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow + strlen(Words), cols = randCol + strlen(Words);

			int check = 0;

			if (rows <= given_Row && cols <= given_Col)
			{
				for (int i = randRow, j = randCol, k = 0; i < rows && j < cols; i++, j++, k++)
				{

					if (Grid[i][j] == ' ' || Grid[i][j] == Words[k])
					{
						check++;                                             // checks if a space = length of word is free
					}
				}
			}
			if (check == strlen(Words))
			{
				int j = randCol;
				int k = 0;

				for (int i = randRow, k = 0, j = randCol; i < rows; i++, j++, k++)
				{
					Grid[i][j] = Words[k];                                   // saves into grid if space is free

				}
				return true;                                                // returns true if successful
			}

		}
	}
	return false;
}

bool DiagonalTopRtoL(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow + strlen(Words), cols = randCol - strlen(Words);

			int check = 0;

			if (rows < given_Row && cols >= 0)
			{
				for (int i = randRow, j = randCol, k = 0; i < rows; i++, j--, k++)
				{
					if (Grid[i][j] == ' ' || Grid[i][j] == Words[k])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int j = randCol;
				int k = 0;

				for (int i = randRow, k = 0, j = randCol; i < rows; i++, j--, k++)
				{
					Grid[i][j] = Words[k];

				}
				return true;
			}

		}
	}
	return false;
}


bool DiagonalDownLtoR(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow - strlen(Words), cols = randCol + strlen(Words);

			int check = 0;

			if (rows >= 0 && cols <= given_Col)
			{
				for (int i = randRow, j = randCol, k = 0; i > rows; i--, j++, k++)
				{
					if (Grid[i][j] == ' ' || Grid[i][j] == Words[k])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{

				int j = randCol;
				int k = 0;

				for (int i = randRow, k = 0, j = randCol; i > rows; i--, j++, k++)
				{
					Grid[i][j] = Words[k];

				}
				return true;
			}

		}
	}
	return false;
}

bool DiagonalDownRtoL(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow - strlen(Words), cols = randCol - strlen(Words);

			int check = 0;

			if (rows >= 0 && cols >= 0)
			{
				for (int i = randRow, j = randCol, k = 0; i > rows; i--, j--, k++)
				{
					if (Grid[i][j] == ' ' || Grid[i][j] == Words[k])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{

				int j = randCol;
				int k = 0;

				for (int i = randRow, k = 0, j = randCol; i > rows; i--, j--, k++)
				{
					Grid[i][j] = Words[k];

				}
				return true;
			}

		}
	}
	return false;
}

bool HorizontalLeft(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow, cols = randCol + strlen(Words);

			int check = 0;

			if (rows <= given_Row && cols <= given_Col)
				for (int j = randCol, k = 0; j < cols; j++, k++)
				{
					if (Grid[randRow][j] == ' ' || Grid[randRow][j] == Words[k])
					{
						check++;
					}
				}

			if (check == strlen(Words))
			{
				int k = 0;


				for (int j = randCol; j < cols; j++, k++)
				{
					Grid[randRow][j] = Words[k];

				}
				return true;
			}

		}
	}
	return false;
}

bool HorizontalRight(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow, cols = randCol - strlen(Words);

			int check = 0;

			if (rows <= given_Row && cols >= 0)
			{

				for (int j = randCol, k = 0; j > cols; j--, k++)
				{
					if (Grid[randRow][j] == ' ' || Grid[randRow][j] == Words[k])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int k = 0;

				for (int j = randCol; j > cols; j--, k++)
				{
					Grid[randRow][j] = Words[k];

				}
				return true;
			}

		}
	}
	return false;
}



bool VerticalUp(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow + strlen(Words), cols = randCol;

			int check = 0;

			if (rows <= given_Row && cols <= given_Col)
			{
				for (int j = randRow, k = 0; j < rows; j++, k++)
				{
					if (Grid[j][cols] == ' ' || Grid[j][cols] == Words[k])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int k = 0;

				for (int j = randRow; j < rows; j++, k++)
				{
					Grid[j][cols] = Words[k];

				}
				return true;
			}

		}
	}
	return false;
}

bool VerticalDown(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int I = 0; I < given_Row; I++)
	{
		for (int J = 0; J < given_Col; J++)
		{

			randRow = I, randCol = J;

			int rows = randRow - strlen(Words), cols = randCol;

			int check = 0;

			if (rows >= 0 && cols < given_Col)
			{
				for (int j = randRow, k = 0; j > rows; j--, k++)
				{
					if (Grid[j][cols] == ' ' || Grid[j][cols] == Words[k])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int k = 0;

				for (int j = randRow; j > rows; j--, k++)
				{
					Grid[j][cols] = Words[k];

				}
				return true;
			}
		}
	}
	return false;
}


// on the basis of random direction generation - words are saved in grid

void InputWordsinGrid(char** Grid, char** Words, int row, int col, int n_Of_Words)
{

	int check = 0;

	bool(*functions[8]) (char**, char*, int, int, int, int) =                     //  saves functions to generate them randomly later on
	{ HorizontalRight , DiagonalTopLtoR , VerticalUp , DiagonalTopRtoL , VerticalDown , DiagonalDownLtoR , HorizontalLeft , DiagonalDownRtoL };




	while (check < n_Of_Words)
	{

		if ((functions[rand() % 8])(Grid, Words[check], rand() % row, rand() % col, row, col) == true)
		{
			check++;                                                             // random function input on random rows , cols
		}
	}


}

// replaces remaining spaces with random character alphabets

void RandomGenerator(char** Grid, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int r_Char = rand() % 26 - 32;

			char c = 'a' + r_Char;

			if (Grid[i][j] == ' ')
			{
				Grid[i][j] = c;
			}
		}
	}
}

// saves the words to be searched for Search Grid option

char** List_Of_TestCases(ifstream& wordFile, int& rows, int& cols, int& test_Cases)
{
	wordFile >> rows;                                                           // reads dimensions from file
	wordFile >> cols;
	wordFile >> test_Cases;

	char tempBuffer[80];

	char** testCases = new char* [test_Cases];                                  // uses buffer to save words on heap

	for (int i = 0; i < test_Cases; i++)
	{
		int col_Count = 0;

		wordFile.getline(tempBuffer, 80, '\n');


		for (int j = 0; tempBuffer[j] != '\0'; j++)
		{
			col_Count++;                                                      // ensures no extra memory taken on heap
		}

		testCases[i] = new char[col_Count + 1];

		for (int j = 0; j < col_Count; j++)
		{
			if (tempBuffer[j] == ' ')
			{
				j++;
			}

			testCases[i][j] = tempBuffer[j];
		}
		testCases[i][col_Count] = '\0';
	}

	return testCases;
}


// makes a grid on heap without any extra memory use

char** DynamicGrid(char**& grid, int& rows, int& cols)
{
	grid = new char* [rows];

	for (int i = 0; i < rows; i++)
	{
		grid[i] = new char[cols];
	}

	return grid;
}

// saves grid from given user file by reading subsequent lines from file

char** Input_GridFromFile(ifstream& gridFile, char**& grid, int& rows, int& cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			gridFile >> grid[i][j];
		}
	}
	return grid;
}

// checks for words Horizontally and returns true if word is found and saves coordinates in file

bool HorizontalSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int cols)
{
	int check = 0;

	for (int r = 0, s_Col = startCol; r < strlen(Words) && s_Col < cols; r++, s_Col++)
	{
		if (Words[r] == grid[startRow][s_Col])
		{
			check++;                                                                       // horizontally forward
		}

	}
	if (check == strlen(Words))
	{

		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << startRow << " , " << (startCol + strlen(Words) - 1) << "}" << endl << endl;
		return true;
	}

	check = 0;

	for (int r = 0, s_Col = startCol; r < strlen(Words) && s_Col >= 0; r++, s_Col--)
	{
		if (Words[r] == grid[startRow][s_Col])
		{
			check++;                                                                    // horizontally backward
		}
	}
	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << startRow << " , " << (startCol - strlen(Words) + 1) << "}" << endl << endl;
		return true;
	}

	return false;
}


// checks for words vertically up and down , returns true if word is found and saves coordinates in file

bool VerticalSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows)
{
	int check = 0;

	for (int r = 0, s_Row = startRow; r < strlen(Words) && s_Row < rows; r++, s_Row++)
	{
		if (Words[r] == grid[s_Row][startCol])
		{
			check++;                                                                     // vertically down
		}
	}
	if (check == strlen(Words))
	{

		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow + strlen(Words) - 1) << " , " << startCol << "}" << endl << endl;
		return true;
	}

	check = 0;

	for (int r = 0, s_Row = startRow; r < strlen(Words) && s_Row >= 0; r++, s_Row--)
	{
		if (Words[r] == grid[s_Row][startCol])
		{

			check++;                                                                  // vertically up
		}
	}

	if (check == strlen(Words))
	{

		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow - strlen(Words) + 1) << " , " << startCol << "}" << endl << endl;
		return true;
	}

	return false;
}

// checks for words diagonally Up and LtoR / RtoL returns true if word is found and saves coordinates in file

bool DiagonalUpToDownSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows, int cols)
{

	int check = 0;

	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row < rows && s_Col < cols; r++, s_Row++, s_Col++)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                                   // Top to Bottom Diagonal, Left to Right
		}
	}

	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow + strlen(Words) - 1) << " , " << (startCol + strlen(Words) - 1) << "}" << endl << endl;
		return  true;
	}

	check = 0;


	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row < rows && s_Col >= 0; r++, s_Row++, s_Col--)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                                 // Top to Bottom Diagonal, Right to Left
		}
	}

	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow + strlen(Words) - 1) << " , " << (startCol - strlen(Words) + 1) << "}" << endl << endl;
		return true;
	}

}


// checks for words diagonally Down LtoR / RtoL, returns true if word is found and saves coordinates in file

bool DiagonalDownToUpSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows, int cols)
{

	int check = 0;


	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row >= 0 && s_Col < cols; r++, s_Row--, s_Col++)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                                    // Bottom to Top Diagonal , Left to Right
		}
	}

	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow - strlen(Words) + 1) << " , " << (startCol + strlen(Words) - 1) << "}" << endl << endl;
		return true;
	}

	check = 0;

	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row >= 0 && s_Col >= 0; r++, s_Row--, s_Col--)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                                    // Bottom to Top Diagonal , Right to Left
		}
	}

	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow - strlen(Words) + 1) << " , " << (startCol - strlen(Words) + 1) << "}" << endl << endl;
		return true;
	}

	return false;

}

// calls all checks to see if word is found either vertically, diagonally, or horizontally, or else saves not found

bool FindWordsIn2DGrid(ofstream& saveoutput, char** grid, char* Words, int rows, int cols)
{

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (Words[0] == grid[i][j])
			{
				int startRow = i;
				int startCol = j;

				if (HorizontalSearch(saveoutput, grid, Words, startRow, startCol, cols))
				{
					return true;
				}
				if (VerticalSearch(saveoutput, grid, Words, startRow, startCol, rows))
				{
					return true;
				}

				if (DiagonalUpToDownSearch(saveoutput, grid, Words, startRow, startCol, rows, cols) == true)
				{
					return true;
				}

				if (DiagonalDownToUpSearch(saveoutput, grid, Words, startRow, startCol, rows, cols) == true)
				{
					return true;
				}
			}
		}
	}
	saveoutput << "NOT FOUND" << endl << endl;

	return false;
}


// deallocation of memory

void Deallocate(char** array, int& size)
{
	for (int i = 0; i < size; i++)
	{
		delete array[i];                                                   // deletes acquired memory on heap
	}
	delete array;
}

//  makes sure files are text files

bool FileExtensionCheck(string fileName)
{
	string file_Extension = fileName.c_str();

	file_Extension = file_Extension.substr(file_Extension.length() - 4, 4);

	if (file_Extension != ".txt")
	{
		cout << "This is not a valid txt file";
		return false;
	}
	return true;
}

int main()
{

	cout << "                                       Software for Word Search                                                  " << endl << endl;

	cout << "                            The User Can Choose One Of The Following Options                                     " << endl << endl;


	for (int i = 0; ; i++)
	{

		cout << "1) Create Grid" << endl;
		cout << "2) Search Words from Grid" << endl;
		cout << "3) Quit" << endl;

		cout << endl << "Press C to create a grid, press S to search words from an existing grid, and press Q to quit" << endl;

		char choiceUser, key;

		cin >> choiceUser;

		if (choiceUser == 'C' || choiceUser == 'C' + 32)
		{
			string fileName, output_File;

			cout << "Enter the name of the input file containing the list of words to generate a Grid - one word per line" << endl;

			cin >> fileName;

			if (FileExtensionCheck(fileName.c_str()) == true)
			{

				ifstream input_Words(fileName.c_str());

				if (input_Words.is_open())
				{
					int n_Of_Words, row, col;

					cout << "Specify the number of words in the word input file" << endl;

					cin >> n_Of_Words;

					cout << "Specify the number of rows you require for the grid" << endl;

					cin >> row;

					cout << "Specify the number of columns you require for the grid" << endl;

					cin >> col;

					cout << "Specify the file name to save the generated grid in - add .txt at the end " << endl;

					cin >> output_File;

					ofstream generate_Grid(output_File.c_str());

					char** Words = ListOfWords(input_Words, row, col, n_Of_Words);                              // saves list of words from input file by user

					char** Grid = new char* [row];

					for (int i = 0; i < row; i++)
					{
						Grid[i] = new char[col + 1];                                                             // creates grid that can hold largest word Diag / Horiz / Vertically
					}

					Initialize(Grid, row, col);                                                                  // initializes Grid with Blank Spaces

					InputWordsinGrid(Grid, Words, row, col, n_Of_Words);                                         // inputs words in either of the 8 ways as mentioned above

					RandomGenerator(Grid, row, col);                                                              // randomly fills in the rest of the spaces

					SaveGrid(generate_Grid, Grid, row, col);                                                      // saves the generated grid in output.txt 

					Deallocate(Grid, row);

					Deallocate(Words, n_Of_Words);                                                               // frees memory on heap

					cout << "                                               Success in Saving!                                                           " << endl << endl;
					cout << "___________________________________________ Saved in " << output_File << " _________________________________________________" << endl << endl;

				}
				else
				{
					cout << "File could not be successfully opened";
					return 0;
				}

				cout << "The Home Screen can be accessed by pressing any key " << endl;
				cin >> key;
			}
			else
			{
				return 0;
			}
		}

		if (choiceUser == 'S' || choiceUser == 'S' + 32)
		{
			string word_File, grid_File, output_File2;


			cout << "Enter the name of the input file containing the words to be searched and the dimensions" << endl;

			cin >> word_File;
			cout << endl;
			if (FileExtensionCheck(word_File.c_str()) == true)
			{
				ifstream wordFile(word_File.c_str());

				cout << "Enter the name of the input file containing the grid " << endl;

				cin >> grid_File;
				cout << endl;

				if (FileExtensionCheck(grid_File.c_str()) == true)
				{
					ifstream gridFile(grid_File.c_str());

					if (gridFile.is_open() && wordFile.is_open())
					{
						cout << "Enter the name of the output file to save the co-ordinates " << endl;

						cin >> output_File2;
						cout << endl;

						if (FileExtensionCheck(output_File2.c_str()) == true)
						{
							ofstream saveOutput2(output_File2.c_str());

							int rows, cols, test_Cases;

							char** grid = nullptr, ** testCases = nullptr;

							testCases = List_Of_TestCases(wordFile, rows, cols, test_Cases);  // stores words to be searched in given grid

							DynamicGrid(grid, rows, cols);

							grid = Input_GridFromFile(gridFile, grid, rows, cols);

							for (int i = 0; i < test_Cases; i++)
							{
								FindWordsIn2DGrid(saveOutput2, grid, testCases[i], rows, cols);                   // iterates to find each word - saves cordinates in given file
							}

							Deallocate(grid, rows);                                                              // freeing memory on heap

							Deallocate(testCases, test_Cases);

							ifstream showOutput(output_File2.c_str());                                           // opens output file to display it's content on console

							char wordBuffer[100];

							while (!showOutput.eof())
							{
								showOutput.getline(wordBuffer, 100, '\n');

								cout << wordBuffer << endl;
							}

							cout << "                                             Success in Saving!                                                              " << endl << endl;
							cout << "________________________________________ Saved in " << output_File2 << "  ___________________________________________________" << endl << endl;


							cout << "The Home Screen can be accessed by pressing any key " << endl;
							cin >> key;
						}
					}
				}
				else
				{
					cout << "File could not be successfully opened";
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}

		if (choiceUser == 'Q' || choiceUser == 'Q' + 32)
		{
			cout << endl << "Programming is Exiting " << endl << endl;                                            // return 0 causes program to end

			return 0;
		}

	}
}





