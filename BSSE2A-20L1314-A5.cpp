#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

// makes a grid on heap without any extra memory use

char** DynamicGrid(char**& grid, int& rows, int& cols)
{
	grid = new char* [rows];

	for (int i = 0; i < rows; i++)
	{
		grid[i] = new char[cols + 1];                     // + 1 for null char to mark end of grid 
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

// to sort words in a descending order to save largest word first

void Sort(char grid[][50], int n_Of_Words)
{
	for (int i = 1; i < n_Of_Words; i++)
	{
		for (int j = 0; j < n_Of_Words - i; j++)
		{
			if (strlen(grid[j]) < strlen(grid[j + 1]))              // sorts words array by descending order
			{
				swap(grid[j], grid[j + 1]);
			}
		}
	}
}

// to use a temporary buffer to make dynamic words list with no extra memory usage

char** DynamicWordList(char grid[][50], int& n_Of_Words)
{
	char** list = new char* [n_Of_Words];                                 // 2D temp words array is copied to heap

	for (int i = 0; i < n_Of_Words; i++)
	{
		int col_Count = 0;

		for (int j = 0; grid[i][j] != '\0'; j++)
		{
			col_Count++;                                          // ensures no extra memory taken on heap
		}

		list[i] = new char[col_Count + 1];

		for (int j = 0; j < col_Count; j++)
		{
			list[i][j] = grid[i][j];
		}
		list[i][col_Count] = '\0';
	}

	return list;
}


// returns true if largest word can be accomodated in the given rows / cols

bool GridFeasibility(int& largestWord, int& rows, int& cols)
{
	if (largestWord <= rows && largestWord <= cols)
	{
		return true;
	}
	return false;
}

// deallocation of heap acquired memory

void Deallocate(char** array, int& size)
{
	for (int i = 0; i < size; i++)
	{
		delete array[i];                                               // deletes both dimensions
	}
	delete array;
}

//  makes sure files are text format files

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

// to save words in a 2D char ptr array for Create Grid Option

char** ListOfWords(ifstream& fileName, int given_Row, int given_Col, int n_Of_Words, int& largestWord)
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
				grid[row_Num][j] = word_Buffer[k];                        // saves word in 2D array
			}

			grid[row_Num][strlen(word_Buffer)] = '\0';                       // null char to mark end of word

			row_Num++;
		}

		Sort(grid, n_Of_Words);

		largestWord = strlen(grid[0]);                                           // largest word is in [0] (descending order)

		if (GridFeasibility(largestWord, given_Row, given_Col) == false)
		{
			return 0;
		}

		fileName.close();

		char** wordList = DynamicWordList(grid, n_Of_Words);

		return wordList;                                                          // returns 2D words dynamic array
	}
}


// following are 8 conditions to save vertically / horizontally and either up / down
// or diagonally up/down and either left / right
// also checks if the pathway for a word is clear in a particular direction 
// or contains a mutually occuring letter

bool DiagonalTopLtoR(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow + strlen(Words), cols = randCol + strlen(Words);

			int check = 0;

			if (rows <= given_Row && cols <= given_Col)
			{
				for (int i = randRow, j = randCol, wordIterator = 0; i < rows && j < cols; i++, j++, wordIterator++)
				{
					if (Grid[i][j] == ' ' || Grid[i][j] == Words[wordIterator])
					{
						check++;                                 // checks if a space = length of word is free
					}
				}
			}
			if (check == strlen(Words))
			{
				int j = randCol, wordIterator = 0;

				for (int i = randRow, wordIterator = 0, j = randCol; i < rows; i++, j++, wordIterator++)
				{
					Grid[i][j] = Words[wordIterator];                // saves into grid if space is free

				}
				return true;                                            // returns true if successful
			}
		}
	}
	return false;
}

bool DiagonalTopRtoL(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow + strlen(Words), cols = randCol - strlen(Words);

			int check = 0;

			if (rows < given_Row && cols >= 0)
			{
				for (int i = randRow, j = randCol, wordIterator = 0; i < rows; i++, j--, wordIterator++)
				{
					if (Grid[i][j] == ' ' || Grid[i][j] == Words[wordIterator])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int j = randCol, wordIterator = 0;

				for (int i = randRow, wordIterator = 0, j = randCol; i < rows; i++, j--, wordIterator++)
				{
					Grid[i][j] = Words[wordIterator];

				}
				return true;
			}
		}
	}
	return false;
}

