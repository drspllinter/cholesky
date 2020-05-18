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
  
  for (int k=0; k<n; k++)//k indeks kolumny (algorytm idzie po dolnej diagonali od L do P)
  {		  
	if(world_rank == 0){
		double M[3][3] = {{4, 12, -16}, {12, 37, -43}, {-16, -43, 98}};
		/*printf("Jestem procesorem: %d\n", world_rank);
		printf("Przed dzialaiami: \n");
		for (int i = 0; i <n; i++){
			for (int j = 0; j <n; j++){
				printf("%f ,", M[i][j]);
			}
			printf("\n");
		}*/
		
		for (int j=0; j<k; j++) //j indeks pomocniczy-do sumowania wyrazow
		{
			M[k][k]-=(M[k][j]*M[k][j]);	
		}
		M[k][k]=sqrt(M[k][k]);
		for (int p=1; p<n-k; p++) //p indeks procesora do ktorego wysylamy 
		{	
			for (int c=0; c<=k; c++)//c indeks columny elementow do wysylki 
			{
				for (int r=c; r<n; r++)//r indeks wiersza elementu do wysylki
				{	
					MPI_Send(&M[r][c], 1, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
				}
			}
		}	
			
		/*printf("Po dzialaniach: \n");
		for (int i = 0; i <n; i++){
			for (int j = 0; j <n; j++){
				printf("%f ,", M[i][j]);
			}
			printf("\n");
		}*/    
	}
	else if (world_rank<n){
		//printf("Jestem procesorem: %d\n", world_rank);
		for (int c=0; c<=k; c++)
		{
			for (int r=c; r<n; r++)
			{
				MPI_Recv(&M[r][c], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}		
	}   
  }	  
  MPI_Finalize();
}  
