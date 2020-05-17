#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  float M[2][2] = {
	{5, 2},
	{2, 1},
  };
  
	if(world_rank == 0){
		printf("Jestem procesorem: %d\n", world_rank);
		float a=M[0][1]/2;
		MPI_Send(&a, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);     
	}
	else if (world_rank==1){
		printf("Jestem procesorem: %d\n", world_rank);
		MPI_Recv(&a, 2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		M[0][1]=a;
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++){
				printf("%f ,", M[i][j]);
			}
			printf("\n");
		}	
	}
	  
	  
	  /*MPI_Send(&token, 1, MPI_BYTE, world_rank + 1, 0, MPI_COMM_WORLD); 
   MPI_Send(&tokenn, 2, MPI_BYTE, world_rank - 1, 0, MPI_COMM_WORLD);  
   MPI_Recv(&token, 1, MPI_BYTE, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   printf("Process %d received token %c from process %d\n", world_rank, token, world_rank - 1);
   MPI_Recv(&tokenn, 2, MPI_BYTE, world_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   printf("Process %d received token %c from process %d\n", world_rank, tokenn, world_rank + 1);  */     
  MPI_Finalize();
}  
