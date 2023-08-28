// P7Q3.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int withFirstPrivate() {
    omp_set_num_threads(2);
    int a = 1;
    int b = 2;
    int c = 4;

#pragma omp parallel firstprivate(a)

    {
        a = a + b;
        printf("%d\n", a);
        printf("%d\n", b);
        printf("%d\n", c);

    }
    printf("Final a: %d\n", a);
    return 0;
}

int withoutFirstPrivate() {
    omp_set_num_threads(2);
    int a = 1;
    int b = 2;
    int c = 4;

#pragma omp parallel 

    {
        a = a + b;
        printf("%d\n", a);
        printf("%d\n", b);
        printf("%d\n", c);

    }
    printf("Final a: %d\n", a);
    return 0;
}

int main()
{
    withoutFirstPrivate();
    withFirstPrivate();
    
    return 0;
}