bool DiagonalDownLtoR(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow - strlen(Words), cols = randCol + strlen(Words);

			int check = 0;

			if (rows >= 0 && cols <= given_Col)
			{
				for (int i = randRow, j = randCol, wordIterator = 0; i > rows; i--, j++, wordIterator++)
				{
					if (Grid[i][j] == ' ' || Grid[i][j] == Words[wordIterator])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{

				int j = randCol, wordIterator = 0;

				for (int i = randRow, wordIterator = 0, j = randCol; i > rows; i--, j++, wordIterator++)
				{
					Grid[i][j] = Words[wordIterator];

				}
				return true;
			}
		}
	}
	return false;
}

bool DiagonalDownRtoL(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow - strlen(Words), cols = randCol - strlen(Words);

			int check = 0;

			if (rows >= 0 && cols >= 0)
			{
				for (int i = randRow, j = randCol, wordIterator = 0; i > rows; i--, j--, wordIterator++)
				{
					if (Grid[i][j] == ' ' || Grid[i][j] == Words[wordIterator])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{

				int j = randCol, wordIterator = 0;

				for (int i = randRow, wordIterator = 0, j = randCol; i > rows; i--, j--, wordIterator++)
				{
					Grid[i][j] = Words[wordIterator];

				}
				return true;
			}
		}
	}
	return false;
}

bool HorizontalLeft(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow, cols = randCol + strlen(Words);

			int check = 0;

			if (rows <= given_Row && cols <= given_Col)
			{

				for (int j = randCol, wordIterator = 0; j < cols; j++, wordIterator++)
				{
					if (Grid[randRow][j] == ' ' || Grid[randRow][j] == Words[wordIterator])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int wordIterator = 0;


				for (int j = randCol; j < cols; j++, wordIterator++)
				{
					Grid[randRow][j] = Words[wordIterator];

				}
				return true;
			}
		}
	}
	return false;
}

bool HorizontalRight(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow, cols = randCol - strlen(Words);

			int check = 0;

			if (rows <= given_Row && cols >= 0)
			{

				for (int j = randCol, wordIterator = 0; j > cols; j--, wordIterator++)
				{
					if (Grid[randRow][j] == ' ' || Grid[randRow][j] == Words[wordIterator])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int wordIterator = 0;

				for (int j = randCol; j > cols; j--, wordIterator++)
				{
					Grid[randRow][j] = Words[wordIterator];

				}
				return true;
			}
		}
	}
	return false;
}

bool VerticalUp(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{

	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow + strlen(Words), cols = randCol;

			int check = 0;

			if (rows <= given_Row && cols <= given_Col)
			{
				for (int j = randRow, wordIterator = 0; j < rows; j++, wordIterator++)
				{
					if (Grid[j][cols] == ' ' || Grid[j][cols] == Words[wordIterator])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int wordIterator = 0;

				for (int j = randRow; j < rows; j++, wordIterator++)
				{
					Grid[j][cols] = Words[wordIterator];

				}
				return true;
			}
		}
	}
	return false;
}

bool VerticalDown(char** Grid, char* Words, int randRow, int randCol, int given_Row, int given_Col)
{
	for (int gridRowIterator = 0; gridRowIterator < given_Row; gridRowIterator++)
	{
		for (int gridColIterator = 0; gridColIterator < given_Col; gridColIterator++)
		{
			randRow = gridRowIterator, randCol = gridColIterator;

			int rows = randRow - strlen(Words), cols = randCol;

			int check = 0;

			if (rows >= 0 && cols < given_Col)
			{
				for (int j = randRow, wordIterator = 0; j > rows; j--, wordIterator++)
				{
					if (Grid[j][cols] == ' ' || Grid[j][cols] == Words[wordIterator])
					{
						check++;
					}
				}
			}

			if (check == strlen(Words))
			{
				int wordIterator = 0;

				for (int j = randRow; j > rows; j--, wordIterator++)
				{
					Grid[j][cols] = Words[wordIterator];

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

	while (check < 1)
	{
		if (VerticalDown(Grid, Words[check], 0, 0, row, col) == true)
		{
			check++;
		}
	}

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

	char tempBuffer[50][50], wordBuffer[50];
	int row_Num = 0;

	while (!wordFile.eof())
	{
		wordFile.getline(wordBuffer, 100, '\n');

		for (int j = 0, k = 0; j < strlen(wordBuffer); j++, k++)
		{
			if (wordBuffer[k] == ' ')
			{
				k++;
			}

			tempBuffer[row_Num][j] = wordBuffer[k];                        // saves word in 2D array
		}

		tempBuffer[row_Num][strlen(wordBuffer)] = '\0';                        // null char to mark end of word

		row_Num++;
	}

	return DynamicWordList(tempBuffer, rows);
}

// checks for words Left to Right Horizontally and returns true if word is found and saves coordinates in file

bool HorizontalLtoRSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int cols)
{
	int check = 0;

	for (int r = 0, s_Col = startCol; r < strlen(Words) && s_Col < cols; r++, s_Col++)
	{
		if (Words[r] == grid[startRow][s_Col])
		{
			check++;                                                             // horizontally forward
		}

	}
	if (check == strlen(Words))
	{

		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << startRow << " , " << (startCol + strlen(Words) - 1) << "}" << endl << endl;
		return true;
	}

	return false;
}

// checks for words Right to Left Horizontally and returns true if word is found and saves coordinates in file

bool HorizontalRtoLSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int cols)
{
	int check = 0;

	for (int r = 0, s_Col = startCol; r < strlen(Words) && s_Col >= 0; r++, s_Col--)
	{
		if (Words[r] == grid[startRow][s_Col])
		{
			check++;                                                           // horizontally backward
		}
	}
	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << startRow << " , " << (startCol - strlen(Words) + 1) << "}" << endl << endl;
		return true;
	}

	return false;
}

// checks for words vertically top to bottom returns true if word is found and saves coordinates in file

bool VerticalTtoBSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows)
{
	int check = 0;

	for (int r = 0, s_Row = startRow; r < strlen(Words) && s_Row < rows; r++, s_Row++)
	{
		if (Words[r] == grid[s_Row][startCol])
		{
			check++;                                                               // vertically down
		}
	}
	if (check == strlen(Words))
	{

		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow + strlen(Words) - 1) << " , " << startCol << "}" << endl << endl;
		return true;
	}

	return false;
}

