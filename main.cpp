/*

    <@
    (KU//
      "
LUCAS FRIAS PRESENTS
EECS 348 LAB 05
FOR THE UNIVERSITY OF KANSAS
RANDOM MATRIX CPP CODE
DESCRIPTION: GIVEN A PROPERLY
FORMATTED INPUT FILE DOES
SOME MATRIX STUFF.

*/


//lib inclusion, i use a lot
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <climits>
#include <cstdlib>

using namespace std;

//just declaring a standard type so i don't go crazy!!
typedef vector<vector<int>> Matrix;


//Matrix square_matrix_mult, given the size_t N of our matrices, two NxN matrixes (problem specified) we
//then product the product of both of their multiplication. Bounds checking is done by the size_t

Matrix square_matrix_mult(const size_t matrix_n, Matrix matrix1, Matrix matrix2){
    //there really isn't much to say for this because
    //it's the standard naive approach
    //only this is we have to init our array
    //with our size_n vectors so we can assign
    //the values and zero them out

    Matrix result(matrix_n, vector<int>(matrix_n, 0));
    for (size_t i = 0; i < matrix_n; i++){
        for (size_t j = 0; j < matrix_n; j++){
            for (size_t k = 0; k < matrix_n; k++){
               result[i][j] += (matrix1.at(i).at(k) * matrix2.at(k).at(j));
            }
        }
    }
    return result;
}

//make_i_matrix -> given a size_n of our matrix, will compute the matrix's identity matrix

static inline Matrix make_i_matrix(const size_t matrix_n){
    Matrix result;
    //for every single value between 0...matrix_n-1
    for (size_t i = 0; i < matrix_n; i++){
        //make a new row
        vector<int> row;
        //make that row our square matrix size
        row.resize(matrix_n, 0);
        //make the value at the iterator 1
        //so this way we form the identity at every spot
        row[i] = 1;
        //push it back to our row
        result.push_back(row);
    }
    return result;
}

//Problem 1 (part a): reads matrix_n lines of matrix_n ints each from the file into m
void readMatrix(ifstream& file, const size_t matrix_n, Matrix& m){
    for (size_t i = 0; i < matrix_n; i++){
        string input_line;
        getline(file, input_line);
        stringstream ss(input_line);
        string word;
        vector<int> acc;
        while (ss >> word) acc.push_back(atoi(word.c_str()));
        m.push_back(acc);
    }
}

//Problem 1 (part b): prints a matrix. reused by every later problem
void printMatrix(const Matrix& m){
    for (auto&& x : m){for (auto&& y: x) cout << y << " "; cout << "\n";}
}

//Problem 2: entry-by-entry addition, cij = aij + bij
Matrix addMatrices(const Matrix& matrix1, const Matrix& matrix2, const size_t matrix_n){
    Matrix result(matrix_n, vector<int>(matrix_n, 0));
    for (size_t i = 0; i < matrix_n; i++)
        for (size_t j = 0; j < matrix_n; j++)
            result[i][j] = matrix1.at(i).at(j) + matrix2.at(i).at(j);
    return result;
}

//Problem 4: main diagonal (trace) and secondary diagonal sums of a matrix
void diagonalSums(const Matrix& m, const size_t matrix_n, int& main_di_sum, int& side_di_sum){
    main_di_sum = 0;
    side_di_sum = 0;
    for (size_t i = 0; i < matrix_n; i++) main_di_sum += m[i][i];
    for (size_t i = 0; i < matrix_n; i++) side_di_sum += m[matrix_n - i-1][i];
}

//Problem 5: swap two rows. elementary row operation P*A, P = identity with rows i,j swapped
Matrix swapRows(const Matrix& m, const size_t matrix_n, const size_t i, const size_t j){
    if (i >= matrix_n || j >= matrix_n){ cerr << "Error, invalid input"; exit(1); }
    Matrix identity_shift = make_i_matrix(matrix_n);
    vector<int> temp = identity_shift[i];
    identity_shift[i] = identity_shift[j];
    identity_shift[j] = temp;
    return square_matrix_mult(matrix_n, identity_shift, m);
}

