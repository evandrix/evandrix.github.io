#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _hdr {
    uint8_t magic[8];
    uint32_t version;
    uint32_t num_files;
} hdr;

typedef struct _entry {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint8_t filename[32];
} entry;

int main()
{
	int entryNum, uncompressedLen, compressedOffset, compressedIndex, uncompressedIndex, replaceIndex, numReplaced, replaceLen;
	char* compressedBuf;
	char* uncompressedBuf;
	hdr head;
	entry entry;
	FILE* fUncompressed;
	FILE* f = fopen("weissman.csawlz", "rb");
	fread(&head, sizeof(hdr), 1, f);
	for(entryNum=0; entryNum<head.num_files; entryNum++)
	{
		fread(&entry, sizeof(entry), 1, f);
		printf("%s\n", entry.filename);
		printf("0x%x %u %u\n",
				entry.magic, entry.compressed_size, entry.uncompressed_size);
		compressedBuf = calloc(entry.compressed_size, sizeof (char));
		fread(compressedBuf, sizeof (char), entry.compressed_size, f);

		switch(entryNum)
		{
			case 1:
				free(compressedBuf);
				continue;
			case 0:
				fUncompressed = fopen("index.html", "rb");
				break;
			case 2:
				fUncompressed = fopen("alice.txt", "rb");
		}
		fseek(fUncompressed, 0, SEEK_END);
		uncompressedLen = ftell(fUncompressed)*8;
		fseek(fUncompressed, 0, SEEK_SET);
		uncompressedBuf = calloc(uncompressedLen, sizeof (char));
		fread(uncompressedBuf, sizeof (char), uncompressedLen, fUncompressed);

		compressedIndex = 0;
		uncompressedIndex = 0;
		compressedOffset = 0;
		while(compressedIndex < entry.compressed_size)
		{
			if((compressedIndex-compressedOffset)%10 == 0)
			{
				// Weird compression thing here
				uncompressedIndex--;
				if(compressedBuf[compressedIndex] != 0x13)
				{
					printf("//{");
					replaceLen = 0;
					do
					{
						replaceLen += compressedBuf[compressedIndex] >> 1;
						printf("<len:0x%02x>", compressedBuf[compressedIndex] >> 1);
						printf("<0x%08x>", 
								((compressedBuf[compressedIndex+1]&0xff)<<8) +
								(compressedBuf[compressedIndex+2]&0xff));
						compressedIndex += 3;
						compressedOffset += 3;
					}
					while(compressedBuf[compressedIndex] != 0x13);

					printf("}={");

					for(replaceIndex=0; replaceIndex<replaceLen; replaceIndex++)
					{
							uncompressedIndex++;
							printf("%c", uncompressedBuf[uncompressedIndex]);
					}
					printf("}");
					compressedIndex--;
				}
				printf("|");
			}
			else
			{
				// I commented this out for input into Python script to do
				// some frequency analysis. It didn't help.
				if(uncompressedIndex < entry.uncompressed_size)
					printf("%c", compressedBuf[compressedIndex]);
			}
			compressedIndex++;
			uncompressedIndex++;
		}
		// Guess whether or not these were in the code I wrote during the ctf
		fclose(fUncompressed);
		free(compressedBuf);
		free(uncompressedBuf);
	}
	fclose(f);
	return 0;
}
