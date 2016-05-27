
#include "hw6.h"
/*

Samyukta Satish Rao
University Of Southern California.

*/



/*

Reads input and parses commandline arguements. 
hw6 primes -n=maxval

*/


int main(int argc, char * argv[] )
{
	
   	char * input_str;
    char * filename;
    char * filename1;
    char * maxitr;
    //FILE * fp;
	if(argc==1)
    {
        fprintf(stderr, "Error: Information given is not enough.\n");
        exit(0);
    }
    else
    {
    	if(strcmp(argv[1], "primes")==0)
        {	
            unsigned int np;
        	if(argc!=3)
        	{
        		fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 primes -n=maxval\n");
                exit(0);
        	}
        	else
        	{
				//int n;
                //printf("%c\n", argv[2][2] );
                if(strlen(argv[2])<4)
                {
                    fprintf(stderr, "Malformed command\n");
                    exit(0);
                }
                
              
        		if(argv[2][1]=='n')
                {
                   
                        //printf("Hello\n");
                        input_str = strchr(argv[2], '=');                  
                        input_str++;
                        if(strcmp("foobar", input_str)==0)
                        {
                            fprintf(stderr, "Input number invalid.\n");
                            exit(0);
                        }
                      
                		np = strtod(input_str, NULL);
                        //check for errors
                        if(np < 2 || np > 16777216) //maxval must be between 2 and 2^24, inclusive
                        {
                            fprintf(stderr, "maxval must be between 2 and 2^24, inclusive.\n");
                            exit(0);
                            //return EXIT_FAILURE;
                        }
                    

					 sieve(np);
                }
                else
                {
                    fprintf(stderr, "Error1: Malformed command. Please enter in the following format \n hw6 primes -n=maxval\n");
                    exit(0);
                }
                
				
             	
			}
        	
        }

        if(strcmp(argv[1], "trialdiv")==0)
        {   

            if(argc<3)
            {
                fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 trialdiv -n=number -p=primesfile\n");
                exit(0);
            }
            else if(argc == 4)
            {
                int n;
                if(argv[2][1]=='n')
                {
                    input_str = strchr(argv[2], '=');                  
                    input_str++;
                    n = atoi(input_str);
                      if(strcmp("foobar", input_str)==0)
                        {
                            fprintf(stderr, "Input number invalid.\n");
                            exit(0);
                        }
                   // printf("n: %d\n", n);
                }
                if(argv[3][1]=='p')
                {
                    filename = strchr(argv[3], '=');
                    filename++;
                    /*fp = fopen(filename, "rb");
                    if(fp==NULL)
                    {
                        fprintf(stderr, "Error opening file. \n");
                        exit(0);
                    } */
                    //printf("Filename: %s\n", filename );
                }
                trialdiv((unsigned char * )input_str, filename, 1, 0);
                
                
                
            }

         
         
            
        }

        if(strcmp(argv[1], "millerrabin")==0)
        {   
            char * number;
            //char * maxiter;
            char * pfilename;
            if(argc<4)
            {
                fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 millerrabin -n=number -t=maxitr -p=primesfile\n");
                exit(0);
            }
            else if(argc == 5)
            {
                //int n, m;
                int m;
                if(argv[2][1]=='n')
                {
                    number = strchr(argv[2], '=');                  
                    number++;
                    if(strcmp("foobar", number)==0)
                        {
                            fprintf(stderr, "Input number invalid.\n");
                            exit(0);
                        }
                    //n = atoi(input_str);
                   // printf("n: %d\n", n);
                }
                else
                {
                    fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 millerrabin -n=number -t=maxitr -p=primesfile\n");
                    exit(0);
                }
                if(argv[3][1] == 't')
                {
                    maxitr = strchr(argv[3], '=');
                    maxitr++;
                    if(strcmp("foobar", maxitr)==0)
                        {
                            fprintf(stderr, "Input maxitr invalid.\n");
                            exit(0);
                        }
                    m = atoi(maxitr);
                }
                else
                {
                    fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 millerrabin -n=number -t=maxitr -p=primesfile\n");
                    exit(0);
                }
                if(argv[4][1]=='p')
                {
                    pfilename = strchr(argv[4], '=');
                    pfilename++;

                }
                else
                {
                    fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 millerrabin -n=number -t=maxitr -p=primesfile\n");
                    exit(0);
                }
                //char * millerrabin(unsigned char * number , int maxiter, char * pfilename)
                millerrabin((unsigned char *)number, m, pfilename, 1, 0, 1);
                
                
                
            }

           else if(argc == 4)
            {

                fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 millerrabin -n=number -t=maxitr -p=primesfile\n");
                exit(0);
               /*int n;
                if(argv[2][1]=='n')
                {
                    input_str = strchr(argv[2], '=');                  
                    input_str++;
                    n = atoi(input_str);
                    //printf("n: %d\n", n);
                }
                if(argv[3][1] == 't')
                {
                    maxitr = strchr(argv[3], '=');
                    maxitr++;
                    int m = atoi(maxitr);
                }
                //millerrabin(n, m, NULL); */
                
                
            }
            
        }

        if(strcmp(argv[1], "rndsearch")==0)
        {   

            int m;
            if(argc<4)
            {
                fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
                exit(0);
            }
            else if(argc == 6)
            {
                int n;
                if(argv[2][1]=='k')
                {
                    input_str = strchr(argv[2], '=');                  
                    input_str++;
                    n = atoi(input_str);
                    if(n <= 0)
                    {
                        fprintf(stderr, "Number of bits cannot be zero or less than zero.\n");
                        exit(0);
                    }
                   // printf("n: %d\n", n);
                }
                if(argv[3][1] == 't')
                {
                    maxitr = strchr(argv[3], '=');
                    maxitr++;
                    m = atoi(maxitr);
                    if(m<= 0)
                    {
                        fprintf(stderr, "Number of iterations should be greater than 0\n");
                        exit(0);
                    }
                }
                else
                {
                    
                fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
                exit(0);
            
                }
                if(argv[4][1]=='p')
                {
                    filename = strchr(argv[4], '=');
                    filename++;

                }
                else
                {
                    fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
                    exit(0);
                }
                if(argv[5][1]=='r')
                {
                    filename1 = strchr(argv[5], '=');
                    filename1++;

                }
                else
                {
                    fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
                    exit(0);
                }
                //printf("Called.\n");
              //  printf("%d %d %s %s\n", n, m, filename, filename1);
                rndsearch(n, m, filename, filename1);
                
                
                
            }

            else if(argc!=6)
            {
                fprintf(stderr, "Error: Malformed command.Please enter all values in the following format \n hw6 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
                exit(0);
                
            }
            
        }

        if(strcmp(argv[1], "maurer")==0)
        {   



            BIGNUM *bn_n = NULL;
            if(argc<=4)
            {
                fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 maurer -k=numbits -p=primesfile -r=rndfile\n");
                exit(0);
            }
            else if(argc == 5)
            {
                int n;
                if(argv[2][1]=='k')
                {
                    input_str = strchr(argv[2], '=');                  
                    input_str++;
                    n = atoi(input_str);
                   if(n <= 0)
                    {
                        fprintf(stderr, "-k cannot be 0 or less than 0\n");
                        exit(0);
                    }
                }
                
                if(argv[3][1]=='p')
                {
                    filename = strchr(argv[3], '=');
                    filename++;

                }
                else
                {
                    fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 maurer -k=numbits -p=primesfile -r=rndfile\n");
                    exit(0);
                }
                if(argv[4][1]=='r')
                {
                    filename1 = strchr(argv[4], '=');
                    filename1++;
                }
                else
                {
                    fprintf(stderr, "Error: Malformed command. Please enter in the following format \n hw6 maurer -k=numbits -p=primesfile -r=rndfile\n");
                    exit(0);
                }
                bn_n = BN_new();
                maurer( n, filename, filename1, 0);
                
                
                
            }
            //printf("argc: %d\n", argc );
            if(argc!=5)
            {
                fprintf(stderr, "Error: Malformed command. Please enter all values in the following format \n hw6 maurer -k=numbits -p=primesfile -r=rndfile\n");
                exit(0);
            }

           /* else if(argc == 4)
            {
                int n;
                if(argv[2][1]=='n')
                {
                    input_str = strchr(argv[2], '=');                  
                    input_str++;
                    n = atoi(input_str);
                    //printf("n: %d\n", n);
                }
                if(argv[3][1] == 't')
                {
                    maxitr = strchr(argv[3], '=');
                    maxitr++;
                    int m = atoi(maxitr);
                }
                //millerrabin(n, m, NULL);
                
                
            } */
            
        }
    }
	return 0;
}
