#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
/***********************************************************************
******* 

  Compile with:
    mpicc -o mpi_password_crack mpi_password_crack.c -lcrypt

    mpirun -n 3 ./mpi_password_crack

  Dinesh saud 1917493
************************************************************************
******/
int no_of_password = 4;

char *encrypted_passwords[] = {

"$6$KB$vHqb14t4t4Wv62e0clyeQW1tX8vOt/QpuGd2tN.b/FpPXWF6j9J0gA2sPAXBlBeppMeX4XY5h7GI6xeJ7Expk/",

"$6$KB$1ozgjSqeuX4grl/Wp4edMP2GRngtKonP/iBVkEo3gLw0Lj/dGihSi5n36Y0ar1iT7IJ02.brAs6Y//4FIWW06/",

"$6$KB$LihEs3s2zGRVhJbwpR2lPq6spchOFa0qGUj2d4xI7CqlRYLiy1cbZjNuF3KEuHbRprB9r1SJAtmiYkezpJ6x40",

"$6$KB$BBIz4MP5xGoTCpwvEkxkwsUnxNbKPXgPDCmdOK2iZ3g6PZV2Y3oQ3gtExrbo67smAdIXcjAUFRr7IPnPHN2zM/"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}



void function_1(char *salt_and_encrypted){
  int x, y, z,a;   
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='M'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        for(a=0; a<=99; a++){
        printf("1st_instance");
        sprintf(plain, "%c%c%02d%02d", x, y, z,a); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void function_2(char *salt_and_encrypted){
  int x, y, z,a;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='N'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        for(a=0; a<=99; a++){
        printf("2nd_instance");
        sprintf(plain, "%c%c%02d%02d", x, y, z,a); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn +=            1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  
  return !(*difference > 0);
}

int main(int argc, char *argv[]){

  struct timespec start, finish;
  long long int difference;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

 

int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){
      int no_of_password=4;
      int i;
      int return_response;
	MPI_Bcast(&no_of_password, 1, MPI_INT, 0, MPI_COMM_WORLD);  
	MPI_Recv(&return_response, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&return_response, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    } else {
      if(rank == 1){
	int i;
        int no_of_pass;
        int return_response=1;
        MPI_Bcast(&no_of_pass, 1, MPI_INT, 0, MPI_COMM_WORLD);
      	
	for ( i = 0; i<no_of_pass;i<i++){
		function_1(encrypted_passwords[i]);
	}
        
        MPI_Send(&return_response, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
	else if(rank == 2){
	int i;
        int no_of_pass;
        int return_response=1;
        MPI_Bcast(&no_of_pass, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for ( i = 0; i<no_of_pass;i<i++){
		function_2(encrypted_passwords[i]);
	}
        MPI_Send(&return_response, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
    }
  }
  MPI_Finalize(); 

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);

  printf("Elapsed Time: %9.5lfs\n", difference/1000000000.0);
 

  return 0;
}


