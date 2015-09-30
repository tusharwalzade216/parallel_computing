#include <iostream>
#include <mpi.h>
#include <fstream>
#include <cstdlib>
using namespace std;
#define rows 8
#define cols 8

int main(int argc, char *argv[]) {
	int matrix[rows][cols], vector[rows], result[rows];
	fstream fin ("matrix2.txt");
	int npes, myrank, div, size, sum[2];
	sum[0] = 0, sum[1] = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  	MPI_Status status;

	div = rows / npes;
	size = div * cols;
	int matrix1[div][cols];
	//cout << "div : " << div << "\tsize : " << size << endl;

	if(myrank == 0) {
		for(int i = 0; i < rows; i++)
			for(int j = 0; j < cols; j++)
				fin >> matrix[i][j];

		for(int i = 0; i < rows; i++)
			fin >> vector[i];
	}
	
	MPI_Bcast(&div, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(MPI_Scatter(&matrix, size, MPI_INT, &matrix1, size, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
       		perror("Matrix Scatter error");
        	exit(1);
    	}
	
	if(MPI_Bcast(&vector, cols, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
       		perror("Vector Broadcast error");
        	exit(2);
    	}

	int ct = 0;
	cout <<"rank : " << myrank << endl;
	for(int i = 0; i < div; i++) {
		for(int j = 0; j < cols; j++)
			cout << "\t" << matrix1[i][j] << " ";
			if(ct < cols)
				cout << endl;
			ct++;
		//cout << vector[i] << endl;
		//cout << endl;
	}
	
	int i, j, ctr = 0;
	for(int i = 0; i < div; i++) {
		for(int j = 0; j < cols; j++) {
			if(ctr < cols)
				sum[0] += (matrix1[i][j] * vector[j]);
			else
				sum[1] += (matrix1[i][j] * vector[j]);
			ctr++;
		}
	}
	//cout << "sum : " << sum[0] << endl;
	//cout << "sum : " << sum[1] << endl;

	if(MPI_Gather(&sum, div, MPI_INT, &result, div, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
       		perror("Matrix Scatter error");
        	exit(1);
    	}
	
	if(myrank == 0) {
		cout << endl;
		for(int i = 0; i < rows; i++)
			cout << result[i] << endl;
	}

	MPI_Finalize();
	//exit(-1);	
	return 0;
}
