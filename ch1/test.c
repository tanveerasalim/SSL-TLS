#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAXSIZE 128
int main(int argc, char ** argv)
{
#if 0
	static char dst[MAXSIZE];	
	
	if ( argc != 2 )
	{
		
		fprintf(stderr,"%d: Error: Less than three arguments!\n"
				
				"Format: [./test.o] [src_string]"
		       );

		return 1;

	}
	
	errno_t result = strncat_s(dst,MAXSIZE,argv[1],MAXSIZE-strnlen_s(dst,MAXSIZE)-1);
	
	printf("Errno returned: %d\n",result);

	printf("Dest String: %s\n",dst);
#endif
	char s1[100] = "good\0";

	char s2[6] = "hello\0";
	
	char s3[6] = "hello\0";

	char s4[7] = "abc\0";

	char s5[1000] = "bye\0";

	char s6[1000] = "\0";
	
	int r1 = 0, r2 = 0, r3 = 0, r4 = 0,
	    
	    r5 = 0;

	r1 = strncat_s(s1,100,s5,1000);
	
	r2 = strncat_s(s2,6,"\0",1);

	r3 = strncat_s(s3,6,"X\0",2);
	
	r4 = strncat_s(s4,7,"defghijklmn\0",3);

	printf("Results for strings s1-s4:\n");

	printf("Length of s3: %llu\n",strnlen_s( s3,MAXSIZE ) );

	printf("r1: %d s1: %s\nr2: %d s2: %s\nr3: %d s3: %s\nr4: %d s4: %s\n",
			
		r1,s1,r2,s2,r3,s3,r4,s4);

	printf("Does strncat_s warn when M_IS_ZERO?\n");

	r5 = strncat_s(s6,1000,s1,100);

	printf("r5 %d s6: %s\n",r5,s6);

	printf("Does strncat_s give an error when s1max == 0?\n");

	r5 = strncat_s(s6,0,s1,100);

	printf("r5 %d s6: %s\n",r5,s6);

	printf("Does strncat_s give an error when s1 == NULL?\n");

	r5 = strncat_s(NULL,1000,s1,100);

	printf("r5 %d s6: %s\n",r5,s6);

	printf("Does strncat_s give a warning when s2 == NULL?\n");

	r5 = strncat_s(s1,1000,NULL,100);

	printf("Does strncat_s give an error when s1max > RSIZE_MAX?\n");
	
	r5 = strncat_s(s1,RSIZE_MAX+1,s6,100);

	printf("r5 %d s6: %s\n",r5,s6);


	return 0;
}
