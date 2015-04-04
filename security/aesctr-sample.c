/*

OpenSSL decrypt sample code

can rough use AES-CTR encrypt/decrypt

compile with gcc with -lcrypto

Liuqing Yang<yllqq#outlook.com>

*/

#include <openssl/aes.h>
#include <openssl/rand.h> 
#include <openssl/hmac.h>
#include <openssl/buffer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Code example uses partail code from: http://stackoverflow.com/questions/3141860/aes-ctr-256-encryption-mode-of-operation-on-openssl
// Mostly in the ctr_ state, and init_ctr functions. 

struct ctr_state 
{ 
	unsigned char ivec[AES_BLOCK_SIZE];	 
	unsigned int num; 
	unsigned char ecount[AES_BLOCK_SIZE]; 
}; 

FILE *readFile;
FILE *writeFile;
AES_KEY key; 

int bytes_read, bytes_written;	 
unsigned char indata[AES_BLOCK_SIZE]; 
unsigned char outdata[AES_BLOCK_SIZE];
unsigned char KEY[AES_BLOCK_SIZE];
unsigned char iv[AES_BLOCK_SIZE];
struct ctr_state state;	 

int init_ctr(struct ctr_state *state, const unsigned char iv[16])
{		 
	/* aes_ctr128_encrypt requires 'num' and 'ecount' set to zero on the
     * first call. */
    state->num = 0;
    memset(state->ecount, 0, AES_BLOCK_SIZE);

    /* Initialise counter in 'ivec' to 0 */
    memset(state->ivec + 8, 0, 8);

    /* Copy IV into 'ivec' */
    memcpy(state->ivec, iv, 8);

	return 0;
}

void fencrypt(char* read, char* write, const unsigned char* enc_key)
{ 
    if(!memset(iv, 0, AES_BLOCK_SIZE))
    {
        fprintf(stderr, "Could not create zero iv.");
        exit(1);    
    }
        
	readFile = fopen(read,"rb"); // The b is required in windows.
	writeFile = fopen(write,"wb");
	
	if(readFile==NULL) 	
	{
		fprintf(stderr, "Read file is null."); 
		exit(1);
	}
	
	if(writeFile==NULL)
	{
		fprintf(stderr, "Write file is null."); 
		exit(1);
	}
	
	fwrite(iv, 1, 8, writeFile); // IV bytes 1 - 8
    fwrite("\0\0\0\0\0\0\0\0", 1, 8, writeFile); // Fill the last 4 with null bytes 9 - 16
    
	//Initializing the encryption KEY
	if (AES_set_encrypt_key(enc_key, 128, &key) < 0)
    {
        fprintf(stderr, "Could not set encryption key.");
        exit(1); 
    }

	init_ctr(&state, iv); //Counter call
	//Encrypting Blocks of 16 bytes and writing the output.txt with ciphertext	
	while(1) 	
	{
		bytes_read = fread(indata, 1, AES_BLOCK_SIZE, readFile); 
		AES_ctr128_encrypt(indata, outdata, bytes_read, &key, state.ivec, state.ecount, &state.num);
        
		bytes_written = fwrite(outdata, 1, bytes_read, writeFile); 
		if (bytes_read < AES_BLOCK_SIZE)
		{
			break;
		}
	}
	
	fclose(writeFile);
	fclose(readFile);
}
	
void fdecrypt(const char* read, const char* write)
{	
	int ret;
    
	readFile=fopen(read,"rb"); // The b is required in windows.
	writeFile=fopen(write,"wb");
	
	if(readFile==NULL)
	{
		fprintf(stderr,"Read file is null."); 
        exit(1);
	}
	
	if(writeFile==NULL)	
	{
		fprintf(stderr, "Write file is null."); 
        exit(1);
	}
	
	ret = fread(KEY, 1, AES_BLOCK_SIZE, readFile); 
	if (ret != AES_BLOCK_SIZE)
	{
		fprintf(stderr, "Read KEY error.");
	}

    ret = fread(iv, 1, AES_BLOCK_SIZE, readFile); 
	if (ret != AES_BLOCK_SIZE)
	{
		fprintf(stderr, "Read IV error.");
	}

	//Initializing the encryption KEY
    if (AES_set_encrypt_key(KEY, 128, &key) < 0)
    {
        fprintf(stderr, "Could not set decryption key.");
        exit(1);
    }

	init_ctr(&state, iv);//Counter call
	//Encrypting Blocks of 16 bytes and writing the output.txt with ciphertext		 
	while(1) 	
	{
		bytes_read = fread(indata, 1, AES_BLOCK_SIZE, readFile);	
        //printf("%i\n", state.num);
		AES_ctr128_encrypt(indata, outdata, bytes_read, &key, state.ivec, state.ecount, &state.num);
        
		bytes_written = fwrite(outdata, 1, bytes_read, writeFile); 
		if (bytes_read < AES_BLOCK_SIZE) 
		{
			break;
		}
	}
	fclose(writeFile); 
	fclose(readFile); 
}

int main(int argc, char *argv[])
{
	//char key[]	= {0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04};
    //fencrypt("encme.txt", "enced.enc", (const unsigned char*)key);
	fdecrypt("enced.enc", "unenced.txt");
    //getc(stdin);
	return 0;
}
