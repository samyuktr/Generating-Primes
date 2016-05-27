#ifndef _FUNCTION_H_  //handling multiple inclusions
#define _FUNCTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>
 #include <openssl/bn.h>

void sieve(unsigned int max); // generating primes using sieve of erathosthenes
void print_space(int n); // needed for trialdiv function. 
unsigned char *  rndsearch(int num, int maxiter, char *pfilename, char * rfilename); // performs random search. 
int trialdiv(unsigned char* n, char * fp, int x, int y);
char * millerrabin(unsigned char * number , int maxiter, char * pfilename, int x, int y, int z); // performs trial division. 
unsigned char* RndOddNum(int bits, FILE *rndfile);
unsigned char * maurer(int k, char * pfilename, char * rfilename, int l); // performs maurer's algorithm. 

#endif
