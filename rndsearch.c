#include "hw6.h"
/*
Samyukta Satish Rao
5924872657
*/


/*
RndOddNum referred from https://github.com/ashtonmendes/Generating-Primes/rndsearch.c
*/

unsigned char* RndOddNum(int k, FILE *rndfile)
{
	int x = ceil((float)k/(float)8); //how many bytes to read.
	unsigned char buffer[x];

	BIGNUM* randomNum = BN_new();
	BN_zero(randomNum);

	int totalBits = 0;
	int currentBit = -1;

	int bytesRead = fread(buffer, sizeof(buffer), 1, rndfile); //read next x bytes from file
	if(bytesRead != 0)
	{
		//convert byte array to BIGNUM representation
		BN_bin2bn(buffer, x, randomNum);

		//set bit 0 and bit k-1 to 1
		BN_set_bit(randomNum, 0);
		BN_set_bit(randomNum, k-1);

		//sets all bits from k and above, to 0 in the BIGNUM
		totalBits = 8 * BN_num_bytes(randomNum);
		for(currentBit=k; currentBit<totalBits; currentBit++)
			BN_clear_bit(randomNum, currentBit);
	}

	unsigned char* randomNumString = (unsigned char*)BN_bn2dec(randomNum);

	BN_free(randomNum);

	return randomNumString;
}


unsigned char *  rndsearch(int num, int maxiter, char *pfilename, char * rfilename)
{
	/*
		num = number of bits;
		maxiter = maximum number of iterations. 
	*/
	
	FILE * p_fp = fopen(pfilename, "rb");// prime file
	FILE * r_fp = fopen(rfilename, "rb"); // random file 


	if(p_fp==NULL)
	{
		fprintf(stderr, "Could not open primesfile. Invalid file. \n");
		exit(0);
	}
	if(r_fp==NULL)
	{
		fprintf(stderr, "Could not open random file. Invalid file.  \n");
		exit(0);
	}




	// reading into primes array. 
	int count = 0; // counter for the primes array 
	int n_read = -1;
	unsigned char bval[4] = {}; // array to read big endian representation of a number. 
	unsigned int temp;
	int i; // used for the for loop. 
	
	// counting number of entries in the file. 
	fseek(p_fp, 0L, SEEK_END);
	int file_size = ftell(p_fp);
	fseek(p_fp, 0L, SEEK_SET);

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
	unsigned char * rand_oddnum;
	// read all 4 bytes and use << and OR operation to get the decimal value of the input. 
	while(n_read!=0)
	{
		n_read = fread(bval, sizeof(int), 1, p_fp);
		if(n_read ==0)
			break;

	
		for(i=0;i<4;i++)
		{
			temp = (temp<<8) | bval[i];

		}
		primes[count++] = temp;
	}
//================================================================================================================
		int count_rand = 1;

		while(1)
		{
			printf("RANDOM-SEARCH: iteration %d\n", count_rand++);

			rand_oddnum = (unsigned char*)RndOddNum(num, r_fp);

			printf("  n = %s\n", rand_oddnum);

			if(trialdiv(rand_oddnum, pfilename, 1, 2) == 0) //trial division failed
				continue;

			if(strcmp(millerrabin(rand_oddnum, maxiter, pfilename, 1, 2, 0), "prime")==0)
				return rand_oddnum;
		}
}



