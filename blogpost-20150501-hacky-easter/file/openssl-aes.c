// AES encryption/decryption demo (OpenSSL EVP API)
// gcc -Wall openssl-aes.c -lcrypto -o openssl-aes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
//#include <openssl/evp.h>

static const unsigned char key[] = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

int main()
{
	unsigned char text[]="hello world!";

	// unsigned char enc_out[10];
	// unsigned char dec_out[10];
	// AES_KEY wctx;
	// AES_set_encrypt_key(key, 128, &wctx);
	// AES_encrypt(text, enc_out, &wctx);
	// printf("encrypt: %s\n", enc_out);
	// AES_decrypt(enc_out, dec_out, &wctx);
	// printf("decrypt: %s \n", dec_out);

	unsigned char * enc_out = malloc(128*sizeof(char));
	unsigned char * dec_out = malloc(128*sizeof(char));
	AES_KEY enc_key, dec_key;
	AES_set_encrypt_key(key, 128, &enc_key);
	AES_encrypt(text, enc_out, &enc_key);
	AES_set_decrypt_key(key,128,&dec_key);
	AES_decrypt(enc_out, dec_out, &dec_key);

	int i;
	printf("orig:\t");
	for(i=0;*(text+i)!=0x00;i++)
	{
		printf("%X ",*(text+i));
	}
	printf("\nencrypt:\t");
	for(i=0;*(enc_out+i)!=0x00;i++)
	{
		printf("%X ",*(enc_out+i));
	}
	printf("\ndecrypt:\t");
	for(i=0;*(dec_out+i)!=0x00;i++)
	{
		printf("%X ",*(dec_out+i));
	}
	printf("\n");

	// free(enc_out);
	// free(dec_out);

	return 0;
}
