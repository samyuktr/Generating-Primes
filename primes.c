
/*
Samyukta Satish Rao
University of Southern California
ID : 5924873657
*/
#include "hw6.h"


void set(unsigned char* arr, unsigned int n)
{
    int byteNum = n/8;
    int bitNum = (n%8) + 1; 
    unsigned char mask = (unsigned char)1 << (8-bitNum);
    arr[byteNum] = arr[byteNum] | mask;
}

unsigned char check_val(unsigned char* arr, unsigned int n)
{
    int byteNum = n/8;
    int bitNum = (n%8) + 1; //1-based index in byte

    unsigned char mask = (unsigned char)1 << (8-bitNum);

    return (arr[byteNum] & mask)==mask;
}
void unset(unsigned char* array, unsigned int n)
{
    int byteNum = n/8;
    int bitNum = (n%8) + 1; //1-based index in byte

    unsigned char mask = ~((unsigned char)1 << (8-bitNum)); //single bit is 0

    array[byteNum] = array[byteNum] & mask;
}

void sieve(unsigned int n_max)
{
    int needed_bytes = ceil((float)n_max/(float)8); //how many bytes required
    unsigned char array[needed_bytes];
    unsigned int p = 2, j=0;
    //unsigned int j = 0;
    int i=0;

    for(i=0; i<needed_bytes; i++)
        array[i]=0;
  
    unset(array, 1);
    for(i=2; i<=n_max; i++) //initially all numbers from 2 to n are considered primes
        set(array, i);

// according to given algorithm.
    while( p*p <= n_max)
    {
        j = p*p;
        while(j <= n_max)
        {
            unset(array, j);
            j = j + p;
        }
        do
        {
            p++;

        }
        while(check_val(array, p)!=1); //skips composite numbers. 
    }

    // printing in the required format. 
    n_max = htonl((unsigned int)n_max);
    fwrite(&n_max, sizeof(uint32_t), 1, stdout);
    
    for(i=2; i<=n_max; i++)
        if(check_val(array, i))
        {
            i = htonl(i);
            fwrite(&i, sizeof(uint32_t), 1, stdout);
        }




}
