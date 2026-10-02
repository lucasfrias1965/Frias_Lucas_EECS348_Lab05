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
#include <climits>
#include <iterator>

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

            vector<int> acc;

            for (size_t k = 0; k < matrix_n; k++){
               result[i][j] += (matrix1.at(i).at(k) * matrix2.at(k).at(j));
            }

            //push the acc value back
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

//rarrr c++ should have this already
//copied from anthony king
//https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string
string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
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

    //for every i that is matrix_n doubled
    for (size_t i = 0; i < matrix_n << 1; i++){

        //get our line
        getline(matrixFile, input_line);

        //turn it to this stringstream iterator
        stringstream ss(input_line);
        string word;
        vector<int> acc;
        //while we can still shove ss into word
        while (ss >> word) {
            //convert our std::string to a cstring and convert
            //that cstring to a int and push that int into our
            //row accumulator

            //trimming function works except 
            //when empty
            if (word.length() == 0) continue;
            word = trim(word);
            acc.push_back(atoi(word.c_str()));
        }
        //now we're done we decide which matrix to push that into
        //if we finished the first i >= matrix_n
        if (acc.size() != matrix_n){ cerr << "Error! Input does not match size\n"; exit(1);}
        if (i < matrix_n) matrix1.push_back(acc);
        else matrix2.push_back(acc);

        //display x and push it back
    }

    cout << "Matrix A:\n";
    //display matrix 1, using some auto c++ array bs i found on this stackoverflow post
    //https://stackoverflow.com/questions/9751932/displaying-contents-of-a-vector-container-in-c
    for (auto&& x : matrix1){for (auto&& y: x) cout << y << " "; cout << "\n";}

    //ahhh i hate this language
    cout << "Matrix B:\n";
    for (auto&& x : matrix2){for (auto&& y: x) cout << y << " "; cout << "\n";}

    //now we just display a+b
    cout << "A + B:\n";
    for (int i = 0; i < matrix_n; i++){
        //we do the computation for every element while we print is just because
        for (int j = 0; j < matrix_n; j++) cout << (matrix1.at(i).at(j) + matrix2.at(i).at(j)) << " ";
        cout << "\n";
    }

    cout << "A * B:\n";
    //get the multiplcation
    Matrix mult_a_b = square_matrix_mult(matrix_n, matrix1, matrix2);

    //display the values
    for (auto&& x : mult_a_b){for (auto&& y: x) cout << y << " "; cout << "\n";}

    //init our sums
    int main_di_sum = 0;
    int side_di_sum = 0;
    for (size_t i = 0; i < matrix_n; i++) main_di_sum += matrix1[i][i];
    for (size_t i = 0; i < matrix_n; i++) side_di_sum += matrix1[matrix_n - i-1][i];

    cout << "Diagonal sums for Matrix A\nMain diagonal sum: " << main_di_sum << "\nSecondary diagonal sum: " << side_di_sum << "\n";

    //make an identity matrix and then a temp
    Matrix identity_shift = make_i_matrix(matrix_n);
    vector<int> temp;

    //swap the two values
    cout << "Swap Matrix A's rows. Specify an index\nIndex 1: ";
    size_t swap_a, swap_b;
    bool has_error = false;
    cin >> swap_a; cout << "Index 2:";
    cin >> swap_b;
    if (0 > swap_a || swap_a >= matrix_n) has_error = true;
    if (0 > swap_b || swap_b >= matrix_n) has_error = true;
    if (has_error) {cerr << "Error, invalid input"; exit(1);}
    temp = identity_shift[swap_a];
    identity_shift[swap_a] = identity_shift[swap_b];
    identity_shift[swap_b] = temp;

    //row and column swap the values
    //opposite as for any given elementary row
    //operation E we can say that
    // E*A = that operation done on the rows
    // A*E = that operation done on the columns

    Matrix row_swapped = square_matrix_mult(matrix_n, identity_shift, matrix1);
    cout << "Swap Matrix A's columns. Specify an index\nIndex 1: ";
    cin >> swap_a; cout << "Index 2:";
    cin >> swap_b;
    if (0 > swap_a || swap_a >= matrix_n) has_error = true;
    if (0 > swap_b || swap_b >= matrix_n) has_error = true;
    if (has_error) {cerr << "Error, invalid input"; exit(1);}

    //we gotta do the same identity shift but just with the
    //different inputs
    identity_shift = make_i_matrix(matrix_n);
    temp = identity_shift[swap_a];
    identity_shift[swap_a] = identity_shift[swap_b];
    identity_shift[swap_b] = temp;

    Matrix col_swapped = square_matrix_mult(matrix_n, matrix1, identity_shift);

    cout << "\nhello\n";

    for (auto&& x : row_swapped){for (auto&& y: x) cout << y << " "; cout << "\n";}
    for (auto&& x : col_swapped){for (auto&& y: x) cout << y << " "; cout << "\n";}

    //update one index
    cout << "Update an element in Matrix A. Specify an index\n Row: ";
    cin >> swap_a; cout << " Column: ";
    cin >> swap_b;
    if (0 > swap_a || swap_a >= matrix_n) has_error = true;
    if (0 > swap_b || swap_b >= matrix_n) has_error = true;
    if (has_error) {cerr << "Error, invalid input"; exit(1);}
    cout << "Input: "; int new_element; cin >> new_element;

    matrix1[swap_a][swap_b] = new_element < INT_MAX && new_element > INT_MIN ? new_element : 0;


    for (auto&& x : matrix1){for (auto&& y: x) cout << y << " "; cout << "\n";}
    
    return 0;
}
