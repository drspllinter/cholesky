#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <iostream>
int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int n=3;
  double a;
  double b;
  double M[3][3] = {{4, 12, -16}, {12, 37, -43}, {-16, -43, 98}};
  
	if(world_rank == 0){
		printf("Jestem procesorem: %d\n", world_rank);
		printf("Przed dzialaiami: \n");
		for (int i = 0; i <n; i++){
			for (int j = 0; j <n; j++){
				printf("%f ,", M[i][j]);
			}
			printf("\n");
		}
		
		M[0][0]=sqrt(M[0][0]);
		a=M[0][0];
		for (int i=1; i<n; i++){
			MPI_Send(&a, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Recv(&b, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			M[i][0]=b;
		}
		for (int i=0; i<1; i++)
		{
			M[1][1]-=(M[1][i]*M[1][i]);	
		}	
		
		printf("Po dzialaniach: \n");
		for (int i = 0; i <n; i++){
			for (int j = 0; j <n; j++){
				printf("%f ,", M[i][j]);
			}
			printf("\n");
		}    
	}
	else if (world_rank<n){
		//printf("Jestem procesorem: %d\n", world_rank);
		MPI_Recv(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		b=M[world_rank][0]/a;
		MPI_Send(&b, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);	
	}     
  MPI_Finalize();
}  
