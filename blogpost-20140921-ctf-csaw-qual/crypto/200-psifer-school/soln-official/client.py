#!/usr/bin/env python
#socat EXEC:"python -u ./solveschool.py" TCP4:localhost:12345
from __future__ import print_function
import sys, string, math

def read_until(match):
    while True:
        scan = raw_input()
        if scan.find(match) != -1:
            return scan

ciphertext = read_until("psifer text: ").replace("psifer text: ","")
print(ciphertext,file=sys.stderr)
slide = ord(ciphertext[0]) - ord('t') 
plaintext = ''
for c in ciphertext:
    if c.isalpha():
        plaintext += string.lowercase[ (string.lowercase.find(c) - slide ) % 26]
    else:
        plaintext += c
answer = plaintext.split(" ")[-1]
print("Answer is : %s" % answer,file=sys.stderr)
print(answer)

ciphertext = read_until("psifer text: ").replace("psifer text: ","")
print(ciphertext,file=sys.stderr)

def transpose(key, message):
    numOfColumns = int(math.ceil(1.0*len(message) / key))
    numOfRows = int(key)
    numOfShadedBoxes = (numOfColumns * numOfRows) - len(message)
    plaintext = ['']*numOfColumns
    col = 0
    row = 0
    for c in message:
        plaintext[col] += c
        col += 1
        if (col == numOfColumns) or (col == numOfColumns - 1 and row >= numOfRows - numOfShadedBoxes):
            col = 0
            row += 1

    return ''.join(plaintext)

for key in (range(2,len(ciphertext))):
    plaintext = transpose(key, ciphertext)
    if plaintext.find("should be fairly straight forward if you have") != -1:
        break

print(plaintext,file=sys.stderr)

answer = plaintext.split('"')[1]
print("Answer is : %s" % answer,file=sys.stderr)
print(answer)

ciphertext = read_until("psifer text: ").replace("psifer text: ","")
print(ciphertext,file=sys.stderr)

from pygenere import *
plaintext = VigCrack(ciphertext).crack_message()
plaintext = plaintext.replace(" ","")
print(plaintext,file=sys.stderr)

answerStart = plaintext.find("RIGHTHERE") + len("RIGHTHERE")
answerEnd = plaintext.find("OKNOWMORE") 

answer = plaintext[answerStart:answerEnd]
print(answer,file=sys.stderr)
print(answer)

while True:
    try:
        print(raw_input(),file=sys.stderr)
    except:
        False
