#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sys/stat.h>
#include <assert.h>

#define MAX_RUN_BYTES 9
#define NUM_RUNS (1<<20)
#define MASK (0xffff)
#define HASH_LEN 4

typedef struct {
	uint8_t len;
	char bytes[MAX_RUN_BYTES];
} run_t;

run_t runs[NUM_RUNS] = { { 0, {0} } };

#pragma pack(push, 1)
typedef struct _backref {
	uint8_t lit  : 1;
	uint8_t len  : 7;
	uint16_t hash;
} backref_t;

typedef struct _literal {
	uint8_t lit  : 1;
	uint8_t len  : 7;
} literal_t;
#pragma pack(pop)

unsigned int first_difference(char *one, char *two) 
{
	unsigned int i = 0;

	while (one[i] == two[i] && i < MAX_RUN_BYTES)
		i++;

	return i;

}

uint16_t do_hash(const char *str)
{
	unsigned int hash = 0x55aa55aa;
	unsigned int i    = 0;

	for(i = 0; i < HASH_LEN; str++, i++)
	{
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}

	return hash & MASK;

}

void clear_runs()
{
	for (unsigned int i = 0; i < NUM_RUNS; i++)
	{
		memset(&runs[i], 0, sizeof(run_t));
	}
}

char *compress(const char *input, unsigned int *out_size)
{

	clear_runs();

	struct stat st;
	if (stat(input, &st) < 0)
		err(2, "Stat failed");

	char *in = (char *)malloc(st.st_size);
	FILE *fp = fopen(input, "rb");
	if (!fp)
		err(3, "Couldn't open file");

	fread(in, 1, st.st_size, fp);
	fclose(fp);

	// Output in will be no bigger than file
	char *out = (char *)malloc(st.st_size);
	memset(out, 0, st.st_size);

	printf("%p\n", out);

	unsigned int ii = 0;
	unsigned int oi = 0;

	do {

		// hash 4 bytes of input
		uint16_t hash = do_hash(&in[ii]);
		unsigned int max_len = first_difference(&in[ii], runs[hash].bytes);

		// see if its in the hash table
		if (runs[hash].len != 0 && max_len >= HASH_LEN) {
			// if so, find the longest run
			// emit backref with length and hash
			assert(max_len >= HASH_LEN);

			backref_t backref;
			memset(&backref, 0, sizeof(backref));

			backref.lit = 0;
			backref.len = max_len;
			backref.hash = hash;

			memcpy(&out[oi], &backref, sizeof(backref));
			oi += sizeof(backref);

			// replace hash table bytes
			memcpy(runs[hash].bytes, &in[ii], max_len);
			runs[hash].len = max_len;

			ii += max_len;

		} else {
			// if not in hash, emit a literal and add to hash table

			literal_t literal;
			memset(&literal, 0, sizeof(literal));

			literal.lit = 1;
			literal.len = MAX_RUN_BYTES;

			memcpy(&out[oi], &literal, sizeof(literal));
			oi += sizeof(literal);

			memcpy(&out[oi], &in[ii], literal.len);
			oi += literal.len;

			runs[hash].len = literal.len;
			memcpy(runs[hash].bytes, &in[ii], literal.len);

			ii += literal.len;
		}	

	} while (ii < st.st_size);

	*out_size = oi;
	free(in);

	return out;
}

char *decompress(const char *in, const unsigned int input_size, unsigned int *decompressed_size)
{
	clear_runs();

	unsigned int ii = 0;
	unsigned int oi = 0;
	unsigned int out_size = 0;

	literal_t maybe_lit;
	backref_t backref;

	// Calculate output size
	do {
		memcpy(&maybe_lit, &in[ii], sizeof(maybe_lit));

		if (maybe_lit.lit) {
			ii += sizeof(maybe_lit) + maybe_lit.len;
			out_size += maybe_lit.len;
		} else {
			ii += sizeof(backref);
			out_size += maybe_lit.len;
		}

	} while (ii < input_size);

	*decompressed_size = out_size;

	char *out = (char *)malloc(out_size);
	memset(out, 0, out_size);

	ii = 0;
	oi = 0;

	do {

		memcpy(&maybe_lit, &in[ii], sizeof(maybe_lit));

		if (maybe_lit.lit) {

			ii += sizeof(maybe_lit);
			uint16_t hash = do_hash(&in[ii]);
			runs[hash].len = maybe_lit.len;
			memcpy(&runs[hash].bytes, &in[ii], maybe_lit.len);

			memcpy(&out[oi], &in[ii], maybe_lit.len);
			ii += maybe_lit.len;
			oi += maybe_lit.len;
		} else {
				
			memcpy(&backref, &in[ii], sizeof(backref));
			ii += sizeof(backref);
			memcpy(&out[oi], &runs[backref.hash].bytes, backref.len);

			oi += maybe_lit.len;
		}

	} while (ii < input_size);

	return out;
}