// checks for words vertically bottom to top returns true if word is found and saves coordinates in file

bool VerticalBtoTSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows)
{
	int check = 0;

	for (int r = 0, s_Row = startRow; r < strlen(Words) && s_Row >= 0; r++, s_Row--)
	{
		if (Words[r] == grid[s_Row][startCol])
		{

			check++;                                                                // vertically up
		}
	}

	if (check == strlen(Words))
	{

		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow - strlen(Words) + 1) << " , " << startCol << "}" << endl << endl;
		return true;
	}

	return false;
}

// checks for words diagonally - top left to bottom right and returns true if word is found and saves coordinates in file

bool DiagonalLeftUpToRightDownSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows, int cols)
{

	int check = 0;

	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row < rows && s_Col < cols; r++, s_Row++, s_Col++)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                             // Top to Bottom Diagonal, Left to Right
		}
	}

	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow + strlen(Words) - 1) << " , " << (startCol + strlen(Words) - 1) << "}" << endl << endl;
		return  true;
	}

	return false;
}

// checks for words diagonally - top right to bottom left and returns true if word is found and saves coordinates in file

bool DiagonalRightUpToLeftDownSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows, int cols)
{
	int check = 0;

	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row < rows && s_Col >= 0; r++, s_Row++, s_Col--)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                            // Top to Bottom Diagonal, Right to Left
		}
	}

	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow + strlen(Words) - 1) << " , " << (startCol - strlen(Words) + 1) << "}" << endl << endl;
		return true;
	}

	return false;
}

// checks for words diagonally - bottom left to top right and returns true if word is found and saves coordinates in file

bool DiagonalLeftDownToRightUpSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows, int cols)
{

	int check = 0;


	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row >= 0 && s_Col < cols; r++, s_Row--, s_Col++)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                            // Bottom to Top Diagonal , Left to Right
		}
	}

	if (check == strlen(Words))
	{
		save_C << "{" << startRow << " , " << startCol << "}  " << "{" << (startRow - strlen(Words) + 1) << " , " << (startCol + strlen(Words) - 1) << "}" << endl << endl;
		return true;
	}

	return false;
}

// checks for words diagonally - bottom right to top left and returns true if word is found and saves coordinates in file