//Problem 6: swap two columns. elementary column operation A*P, P = identity with columns i,j swapped
//we gotta do our own fresh identity shift here, not reuse the row one
Matrix swapCols(const Matrix& m, const size_t matrix_n, const size_t i, const size_t j){
    if (i >= matrix_n || j >= matrix_n){ cerr << "Error, invalid input"; exit(1); }
    Matrix identity_shift = make_i_matrix(matrix_n);
    vector<int> temp = identity_shift[i];
    identity_shift[i] = identity_shift[j];
    identity_shift[j] = temp;
    return square_matrix_mult(matrix_n, m, identity_shift);
}

//Problem 7: update a single element in place, a'ij = new value
void updateElement(Matrix& m, const size_t matrix_n, const size_t i, const size_t j, const int value){
    if (i >= matrix_n || j >= matrix_n){ cerr << "Error, invalid input"; exit(1); }
    m[i][j] = value < INT_MAX && value > INT_MIN ? value : 0;
}



int main(){
    //prompt the user for a file name
    cout << "Enter input filename:"; char filename [128]; cin.get(filename, 128);
    ifstream matrixFile(filename); //try to load that filename
    //uh oh, we can't. so we have to return an error
    if (matrixFile.fail()){ cout << "Error reading file, program terminating\n"; return 1;}

    //get out string for the users prompt, and the size_t
    string input_line;
    //get the first string, and convert it to a size_t
    getline(matrixFile, input_line);
    const size_t matrix_n = stoul(input_line);

    //make our two cool cat matrixes
    Matrix matrix1, matrix2;

    //Problem 1: read both matrices, then print them
    readMatrix(matrixFile, matrix_n, matrix1);
    readMatrix(matrixFile, matrix_n, matrix2);

    cout << "Matrix A:\n";
    printMatrix(matrix1);

    //ahhh i hate this language
    cout << "Matrix B:\n";
    printMatrix(matrix2);

    //Problem 2: add
    cout << "A + B:\n";
    Matrix sum_a_b = addMatrices(matrix1, matrix2, matrix_n);
    printMatrix(sum_a_b);

    //Problem 3: multiply
    cout << "A * B:\n";
    Matrix mult_a_b = square_matrix_mult(matrix_n, matrix1, matrix2);
    printMatrix(mult_a_b);

    //Problem 4: diagonal sums of Matrix A
    int main_di_sum, side_di_sum;
    diagonalSums(matrix1, matrix_n, main_di_sum, side_di_sum);
    cout << "Diagonal sums for Matrix A\nMain diagonal sum: " << main_di_sum << "\nSecondary diagonal sum: " << side_di_sum << "\n";

    //Problem 5: swap two rows of Matrix A
    cout << "Swap Matrix A's rows. Specify an index\nIndex 1: ";
    size_t swap_a, swap_b;
    cin >> swap_a; cout << "Index 2:";
    cin >> swap_b;
    Matrix row_swapped = swapRows(matrix1, matrix_n, swap_a, swap_b);

    //Problem 6: swap two columns of Matrix A
    cout << "Swap Matrix A's columns. Specify an index\nIndex 1: ";
    cin >> swap_a; cout << "Index 2:";
    cin >> swap_b;
    Matrix col_swapped = swapCols(matrix1, matrix_n, swap_a, swap_b);

    cout << "\nhello\n";
    printMatrix(row_swapped);
    printMatrix(col_swapped);

    //Problem 7: update one element of Matrix A
    cout << "Update an element in Matrix A. Specify an index\n Row: ";
    cin >> swap_a; cout << " Column: ";
    cin >> swap_b;
    cout << "Input: "; int new_element; cin >> new_element;
    updateElement(matrix1, matrix_n, swap_a, swap_b, new_element);

    printMatrix(matrix1);
    return 0;
}
