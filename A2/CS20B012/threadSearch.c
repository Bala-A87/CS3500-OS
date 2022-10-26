#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int count = 0;

struct sort_params
{
    int *arr;
    int n;
};

void assign_params_s(struct sort_params *s, int *arr, int n)
{
    s->arr = arr;
    s->n = n;
}

struct merge_params
{
    int *arr1, *arr2;
    int n1, n2;
};

void assign_params_m(struct merge_params *m, int *arr1, int n1, int *arr2, int n2)
{
    m->arr1 = arr1;
    m->n1 = n1;
    m->arr2 = arr2;
    m->n2 = n2;
}

struct count_params
{
    int *arr;
    int n;
    int element;
};

void assign_params_c(struct count_params *c, int *arr, int n, int element)
{
    c->arr = arr;
    c->n = n;
    c->element = element;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void *sort(void *params)
{
    struct sort_params *s_params = (struct sort_params*) params;
    int *arr = s_params->arr;
    int n = s_params->n;

    qsort(arr, n, sizeof(int), cmpfunc);
}

void *merge(void *params)
{
    struct merge_params *m_params = (struct merge_params*) params;
    int *arr1 = m_params->arr1;
    int n1 = m_params->n1;
    int *arr2 = m_params->arr2;
    int n2 = m_params->n2;

    int temp[n1+n2];
    int i1 = 0, i2 = 0, i = 0;
    while(i1 < n1 && i2 < n2)
    {
        if(arr1[i1] < arr2[i2])
        {
            temp[i] = arr1[i1];
            i1++;
            i++;
        }
        else
        {
            temp[i] = arr2[i2];
            i2++;
            i++;
        }
    }
    while(i1 < n1)
    {
        temp[i] = arr1[i1];
        i1++;
        i++;
    }
    while(i2 < n2)
    {
        temp[i] = arr2[i2];
        i2++;
        i++;
    }
    for(int i = 0; i < n1+n2; i++)
        arr1[i] = temp[i];
}

void *count_multiples(void *params)
{
    struct count_params *c_params = (struct count_params*) params;
    int *arr = c_params->arr;
    int n = c_params->n;
    int element = c_params->element;
    
    for(int i = 0; i < n; i++)
        if(arr[i] % element == 0)
            count++;
}

int main() {
    int asize;
    scanf("%d", &asize);
    int arr[asize];
    for(int i = 0; i < asize; i++)
        scanf("%d", arr+i);
    int N;
    scanf("%d", &N);
    int element;
    scanf("%d", &element);

    pthread_t sorting1, sorting2;
    struct sort_params s1, s2;
    assign_params_s(&s1, arr, asize/2);
    assign_params_s(&s2, arr+asize/2, asize-asize/2);
    pthread_create(&sorting1, NULL, sort, &s1);
    pthread_create(&sorting2, NULL, sort, &s2);
    pthread_join(sorting1, NULL);
    pthread_join(sorting2, NULL);

    pthread_t merger;
    struct merge_params m;
    assign_params_m(&m, arr, asize/2, arr+asize/2, asize-asize/2);
    pthread_create(&merger, NULL, merge, &m);
    pthread_join(merger, NULL);

    pthread_t searchcount[N];
    struct count_params c[N];
    for(int i = 0; i < N-1; i++)
        assign_params_c(&c[i], arr+i*(asize/N), asize/N, element);
    assign_params_c(&c[N-1], arr+(N-1)*(asize/N), asize - (N-1)*(asize/N), element);
    for(int i = 0; i < N; i++)
        pthread_create(&searchcount[i], NULL, count_multiples, &c[i]);
    for(int i = 0; i < N; i++)
        pthread_join(searchcount[i], NULL);
    
    printf("%d\n", count);
    return 0;
}
