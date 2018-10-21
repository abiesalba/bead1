#include <fstream>
#include <vector>
#include <future> // ASYNC, FUTURE
#include <iostream>

//Inplements a sudoku:
class Sudoku{

private:
    unsigned int _size; //sudoku's size
    std::vector<std::vector<unsigned int>> _matrix; //contains sudoku's matrix

public:
	//Getters:
    unsigned int getSize(){ return _size;}
    std::vector<std::vector<unsigned int>> getMatrix(){return _matrix;}

	//Constructors:
	Sudoku(unsigned int sudoku_size, std::vector<std::vector<unsigned int>> sudoku_matrix)
		: _size(sudoku_size), _matrix(sudoku_matrix){}
	Sudoku(){}

	//Prints the sudoku on the screen:
	void printSudoku(){
		//Print the size of the sudoku:
		std::cout<< this->getSize() <<std::endl;
		//Print the matrix of the sudoku:
		for(int i = 0; i < this->getSize(); ++i){
			for(int j = 0; j < this->getSize(); ++j){
				std::cout<< this->getMatrix()[i][j] << " ";
			}
			std::cout<<"\n";
		}
	}

};

//Reads a sudoku from file
Sudoku readSudoku(std::ifstream& input){
	unsigned int n; //size
	input >> n;
	std::vector<std::vector<unsigned int>> matrix(n);
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			unsigned int actual;
			input >> actual;
			matrix[i].push_back(actual);
		}
	}
	return Sudoku(n, matrix);
}

//Find out if a sudoku is valid
	bool isValid(Sudoku& sudoku){
		for(int i = 0; i < sudoku.getSize(); ++i){
			for(int j = 0; j < sudoku.getSize(); ++j){
				unsigned int actual = sudoku.getMatrix()[i][j];
				//std::cout<< actual <<std::endl;
				if(actual != 0){
					//std::cout<< "This isn't 0"<<std::endl;
					for(int col = j+1; col < sudoku.getSize(); ++col){
						if(sudoku.getMatrix()[i][col] == actual)	return false;
					}
					for(int row = i+1; row < sudoku.getSize(); ++row){
						if(sudoku.getMatrix()[row][j] == actual) return false;
					}
				}
			}
		}
		return true;
	}

//Writes a sudoku to a file:
void writeSudoku(std::ofstream& output, const std::vector<std::vector<unsigned int>>& matrix){
	for(int i = 0; i < matrix.size(); ++i){
		for(int j = 0; j < matrix[i].size(); ++j){
			std::cout<< matrix[i][j] << " ";
			output << matrix[i][j] << " ";
		}
		std::cout<< "\n";
		output << "\n";
	}
}

//Solves a sudoku:
std::vector<std::vector<unsigned int>> solveSudoku(Sudoku& sudoku){
	std::vector<std::vector<unsigned int>> solved_sudoku;
	if(!isValid(sudoku)){
		std::cout<< "it isn't ok'" <<std::endl;
		return sudoku.getMatrix();
	}
	return solved_sudoku;
}

int main()
{
	std::ifstream input("input.txt");

	//Read the number of sudokus:
	unsigned int sudoku_count;
	input >> sudoku_count;

	// Read all the data into Sudokus and strat to solve them
	std::vector<std::future<std::vector<std::vector<unsigned int>>>> results;
	Sudoku actual;

	for(int i = 0; i < sudoku_count; ++i){
		actual = readSudoku(input);
		//actual.printSudoku();
		//results.push_back(std::async(std::launch::async, solveSudoku, std::ref(actual)));
		solveSudoku(actual);
	}

	input.close();

	// Wait for the function to return, write the result into a file
	std::ofstream output("output.txt");

	for (std::future<std::vector<std::vector<unsigned int>>>& f : results){
		f.wait();
		writeSudoku(output, f.get());
	}
	output.close();

	return 0;
}
