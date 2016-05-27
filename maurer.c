/*
	Samyukta Satish Rao
	University of Southern California
	5924873657
	Reference - https://github.com/ashtonmendes/Generating-Primes/maurer.c
*/


	#include "hw6.h"
	#define round(X) (((X) >= 0) ? (int)((X)+0.5) : (int)((X)-0.5))


	unsigned char rndByte(FILE *rndfile)
	{
		unsigned char buffer[1];

		fread(buffer, sizeof(buffer), 1, rndfile); //read 1 byte

		return buffer[0];
	}
	unsigned char *  maurer(int k, char * pfilename, char * rfilename, int level)
	{
		FILE * p_fp = fopen(pfilename, "rb");// prime file
		FILE * r_fp = fopen(rfilename, "rb"); // random file 
		//printf("primes: %s\n random: %s\n", pfilename, rfilename );

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
		unsigned int temp1;
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
		
		// read all 4 bytes and use << and OR operation to get the decimal value of the input. 
		while(n_read!=0)
		{
			n_read = fread(bval, sizeof(int), 1, p_fp);
			if(n_read ==0)
				break;

		
			for(i=0;i<4;i++)
			{
				temp1 = (temp1<<8) | bval[i];

			}
			primes[count++] = temp1;
		}

		//========================================================================

			printf("Maurer: level %d, k=%d\n", level, k);

			unsigned char* n;
			unsigned char* qS;
			unsigned char* rS;
			unsigned char* aS;
			unsigned char rndByteReturn = 0;
			//int size;
			int num_bits_in_q = 0, num_bits_in_n = 0, iterNum = 0;
			//unsigned int* numbersFromPrimesFile = readPrimesFile(pfilename, &size);
			double c = 0, B = 0, r = 0;
			int m = 0;
			BIGNUM* q = BN_new();

			BIGNUM* one = BN_new();
			BIGNUM* two = BN_new();
			BIGNUM* km2 = BN_new();
			BIGNUM* nm1 = BN_new();

			BIGNUM* temp = BN_new();

			BIGNUM* I = BN_new();
			BN_CTX* bn_ctx = BN_CTX_new();
			BIGNUM* R = BN_new();
			
			BIGNUM* bn_n = BN_new();
			BIGNUM* a = BN_new();			
			BIGNUM* b = BN_new();			
			BIGNUM* d = BN_new();

			BN_one(one);
			uint32_t tempInt = 2;
			BN_bin2bn((unsigned char *)(&tempInt), sizeof(uint32_t), two);
			tempInt = k-2;
			BN_bin2bn((unsigned char *)(&tempInt), sizeof(uint32_t), km2);

			if(k <= 20)
			{
				while(1)
				{
					n = RndOddNum(k, r_fp);

					printf("  step 1.1, n = %s\n", n);

					if(trialdiv(n, pfilename, 1, 4) == 1) //trial division passed
					{
						BN_dec2bn(&bn_n, (char*)n);
						if(level==0)printf("\nMaurer's Algorithm found an %d-bit prime:\n  n = %s\n", BN_num_bits(bn_n), n);
						return n;
					}
				}
			}

			c = 0.1; m = 20;

			B = c * k * k;

			//generate a fraction r, the size of q relative to n
			printf("  step 4");
			if(k <= 2*m)
				r = 0.5;
			if(k > 2*m)
			{
				while(1)
				{
					rndByteReturn = rndByte(r_fp);
					r = rndByteReturn / 255.0;
					r = 0.5 + r / 2.0;
					if( (k*(1-r)) > m)
					{
						printf(": random byte = %d", rndByteReturn);
						break;

					}
				}
			}
			printf(", r = %d%%\n", round(100.0*r));

			//RECURSION
			qS = maurer(floor(r*k)+1, pfilename, rfilename, level+1);
			BN_dec2bn(&q, (char*)qS); //convert q to BIGNUM representation
			printf("Maurer: back to level %d, k=%d, q=%s\n", level, k, qS);

			num_bits_in_q = BN_num_bits(q);

			//I = floor(2^(k-2) / q)
			BN_exp(I, two, km2, bn_ctx);
			BN_div(I, NULL, I, q, bn_ctx);

			while(1)
			{
				iterNum++;

				//R = RndOddNum(k+1-num_bits_in_q)
				//printf("Before RndOdd: %d\n", k+1-num_bits_in_q );
				rS = RndOddNum(k+1-num_bits_in_q, r_fp);
				//printf("rS: %s\n", rS );
				BN_dec2bn(&R, (char*)rS);

				//R = (R mod I) + I + 1
				BN_mod(R, R, I, bn_ctx);
				BN_add(R, R, I);
				BN_add(R, R, one);

				//n = 2Rq+1
				BN_mul(bn_n, two, R, bn_ctx);
				BN_mul(bn_n, bn_n, q, bn_ctx);
				BN_add(bn_n, bn_n, one);
				n = (unsigned char*)BN_bn2dec(bn_n);

				printf("  step 7, itr %d: R = %s, n = %s\n", iterNum, BN_bn2dec(R), n);

				if(trialdiv(n, pfilename, 1, 4) == 1) //trial division passes
				{
					num_bits_in_n = BN_num_bits(bn_n);

					do
					{
						aS = RndOddNum(num_bits_in_n, r_fp);
						BN_dec2bn(&a, (char*)aS);

						BN_sub(nm1, bn_n, one); //n-1
					}
					while(BN_cmp(a, one)<=0 || BN_cmp(a, nm1)>=0 ); //while (a <= 1 or a >= n-1))

					printf("  step 7.2.1, itr %d: a = %s\n", iterNum, aS);

					//b = a^{n-1} mod n
					BN_mod_exp(b, a, nm1, bn_n, bn_ctx);

					if(BN_is_one(b))
					{
						//b = a^{2R} mod n
						BN_mul(temp, two, R, bn_ctx);
						BN_mod_exp(b, a, temp, bn_n, bn_ctx);

						//d = gcd(b-1,n)
						BN_sub(temp, b, one); //b-1
						BN_gcd(d, temp, bn_n, bn_ctx);

						if(BN_is_one(d))
						{
							if(level==0)printf("\nMaurer's Algorithm found an %d-bit prime:\n  n = %s\n", BN_num_bits(bn_n), n);
							return n;
						}
					}
				}
			}


			BN_free(q);
			BN_free(one);
			BN_free(two);
			BN_free(km2);
			BN_free(nm1);
			BN_free(I);
			BN_CTX_free(bn_ctx);
			BN_free(R);
			BN_free(temp);
			BN_free(bn_n);
			BN_free(a);			
			BN_free(b);			
			BN_free(d);

	}
