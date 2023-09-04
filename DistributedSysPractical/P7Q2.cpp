// P7Q2.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif
#define MAX 10

struct node {
    int data;
    int fibdata;
    struct node* next;
};

int fib(int n) {
    int x, y;
    if (n < 2) {
        return (n);
    }
    else {
        x = fib(n - 1);
        y = fib(n - 2);
        return (x + y);
    }
}

void processwork(struct node* p)
{
    int n;
    n = p->data;
    p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;

    head = (struct node*)malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i = 0; i < N; i++) {
        temp = (struct node*)malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}

int serial() {
    double start, end;
    struct node* p = NULL;
    struct node* temp = NULL;
    struct node* head = NULL;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

    start = omp_get_wtime();
    {
        while (p != NULL) {
            processwork(p);
            p = p->next;
        }
    }
    end = omp_get_wtime();
    p = head;
    while (p != NULL) {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Compute Time: %f seconds\n", end - start);

    return 0;
}

//int parallel() {
//    double start, end;
//    struct node* p = NULL;
//    struct node* temp = NULL;
//    struct node* head = NULL;
//    struct node* parr[MAX];
//    int i, count = 0;
//
//    printf("Process linked list\n");
//    printf("  Each linked list node will be processed by function 'processwork()'\n");
//    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);
//
//    p = init_list(p);
//    head = p;
//
//    start = omp_get_wtime();
//
//#pragma omp parallel shared(count) private(i, temp)
//    {
//        int tid = omp_get_thread_num();
//        int num_threads = omp_get_num_threads();
//        struct node* local_head = NULL;
//        struct node* local_tail = NULL;
//
//        // Divide the linked list among threads
//        for (i = 0, temp = p; temp != NULL; i++, temp = temp->next) {
//            if (i % num_threads == tid) {
//                if (local_head == NULL) {
//                    local_head = temp;
//                    local_tail = temp;
//                }
//                else {
//                    local_tail->next = temp;
//                    local_tail = temp;
//                }
//                count++;
//            }
//        }
//
//        // Process the local linked list
//        while (local_head != NULL) {
//            processwork(local_head);
//            local_head = local_head->next;
//        }
//    }
//
//    end = omp_get_wtime();
//    p = head;
//
//    while (p != NULL) {
//        printf("%d : %d\n", p->data, p->fibdata);
//        temp = p->next;
//        free(p);
//        p = temp;
//    }
//    free(p);
//
//    printf("Compute Time: %f seconds\n", end - start);
//
//    return 0;
//}

//Chat gpt parallel
//int parallel() {
//    double start, end;
//    struct node* p = NULL;
//    struct node* temp = NULL;
//    struct node* head = NULL;
//    struct node* parr[MAX];
//    int i, count = 0;
//
//    printf("Process linked list\n");
//    printf("  Each linked list node will be processed by function 'processwork()'\n");
//    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);
//
//    p = init_list(p);
//    head = p;
//
//    // Create an array of pointers to the linked list nodes
//    i = 0;
//    while (p != NULL) {
//        parr[i++] = p;
//        p = p->next;
//        count++;
//    }
//
//    start = omp_get_wtime();
//
//#pragma omp parallel for shared(parr, count) private(i)
//    for (i = 0; i < count; i++) {
//        processwork(parr[i]);
//    }
//
//    end = omp_get_wtime();
//    p = head;
//
//    while (p != NULL) {
//        printf("%d : %d\n", p->data, p->fibdata);
//        temp = p->next;
//        free(p);
//        p = temp;
//    }
//    free(p);
//
//    printf("Compute Time: %f seconds\n", end - start);
//
//    return 0;
//}

int parallel() {
    double start, end;
    struct node* p = NULL;
    struct node* temp = NULL;
    struct node* head = NULL;
    struct node* parr[MAX];
    int i, count = 0;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

   
    start = omp_get_wtime();
    {
        while (p != NULL) {
            p = p->next;
            count++;
        }

        p = head;
        for (i = 0; i < count; i++) {
            parr[i] = p;
            p = p->next;
        }

        omp_set_num_threads(8);
#pragma omp parallel
        {
#pragma omp single
            printf(" %d threads \n", omp_get_num_threads());
#pragma omp for schedule(dynamic,5)
            for (i = 0; i < count; i++)
                processwork(parr[i]);
        }
    }
    end = omp_get_wtime();
    p = head;

    while (p != NULL) {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Parellel Compute Time: %f seconds\n", end - start);

    return 0;
}

int main(int argc, char* argv[]) {
    serial();
    printf("\n");
    parallel();
    return 0;
}