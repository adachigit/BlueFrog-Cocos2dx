#ifndef _RC4_H_
#define _RC4_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		unsigned int     i;
		unsigned int     j;
		unsigned char    S[256];
	} RC4Context;

	void RC4encode(char* output,
					const unsigned int output_len,
					const char* key,
					const unsigned int key_len
					);
#ifdef __cplusplus
}
#endif

#endif