bool DiagonalRightDownToLeftUpSearch(ofstream& save_C, char** grid, char* Words, int startRow, int startCol, int rows, int cols)
{
	int check = 0;

	for (int r = 0, s_Row = startRow, s_Col = startCol; r < strlen(Words) && s_Row >= 0 && s_Col >= 0; r++, s_Row--, s_Col--)
	{
		if (Words[r] == grid[s_Row][s_Col])
		{
			check++;                                                             // Bottom to Top Diagonal , Right to Left
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

				if (HorizontalLtoRSearch(saveoutput, grid, Words, startRow, startCol, cols) == true)
				{
					return true;
				}
				if (HorizontalRtoLSearch(saveoutput, grid, Words, startRow, startCol, cols) == true)
				{
					return true;
				}
				if (VerticalTtoBSearch(saveoutput, grid, Words, startRow, startCol, rows) == true)
				{
					return true;
				}
				if (VerticalBtoTSearch(saveoutput, grid, Words, startRow, startCol, rows) == true)
				{
					cout << Words << endl;
					return true;
				}
				if (DiagonalLeftUpToRightDownSearch(saveoutput, grid, Words, startRow, startCol, rows, cols) == true)
				{
					cout << Words << endl;
					return true;
				}
				if (DiagonalRightUpToLeftDownSearch(saveoutput, grid, Words, startRow, startCol, rows, cols) == true)
				{
					return true;
				}
				if (DiagonalLeftDownToRightUpSearch(saveoutput, grid, Words, startRow, startCol, rows, cols) == true)
				{
					return true;
				}
				if (DiagonalRightDownToLeftUpSearch(saveoutput, grid, Words, startRow, startCol, rows, cols) == true)
				{
					return true;
				}
			}
		}
	}
	saveoutput << "NOT FOUND" << endl << endl;

	return false;
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

		cout << endl << "Press C to create a grid, press S to search words from an existing grid, and press Q to quit" << endl << endl;

		cout << "Pressing any other key will cause the Word Search Home Screen to reappear " << endl << endl;

		cout << "Note to User: All of the Input & Output Files must be in txt format " << endl << endl;

		int largestWord = 0;

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

					cout << endl << "Specify the number of words in the word input file" << endl << endl;

					cin >> n_Of_Words;

					cout << endl << "Specify the number of rows you require for the grid" << endl << endl;

					cin >> row;

					cout << endl << "Specify the number of columns you require for the grid" << endl << endl;

					cin >> col;

					cout << endl << "Specify the file name to save the generated grid in " << endl << endl;

					cin >> output_File;

					ofstream generate_Grid(output_File.c_str());

					row++, col++;                                                                                     // for more variation if all words are largest

					char** words = ListOfWords(input_Words, row, col, n_Of_Words, largestWord);                      // saves list of words from input file by user

					if (GridFeasibility(largestWord, row, col) == true)
					{

						char** grid = DynamicGrid(grid, row, col);

						Initialize(grid, row, col);                                                                  // initializes Grid with Blank Spaces

						InputWordsinGrid(grid, words, row, col, n_Of_Words);                                         // inputs words in either of the 8 ways as mentioned above

						RandomGenerator(grid, row, col);                                                             // randomly fills in the rest of the spaces

						SaveGrid(generate_Grid, grid, row, col);                                                     // saves the generated grid in output.txt 

						Deallocate(grid, row);

						Deallocate(words, n_Of_Words);                                                               // frees memory on heap

						cout << "                                               Success in Saving!                                                           " << endl << endl;
						cout << "___________________________________________ Saved in " << output_File << " _________________________________________________" << endl << endl;
					}
					else
					{
						cout << "Grid cannot be formed - Largest Word can not be accomodated in given dimensions" << endl;
						return 0;
					}
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
				cout << "The Input / Output File is not of required txt format. General Format - 'InputFile.txt' " << endl << endl;

				return 0;
			}
		}

		if (choiceUser == 'S' || choiceUser == 'S' + 32)
		{
			string word_File, grid_File, output_File2;

			cout << "Enter the name of the input file containing the words to be searched and the dimensions" << endl << endl;

			cin >> word_File;

			cout << endl << endl;

			if (FileExtensionCheck(word_File.c_str()) == true)
			{
				ifstream wordFile(word_File.c_str());

				cout << "Enter the name of the input file containing the grid " << endl;

				cin >> grid_File;

				cout << endl << endl;

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

							testCases = List_Of_TestCases(wordFile, rows, cols, test_Cases);                       // stores words to be searched in given grid

							DynamicGrid(grid, rows, cols);

							grid = Input_GridFromFile(gridFile, grid, rows, cols);

							for (int i = 0; i < test_Cases; i++)
							{
								FindWordsIn2DGrid(saveOutput2, grid, testCases[i], rows, cols);              // iterates to find each word - saves cordinates in given file
							}

							Deallocate(grid, rows);                                                              // freeing memory on heap

							Deallocate(testCases, test_Cases);

							saveOutput2.close();

							ifstream showOutput(output_File2.c_str());                                           // opens output file to display it's content on console

							char wordBuffer[100];

							while (!showOutput.eof())
							{
								showOutput.getline(wordBuffer, 500, '\n');

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
				cout << "The Input / Output File is not of required txt format. General Format - 'InputFile.txt' " << endl << endl;

				return 0;
			}
		}

		if (choiceUser == 'Q' || choiceUser == 'Q' + 32)
		{
			cout << endl << "Programming is Exiting " << endl << endl;                                        // return 0 causes program to end

			return 0;
		}

	}
}
