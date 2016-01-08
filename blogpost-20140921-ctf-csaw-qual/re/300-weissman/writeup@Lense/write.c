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
	int entryNum, compressedOffset, compressedIndex, replaceIndex, numReplaced;
	int* replaceSizes = calloc(100, sizeof (int));
	int* replaceHashes = calloc(100, sizeof (int));
	char* compressedBuf;
	char* replaceBuf = calloc(9, sizeof (char));
	hdr head;
	entry entry;
	FILE* f = fopen("weissman.csawlz", "rb");
	fread(&head, sizeof(hdr), 1, f);
	for(entryNum=0; entryNum<head.num_files; entryNum++)
	{
		fread(&entry, sizeof(entry), 1, f);
		compressedBuf = calloc(entry.compressed_size, sizeof (char));
		fread(compressedBuf, sizeof (char), entry.compressed_size, f);
		switch(entryNum)
		{
			case 1:
				break;
			case 0:
			case 2:
				free(compressedBuf);
				continue;
		}

		compressedIndex = 0;
		compressedOffset = 0;
		while(compressedIndex < entry.compressed_size)
		{
			if((compressedIndex-compressedOffset)%10 == 0)
			{
				// Weird compression thing here
				if(compressedBuf[compressedIndex] != 0x13)
				{
					memset(replaceSizes, 0, 100*sizeof (int));
					memset(replaceHashes, 0, 100*sizeof (int));
					numReplaced = 0;
					do
					{
						replaceSizes[numReplaced] = compressedBuf[compressedIndex] >> 1;
						replaceHashes[numReplaced] = 
								((compressedBuf[compressedIndex+1]&0xff)<<8) +
								(compressedBuf[compressedIndex+2]&0xff);
						compressedIndex += 3;
						compressedOffset += 3;
						numReplaced++;
					}
					while(compressedBuf[compressedIndex] != 0x13);

					for(replaceIndex=0; replaceIndex<numReplaced; replaceIndex++)
					{
						switch(replaceHashes[replaceIndex])
						{
							case 0x4f9a:
								strncpy(replaceBuf, "DDDDDDDDD", 9);
								break;
							case 0x4ce3:
								strncpy(replaceBuf,
										"\x00\x00\x00\x00\x00\x00\x00\x01\x02", 9);
								break;
							default:
								strncpy(replaceBuf, "AAAAAAAAA", 9);
						}
						while(replaceSizes[replaceIndex]--)
						{
							printf("%c", replaceBuf[replaceIndex%9]);
						}
					}
					compressedIndex--;
				}
			}
			else
			{
				printf("%c", compressedBuf[compressedIndex]);
			}
			compressedIndex++;
		}
		// Guess whether or not these were in the code I wrote during the ctf
		free(compressedBuf);
	}
	free(replaceSizes);
	free(replaceHashes);
	fclose(f);
	free(replaceBuf);
	return 0;
}
