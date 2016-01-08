###First glance
(I didn't look at the challenge until both updates were posted.)  
The structs mention compression, and file is named `weissman.csawlz`, so this challenge must relate to [LZ compression](https://en.wikipedia.org/wiki/LZ77_and_LZ78). It turns out LZ is the father of many major compression algorithms used today, with many varieties; I wasn't going to find one standard implementation that I could use.  
A cursory inspection of a file reveals `0x13` bytes every 10 bytes, with occasional other bytes being garbled. The `0x13` bytes are markers for blocks of data, so after 9 bytes, if the next byte isn't another `0x13`, then all of the bytes up until the marker are some compression reference.

###Write some code
I then whipped up the bare bones of [read.c](https://gist.github.com/Lense/7ed1d8db57950b2de8fc#file-read-c). I found the [html file](http://burtleburtle.net/bob/hash/) and the right version of [Alice in Wonderland](https://raw.githubusercontent.com/mihi-tr/reading-alice/master/pg28885.txt) (after fixing newlines and adding the easy flag as mentioned in the update) online, which let me look at what was replaced with what:  
```
|<html>
<h|ead>
<tit|le>Hash F|unctions |and Block| Ciphers<|/title>
<|meta name|="keyword|s" conten|t="hash, |hash func|tion, has|h table l|ookup,
ch|ecksum, c|ipher, bl|ock ciphe|r, stream| cipher, |rng, prng|, random |number">
|</head>
<|body bgco|lor="#fff|fff" text|="#000000|" link="#|0000ff" 
|vlink="#0|0ff00" al|ink="#ff0|000">

<c|enter><h2|>Hash Fun|ctions an|d Block C{<0x0a><0x1e><0x20>}={ipher}||s</h2></c{<0x0c><0xc5><0x6c>}={enter>}||

<p>I ha|ve a lot |of materi|al relate|d to hash|ing.
<UL>|
<LI>Defi|nitions a|nd my off|erings:
 | <UL>
  <|LI><a hre|f="#looku|p">Hash f{<0x12><0xd6><0x73>}={unctions }||for hash |table loo|kup</a>
 | <LI><a h|ref="#cor|rection">|Error Cor{<0x0e><0x02><0x6e>}={rection}|| Codes</a|>
  <LI><|a href="#|checksum"|>Noncrypt|ographic |Checksums|</a>
  <L|I><a href|="nandhas|h.html">A| noncrypt{<0x12><0xdb><0x75>}={ographic }||hardware {<0x08><0x92><0x61><0x12><0x5e><0x0a><0x12><0x5d><0x62>}={hash</a>
  <LI><a href}||="#one-wa|y">One-wa|y functio|ns (crypt{<0x12><0xdb><0x20><0x08><0x92><0x75>}={ographic hash}||
function|s, digita|l signatu|res)</a>
|  <LI><a |href="#bl|ock">Bloc|k Ciphers{<0x12><0x5e><0x68><0x12><0x5d><0x75>}={</a>
  <LI><a href}||="#random|">Random |Number Ge|nerators<|/a>
  <LI|><a href=|"#stream"|>Stream C{<0x0a><0x1e><0x13>}={ipher}||s</a>
```
I replaced `0x13` markers with ascii `|`s, and put the compressed data and uncompressed data in `{``}` pairs.

###Now the hard part
Interesting features:
- Everything is replaced by text occurring at the start of a block.  
- `ipher` occurs twice, and is replaced by the same 6 bytes each time. This means that it the compression must not use relative references.  
- `ipher`, `enter>`, and `unctions ` have wildly different bytes, so the compression probably doesn't use absolute references.
- That leaves hashes. (Somewhere around this point I started working with Sliden)
- Expanding the bytes into binary reveals that first byte or so has mostly `0`s, while the rest appears evenly distributed.
- The 8th bit is always 0
- **The first 7 bits are the length of the replaced section.** 
At this point I wasted a significant amount of time reading the website that the html was taken from trying to find the hash that was used.

###Actually solving the challenge
It turns out that there are only 23 distinct hashes used in the jpeg, but the jpeg is readable after only fixing the first two.
```
00000000   13 FF D8 FF  EE 00 0E 41  64 6F 13 62  65 00 64 C0  .......Ado.be.d.
00000010   00 00 00 01  13 FF DB 00  84 00 14 10  10 19 13 12  ................
00000020   19 27 17 17  27 32 26 1F  13 26 32 2E  26 26 26 26  .'..'2&..&2.&&&&
00000030   2E 3E 13 35  35 35 35 35  3E 44 41 41  13 41 41 41  .>.55555>DAA.AAA
00000040   41 44 44 44  44 44 13 44  44 44 44 44  44 44 44 44  ADDDDD.DDDDDDDDD
00000050   7B 3C 6C 65  6E 3A 30 78  30 39 3E 3C  30 78 30 30  {<len:0x09><0x00
00000060   34 66 39 61  3E 3C 6C 65  6E 3A 30 78  30 36 3E 3C  4f9a><len:0x06><
00000070   30 78 30 30  34 66 39 61  3E 7D 13 01  15 19 19 20  0x004f9a>}.....
00000080   1C 20 26 18  13 18 26 36  26 20 26 36  44 36 13 2B  . &...&6& &6D6.+
00000090   2B 36 44 44  44 42 35 42  7B 3C 6C 65  6E 3A 30 78  +6DDDB5B{<len:0x
000000A0   30 39 3E 3C  30 78 30 30  34 66 39 61  3E 3C 6C 65  09><0x004f9a><le
000000B0   6E 3A 30 78  30 39 3E 3C  30 78 30 30  34 66 39 61  n:0x09><0x004f9a
000000C0   3E 3C 6C 65  6E 3A 30 78  30 39 3E 3C  30 78 30 30  ><len:0x09><0x00
000000D0   34 66 39 61  3E 3C 6C 65  6E 3A 30 78  30 39 3E 3C  4f9a><len:0x09><
000000E0   30 78 30 30  34 66 39 61  3E 7D 13 44  44 FF C0 00  0x004f9a>}.DD...
000000F0   11 08 02 C8  13 05 00 03  01 22 00 02  11 01 13 03  ........."......
00000100   11 01 FF C4  00 A1 00 00  13 02 03 01  01 01 00 00  ................
00000110   00 00 13 00  00 00 00 00  00 00 01 02  13 03 05 04  ................
00000120   06 07 01 01  01 01 13 01  01 00 00 00  00 00 00 00  ................
00000130   7B 3C 6C 65  6E 3A 30 78  30 35 3E 3C  30 78 30 30  {<len:0x05><0x00
00000140   34 63 65 33  3E 7D 13 01  02 03 04 10  01 00 02 01  4ce3>}..........
```
`0x4f9a` is a block of `0x44` `D`s, and `0x4ce3` is `0x000000000000000102`, found from trying the possible blocks. [Here](https://gist.github.com/Lense/7ed1d8db57950b2de8fc#file-key-jpg) is the image if you replace every single other compressed block with `0x41` `A`s. [write.c](https://gist.github.com/Lense/7ed1d8db57950b2de8fc#file-write-c) is what I wrote to write the file.  
  
###Challenge source
[Part of the source](https://gist.github.com/withzombies/909b403852ea1e31f553)  
Hash algorithm:
```
unsigned int hash = 0x55aa55aa;
unsigned int i = 0;
 
for(i = 0; i < HASH_LEN; str++, i++)
{
hash ^= ((hash << 5) + (*str) + (hash >> 2));
}
 
return hash & MASK;
```
Nope, wouldn't have guessed that.
