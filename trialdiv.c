#include "hw6.h"

/*
	Samyukta Satish Rao
	University of Southern California
	5924873657
*/


int trialdiv(unsigned char *  n, char * filename, int print_flag, int space_init) // performs trial division. 
{


	FILE *  fp;
	int count = 0; // counter for the primes array 
	int n_read = -1;
	unsigned char bval[4] = {}; // array to read big endian representation of a number. 
	unsigned int temp;
	int i; // used for the for loop. 
	
	if(filename == NULL)
	{
		fp = stdin;
	}
	else
	{
		fp = fopen(filename, "rb");
		if(fp==NULL)
		{
			fprintf(stderr, "Error opening file. \n");
			exit(0);

		}
	}

	// counting number of entries in the file. 
	fseek(fp, 0L, SEEK_END);
	int file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	// Since  its in Big Endian notation, each number is written as 4 bytes. So size should be multiple of 4. 
	// Check for primefile validity. 


	if(file_size%4!=0)
	{
		fprintf(stderr, "Invalid primes file. \n");
		
	}

	//reading the primes 
	// Actual number of primes in the file - 
	int primes_size = file_size/4;

	unsigned int * primes = calloc(primes_size, sizeof(unsigned int)); // array to hold the primes
	
	// read all 4 bytes and use << and OR operation to get the decimal value of the input. 
	while(n_read!=0)
	{
		n_read = fread(bval, sizeof(int), 1, fp);
		if(n_read ==0)
			break;

	
		for(i=0;i<4;i++)
		{
			temp = (temp<<8) | bval[i];

		}
		primes[count++] = temp;
	}

	




	BIGNUM* givenno = BN_new();
	BN_dec2bn(&givenno, (char*)n); //convert given number to BIGNUM


	BIGNUM* divisor = BN_new();
	BIGNUM* rem_num = BN_new();
	BN_CTX* bn_ctx = BN_CTX_new();

	BIGNUM* div2 = BN_new();

	int result_isprime = 1;

	for(i=1; i<primes_size; i++)
	{
		divisor = BN_new();
		rem_num = BN_new();
		bn_ctx = BN_CTX_new();

		BN_bin2bn((unsigned char *)(&primes[i]), sizeof(uint32_t), divisor); //Read one value from primes file. Conver to BIGNUM representation.

		//Need to check till sqrt of given number. 
		BN_sqr(div2, divisor, bn_ctx);
		if(BN_cmp( div2, givenno) == 1) //exit if the condition fails. 
			break;

		BN_mod(rem_num, givenno, divisor, bn_ctx); //mod operation. 

		if(BN_is_zero(rem_num) == 1) //if a rem_num is not generated => number is divisible by that prime. 
		{
			result_isprime = 0; //trial division failed

			if(print_flag)
			{
				print_space(space_init);
				printf("n is composite by trial division (mod %s = 0)\n", BN_bn2dec(divisor));
			}

			break;
		}
	}



	BIGNUM* maxval = BN_new();
	BIGNUM* maxval2 = BN_new();
	BN_bin2bn((unsigned char *)(&primes[0]), sizeof(uint32_t), maxval); // first value in the primes file is the max value of the primes. 
	// convert to BIGNUM
	BN_sqr(maxval2, maxval, bn_ctx);

	if(result_isprime== 1) //previous block did not fail
	{
		if(BN_cmp( maxval2, givenno) == -1) //If the maxval in primesfile is strictly less than floor(sqrt(number)), i.e maxval^2 < number
		{
			if(print_flag)
			{
				print_space(space_init);
				printf("n passes trial division test (not enough primes)\n");
			}
		}
		else
		{
			if(print_flag)
			{
				print_space(space_init);
				printf("n passes trial division test\n");
			}
		}
	}

	BN_free(givenno);
	BN_free(divisor);
	BN_free(rem_num);
	BN_free(div2);
	BN_free(maxval);
	BN_free(maxval2);
	BN_CTX_free(bn_ctx);

	return result_isprime;

}
