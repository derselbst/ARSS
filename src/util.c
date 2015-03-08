#include "util.h"

void win_return()
{
	#ifdef WIN32
	if (quiet==0)
	{
		printf("Press Return to quit\n");
		getchar();
	}
	#endif
}

#ifdef WIN32
#include "Windows.h"

int32_t gettime()	// in milliseconds
{
	return (int32_t) GetTickCount();
}
#else
#include <sys/time.h>

int32_t gettime()	// in milliseconds
{
	struct timeval t;

	gettimeofday(&t, NULL);

	return (int32_t) t.tv_sec*1000 + t.tv_usec/1000;
}
#endif

inline double roundoff(double x)	// nearbyint() replacement, with the exception that the result contains a non-zero fractional part
{
	if (x>0)
		return x + 0.5;
	else
		return x - 0.5;
}

inline int32_t roundup(double x)
{
	if (fmod(x, 1.0) == 0)
		return (int32_t) x;
	else
		return (int32_t) x + 1;
}

float getfloat()
{
	float x;
	char a[32];
	fgets(a, 31, stdin);
	if (a[0]==0)
		return 0.0;
	else
	{
		x=atof(a);
		return x;
	}
}

inline int32_t smallprimes(int32_t x)	// returns 1 if x is only made of these small primes
{
	int32_t i, p[2]={2, 3};

	for (i=0; i<2; i++)
		while (x%p[i] == 0)
			x/=p[i];

	return x;
}

inline int32_t nextsprime(int32_t x)	// returns the next integer only made of small primes
{
	while (smallprimes(x)!=1)
		x++;

	return x;
}

inline double log_b(double x)
{
	if (LOGBASE==1.0)
		return x;
	else
		#ifdef DEBUG
		if (x==0)
			fprintf(stderr, "Warning : log(0) returning -infinite\n");
		else
		#endif
		return log(x)/log(LOGBASE);
}

inline uint32_t rand_u32()
{
	#if RAND_MAX == 2147483647
		return rand();
	#elif RAND_MAX == 32767
		return ((rand()%256)<<24) | ((rand()%256)<<16) | ((rand()%256)<<8) | (rand()%256);
	#else
		fprintf(stderr, "Unhandled RAND_MAX value : %d\nPlease signal this error to the developer.", RAND_MAX);
		return rand();
	#endif
}

inline double dblrand()	// range is +/- 1.0
{
	return ((double) rand_u32() * (1.0 / 2147483648.0)) - 1.0;
}

inline uint16_t fread_le_short(FILE *file)		// read from file a 16-bit integer in little endian
{
	uint8_t	byte_a, byte_b;

	fread(&byte_a, 1, 1, file);
	fread(&byte_b, 1, 1, file);

	return (uint16_t) (byte_b<<8) | byte_a;
}

inline uint32_t fread_le_word(FILE *file)		// read from file a 32-bit integer in little endian
{
	uint8_t	byte_a, byte_b, byte_c, byte_d;

	fread(&byte_a, 1, 1, file);
	fread(&byte_b, 1, 1, file);
	fread(&byte_c, 1, 1, file);
	fread(&byte_d, 1, 1, file);

	return (uint32_t) (byte_d<<24) | (byte_c<<16) | (byte_b<<8) | byte_a;
}

inline void fwrite_le_short(uint16_t s, FILE *file)	// write to file a 16-bit integer in little endian
{
	uint8_t byte;

	byte = s & 0xFF;
	fwrite(&byte, 1, 1, file);
	byte = (s>>8) & 0xFF;
	fwrite(&byte, 1, 1, file);
}

inline void fwrite_le_word(uint32_t w, FILE *file)	// write to file a 32-bit integer in little endian
{
	uint8_t byte;

	byte = w & 0xFF;
	fwrite(&byte, 1, 1, file);
	byte = (w>>8) & 0xFF;
	fwrite(&byte, 1, 1, file);
	byte = (w>>16) & 0xFF;
	fwrite(&byte, 1, 1, file);
	byte = (w>>24) & 0xFF;
	fwrite(&byte, 1, 1, file);
}

char *getstring()
{
	signed long len_str, i;
	char a[FILENAME_MAX], *b;

	fgets(a, sizeof(a), stdin);
	len_str=strlen(a);

	b=malloc(len_str * sizeof(char));

	for (i=0; i<len_str; i++)
		b[i]=a[i];

	b[len_str-1]=0;

	return b;
}

int32_t str_isnumber(char *string)	// returns 1 is string is a valid float number, 0 otherwise
{
	int32_t i;
	int32_t size = strlen(string);
	char c;

	if (size==0)
		return 0;

	c = string[0];
	if (! ((c>='0' && c<='9') || c=='+' || c=='-'))
		return 0;

	for (i=1; i<size; i++)
	{
		c = string[i];

		if (! ((c>='0' && c<='9') || c=='.' || c=='e'))
			return 0;
	}

	return 1;
}
