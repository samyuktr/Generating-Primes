#include "hw6.h"

/*
	Samyukta Satish Rao
	University of Southern California
	592487367

	
*/

void print_space(int n)
{
	int i=0;
	for(i=0; i<n; i++)
		printf(" ");
}




char * millerrabin(unsigned char * number , int maxiter, char * pfilename, int print_flag, int space_init, int printN)
{
	FILE *  fp;
	int count = 0; // counter for the primes array 
	int n_read = -1;
	unsigned char bval[4] = {}; // array to read big endian representation of a number. 
	unsigned int temp;
	int i; // used for the for loop. 
	int j=1;
	
	if(pfilename == NULL)
	{
		fp = stdin;
	}
	else
	{
		fp = fopen(pfilename, "rb");
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
		fprintf(stderr, "Invalid primes file.\n");
		
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
		primes[count] = temp;
		count = count+1;
	}


	/*
		for(i=0;i<primes_size;i++)
		{
			printf("%d ", primes[i]);
		} 
	*/


	//int print_flag = 1;
	//int space_init = 0;
	//int printN = 1;

	// we need - s, r, n-1, n, a;
	// from bignum we need 0, 1
	// need to convert 2 to big number - as 2 is an even prime. 
//=========================BIGNUM representations =============================================
	// n
	BIGNUM* n = BN_new();
	BN_dec2bn(&n, (char*)number); //convert number(from main file) to BIGNUM representation

	// 1 
	BIGNUM* one = BN_new(); 
	BN_one(one);

	// 2 
	BIGNUM* two = BN_new();
	uint32_t int2 = 2;
	BN_bin2bn((unsigned char *)(&int2), sizeof(uint32_t), two);

	// n-1
	BIGNUM* nm1 = BN_new();
	BN_sub(nm1, n, one);

	// initially r = n-1 i.e., before we represent it as 2^s * (r)
	BIGNUM* r = BN_new();
	BN_copy(r, nm1);

	int s = 0;

	BIGNUM* remainder = BN_new();	
	BN_CTX* bn_ctx = BN_CTX_new();

//==============================Computation according to given algorithm==============================================


	BN_mod(remainder, r, two, bn_ctx); //remainder = r%2

	//split n into 2 parts - > 2^s and r such that r is odd.

	/*
	rem = r%2;
	while(rem==0 && r!=0)
	{
		decrement r 
		s++;
		calculate r%2

	}
	*/
	while(BN_is_zero(remainder) && (BN_is_zero(r) != 1)) //while r%2==0 and r!=0
	{
		BN_rshift1(r, r);
		s = s+1;
		BN_mod(remainder, r, two, bn_ctx);
	}

	//printf("s=%d r=%s\n",s,  BN_bn2dec(r));

	

	BIGNUM* a = BN_new();
	BIGNUM* y = BN_new();

	//print n, n-1, s and r
	if(print_flag)
	{
		if(printN)
		{
			print_space(space_init);
			printf("n = %s\n", BN_bn2dec(n));
		}
		print_space(space_init);
		printf("  n-1 = %s\n", BN_bn2dec(nm1));
		print_space(space_init);
		printf("  s = %d\n", s);
		print_space(space_init);
		printf("  r = %s\n", BN_bn2dec(r));
	}

	for(i=1; i<=maxiter; i++)
	{
		// a = primes[i];
		BN_bin2bn((unsigned char *)(&primes[i]), sizeof(uint32_t), a);

		// n-1
		nm1 = BN_new();
		BN_sub(nm1, n, one);

		//if (a > n-1)
		if(BN_cmp(a, nm1) == 1)
		{
			if(print_flag)
				fprintf(stderr, "Error! a > n-1");
			return "failure";
		}

		//compute y = a^r mod n
		y = BN_new();
		BN_mod_exp(y, a, r, n, bn_ctx);

		if(print_flag)
		{
			print_space(space_init);
			printf("  Itr %d of %d, a = %s, y = %s", i, maxiter, BN_bn2dec(a), BN_bn2dec(y));
			if(BN_cmp(y, nm1)==0)
				printf(" (which is n-1)");
			printf("\n");
		}

		//if (y != 1 and y != n-1)
		if(BN_is_one(y)!=1 && BN_cmp(y, nm1)!=0)
		{
			for(j=1; j<=(s-1) && BN_cmp(y, nm1)!=0; j++)
			{
				BN_mod_sqr(y, y, n, bn_ctx); //y = y^2 mod n

				if(print_flag)
				{
					print_space(space_init);
					printf("    j = %d of %d, y = %s", j, (s-1), BN_bn2dec(y));
					if(BN_cmp(y, nm1)==0)
						printf(" (which is n-1)");
					printf("\n");
				}

				//if (y == 1)
				if(BN_is_one(y) == 1)
				{
					if(print_flag)
					{
						print_space(space_init);
						printf("Miller-Rabin found a strong witness %s\n", BN_bn2dec(a));
					}
					return "composite";
				}
			}

			//if (y != n-1)
			if(BN_cmp(y, nm1)!=0)
			{
				if(print_flag)
				{
					print_space(space_init);
					printf("Miller-Rabin found a strong witness %s\n", BN_bn2dec(a));
				}
				return "composite";
			}
		}
	}

	if(print_flag)
	{
		print_space(space_init);
		printf("Miller-Rabin declares n to be a prime number\n");
	}

	BN_free(n);
	BN_free(one);
	BN_free(two);
	BN_free(nm1);
	BN_free(r);
	BN_free(remainder);
	BN_free(a);
	BN_free(y);
	BN_CTX_free(bn_ctx);
	

	return "prime";



}
