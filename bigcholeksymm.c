#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*This code is parallel implementation of Cholesky decomposition of a symmetric matrix. It works for any rank=n.
Calcultions are done sequentionally column after column (only diagonal and below diagonal elements are calculated). Program is meant to 
be compiled with MPICC: unix command "mpicc cholesky.c -o cholesky -std=c99 -lm" will produce file "cholesky" which is meant to be run with n or more
processors.
*/
int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //Given below matrix is a 4x4 example but program works for any rank symmetric matrix 
  int n=5; //rank of the matrix
  int e=0;
  if (world_size>=n)
  {
	e=n;	
  }
  else 
  {
	e=world_size;	
  }
  double M[5][5] = {{81, 9, 36, 18, 9}, {9, 5, 14, 8, 5}, {36, 14, 50, 32, 23}, {18, 8, 32, 71, 45}, {9, 5, 23, 45, 55}}; //declaration of the matrix to be decomposed
  for (int k=0; k<n; k++)//k-column index (algorithm goes below of diagonal from left to right)
  {		
	if(world_rank == 0){
		//printing to screen initial matrix M
		if (k==0)
		{	
			printf("Matrix M=\n");
			for (int i = 0; i <n; i++){
				for (int j = 0; j <n; j++){
					printf("%f", M[i][j]);
					if(j!=n-1)
					{
						printf(", ");	
					}	
				}
				printf("\n");
			}			
		}	
		//0 procesor calculates every diagonal element
		for (int j=0; j<k; j++)
		{
			M[k][k]-=(M[k][j]*M[k][j]);	
		}
		M[k][k]=sqrt(M[k][k]);
		if (k<n-1)
		{
			for (int p=1; p<e; p++) //p-processor index to which 0 processor sends a given task
			{	
				for (int c=0; c<n; c++)//processor 0 sends matrix
				{
					for (int r=c; r<n; r++)
					{	
						MPI_Send(&M[r][c], 1, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
						printf("%d: Process %d send M(%d, %d) to process %d\n", k, 0, r, c, p);
					}
				}
				for (int l=0; l<n-k-p; l+=world_size-1)
				{
					MPI_Recv(&M[k+p+l][k], 1, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					printf("%d: Process %d recived M(%d, %d) from process %d\n", k, 0, k+p+l, k, p);
				}
			
			}
		}
		else if (k==n-1)
		{	//put 0's above diagonal
			for(int i=0; i<n-1; i++)
			{
				for(int j=i+1; j<n; j++)
				{
					M[i][j]=0;	
				}	
			}
			
			//print result to the screen
			printf("Cholesky decomposition of matrix M: L= \n");
			for (int i = 0; i <n; i++){
				for (int j = 0; j <n; j++){
					printf("%f", M[i][j]);
					if(j!=n-1)
					{
						printf(", ");	
					}
				}
				printf("\n");
			}
		}	
	}
	else if (world_rank<e){
		if (k<n-1)
		{
			for (int c=0; c<n; c++)//receving already calculated values
			{
				for (int r=c; r<n; r++)
				{
					MPI_Recv(&M[r][c], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					printf("%d: Process %d recived M(%d, %d) from process %d\n", k, world_rank, r, c, 0);
				}
			}
			for (int l=0; l<n-k-world_rank; l+=world_size-1)
			{
				for (int g=0; g<k; g++)//calculating non-diagonal elements concurrently
				{
					M[k+world_rank+l][k]-=M[k][g]*M[k+world_rank+l][g];		
				}	
				M[k+world_rank+l][k]/=M[k][k];
				MPI_Send(&M[k+world_rank+l][k], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
				printf("%d: Process %d send M(%d, %d) to process %d\n", k, world_rank, k+world_rank+l, k, 0);
			}
		}
	}   
  }	  
  MPI_Finalize();
}  
