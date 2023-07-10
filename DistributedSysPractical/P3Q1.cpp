#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
	//	//Paralllel region
	//#pragma omp parallel
	//	{
	//		int ID = omp_get_thread_num();
	//		printf("hello(%d)", ID);
	//		printf("world(%d)\n", ID);
	//	}
	//	return 0;


	//	//Shared Data 
	//	int x = 5;
	//	//Paralllel region
	//#pragma omp parallel
	//	{
	//		x = x + 1;
	//		printf("shared: X is %d\n", x);
	//	}
	//	//Final value of the x 
	//	printf("shared: X is %d\n", x);
	//	return 0;


		//Private Data 
	int x = 5;
	//Paralllel region
#pragma omp parallel
	{
		//Private variable 
		int x;
		x = 3;
		printf("private: X is %d\n", x);
	}
	//Final value of the x --> global (shared variable)
	printf("shared: X is %d\n", x);
	return 0;

}


