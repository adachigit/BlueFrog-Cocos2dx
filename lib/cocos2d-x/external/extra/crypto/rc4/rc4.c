#include "rc4.h"

void RC4encode(char* output, const unsigned int output_len, const char* key, const unsigned int key_len)
{
	RC4Context cxt;

	unsigned int i;
	unsigned int j;

	char swap_byte;

	// Setup key schedule
	for (i = 0; i<256; i++)
	{
		cxt.S[i] = (char)i;
	}

	j = 0;

	for (i = 0; i<256; i++)
	{
		j = (j + cxt.S[i] + key[i % key_len]) % 256;
		//swap cxt.S[i] and cxt.S[j]
		swap_byte = cxt.S[i];
		cxt.S[i] = cxt.S[j];
		cxt.S[j] = swap_byte;
	}

	i = 0;
	j = 0;

	cxt.i = i;
	cxt.j = j;

	for (i = 0; i < output_len; ++i)
	{
		cxt.i = (cxt.i + 1) % 256;
		cxt.j = (cxt.j + cxt.S[cxt.i]) % 256;
		// swap cxt.S[cxt.i] and cxt.S[cxt.j]
		swap_byte = cxt.S[cxt.i];
		cxt.S[cxt.i] = cxt.S[cxt.j];
		cxt.S[cxt.j] = swap_byte;

		output[i] ^= cxt.S[(cxt.S[cxt.i] + cxt.S[cxt.j]) % 256];
	}
}
