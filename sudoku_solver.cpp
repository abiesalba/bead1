#include <fstream>
#include <vector>
#include <future> // ASYNC, FUTURE
#include <iostream>

//Helper class for next and prev functions:
class Index{
private:
	int row_ind;
	int col_ind;
	bool is_valid;
public:
	int getRowInd(){return row_ind;}
	int getColInd(){return col_ind;}
	bool getIsValid(){return is_valid;}

	void setRowInd(int new_ind){row_ind = new_ind;}
	void setColInd(int new_ind){col_ind = new_ind;}
	void setIsValid(bool new_val){is_valid = new_val;}

	Index(int row, int col, bool valid): row_ind(row), col_ind(col), is_valid(valid){};
};

//Inplements a sudoku:
class Sudoku{

private:
    unsigned int _size; //sudoku's size
    std::vector<std::vector<unsigned int>> _matrix; //contains sudoku's matrix
	std::vector<std::vector<unsigned int>> _solution; //contains the solution if solveable

public:
	//Getters:
    unsigned int getSize(){ return _size;}
    std::vector<std::vector<unsigned int>> getMatrix(){return _matrix;}
	std::vector<std::vector<unsigned int>> getSolutionMatrix(){return _solution;}

	//Setters:
	void initializeSolutionMatrix(int size){
		this->_solution.resize(size);
		for(int i = 0; i < size; ++i){
			for(int j = 0; j < size; ++j){
				this->_solution[i].push_back(0);
			}
		}
	}
	void setSolutionMatrix(int i, int j, unsigned int value){_solution[i][j] = value;}

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

	//Prints the solved sudoku on the screen:
	void printSolutionSudoku(){
		for(int i = 0; i < this->_solution.size(); ++i){
			for(int j = 0; j < this->_solution.size(); ++j){
				std::cout<< _solution[i][j] << " ";
			}
			std::cout<<"\n";
		}
	}

//---------Functions to help solving the sudoku---------------

	//Find out if a sudoku is valid
	bool isValid(){
		for(int i = 0; i < this->getSize(); ++i){
			for(int j = 0; j < this->getSize(); ++j){
				unsigned int actual = this->getMatrix()[i][j];
				if(actual != 0){
					for(int col = j+1; col < this->getSize(); ++col){
						if(this->getMatrix()[i][col] == actual)	return false;
					}
					for(int row = i+1; row < this->getSize(); ++row){
						if(this->getMatrix()[row][j] == actual) return false;
					}
				}
			}
		}
		return true;
	}

	//Recursive function of solveing - returns true if solved the sudoku, false otherwise:
	bool solveSudoku(int row_ind, int col_ind){
		Index actual = this->getNext(row_ind, col_ind);
	
		while(actual.getIsValid()){
			row_ind = actual.getRowInd();
			col_ind = actual.getColInd();

			std::cout<< "row: " << row_ind << " col: " << col_ind << " valid: " <<
			actual.getIsValid() << " value: " << this->getMatrix()[row_ind][col_ind] <<std::endl;

			actual = this->getNext(row_ind, col_ind);
		}
		std::cout<< "row: " << row_ind << " col: " << col_ind << " valid: " <<
			actual.getIsValid() << " value: " << this->getMatrix()[row_ind][col_ind] <<std::endl;

		std::cout<< "Start to prev:" <<std::endl;
		actual = this->getPrev(row_ind, col_ind);
		
		while(actual.getIsValid()){
			row_ind = actual.getRowInd();
			col_ind = actual.getColInd();

			std::cout<< "row: " << row_ind << " col: " << col_ind << " valid: " <<
			actual.getIsValid() << " value: " << this->getMatrix()[row_ind][col_ind] <<std::endl;

			actual = this->getPrev(row_ind, col_ind);
		}
	}

	//Returns the next elemnt's index and true if it's exists and false if not:
	Index getNext(int row_ind, int col_ind){
	
		if(col_ind < this->getSize()-1){
			return Index(row_ind, ++col_ind, true);
		}else if(row_ind < this->getSize()-1){
			return Index(++row_ind, 0, true);
		}else{
			return Index(row_ind, col_ind, false);
		}
	}

	//Returns the previous elemnt's index and true if it's exists and false if not:
	Index getPrev(int row_ind, int col_ind){

		if(col_ind > 0){
			return Index(row_ind, col_ind-1, true);
		}else if(row_ind > 0){
			return Index(row_ind-1, this->getSize()-1, true);
		}else{
			return Index(row_ind, col_ind, false);
		}
	}

};


//Intialize the solveing and invites solveSudoku:
std::vector<std::vector<unsigned int>> initSolveSudoku(Sudoku sudoku){

	//Check if the sudoku can be solved:
	if(!sudoku.isValid()){
		//if can't be solved return it:
		return sudoku.getMatrix();
	}

	//Initialize the result matrix's elements to 0:
	sudoku.initializeSolutionMatrix(sudoku.getSize());

	//Initialize indexes and solution's matrix:
	int row_ind = 0;
	int col_ind = 0;
	sudoku.setSolutionMatrix(0, 0, sudoku.getMatrix()[0][0]);

	sudoku.solveSudoku(row_ind , col_ind);

	return sudoku.getSolutionMatrix();
}

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

//Writes a sudoku to a file:
void writeSudoku(std::ofstream& output, const std::vector<std::vector<unsigned int>>& matrix){
	for(int i = 0; i < matrix.size(); ++i){
		for(int j = 0; j < matrix[i].size(); ++j){
			output << matrix[i][j] << " ";
		}
		output << "\n";
	}
}

int main()
{
	std::ifstream input("input1.txt");

	//Read the number of sudokus:
	unsigned int sudoku_count;
	input >> sudoku_count;

	// Read all the data into Sudokus and strat to solve them
	std::vector<std::future<std::vector<std::vector<unsigned int>>>> results;
	Sudoku actual;

	for(int i = 0; i < sudoku_count; ++i){
		actual = readSudoku(input);
		//results.push_back(std::async(std::launch::async, initSolveSudoku, actual));
		initSolveSudoku(actual);
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
