#!/usr/bin/env python
#-*- coding: utf-8 -*-

import re
import sys
import collections
import string
import requests
from pwn import *

#socat EXEC:"python -u psifer-school.py" TCP4:localhost:12345

#the answer to this stage is hopeyouautomate
def part1_remote(ctxt):
	sess = requests.Session()
	resp = sess.post("http://www.xarg.org/tools/caesar-cipher/",
		data={"text": ctxt, "key": 0})
	return resp.text
def part1(ctxt):
	FREQ_EN = [0.0749, 0.0129, 0.0354, 0.0362, 0.1400, 0.0218, 0.0174, 0.0422,
		0.0665, 0.0027, 0.0047, 0.0357, 0.0339, 0.0674, 0.0737, 0.0243, 0.0026,
		0.0614, 0.0695, 0.0985, 0.0300, 0.0116, 0.0169, 0.0028, 0.0164, 0.0004]
	c = collections.Counter()
	c.update(ctxt)
	#c.elements(), c.most_common(N); +: combine cnt; -: diff; &: intersect (+ve min); |: union (max)
	diffs = []
	for offset in xrange(len(string.ascii_lowercase)):
		diff = 0.0
		for letter in string.ascii_lowercase:
			if letter in c:
				freq = c[letter]/float(len(ctxt.replace(" ", "")))
				idx = (ord(letter) - ord('a') + offset)%len(FREQ_EN)
				diff += abs(FREQ_EN[idx] - freq)
		diffs.append( (diff, offset) )
	_, key = min(diffs)
	tr = string.maketrans(string.ascii_lowercase,
		string.ascii_lowercase[key:] + string.ascii_lowercase[:key])
	return ctxt.translate(tr)

if __name__ == "__main__":
	context(os="linux", arch="i386") #, log_level="DEBUG")

	p = remote("localhost", 12345)
	p.recvuntil("psifer text: ")
	ctxt = p.recvline()
	log.info(ctxt)
	ans = part1(ctxt)
	log.info(ans)
	m = re.match(r"^the answer to this stage is (\w+)$", ans)
	if not m: p.close(); sys.exit(1)
	ans = m.group(1)
	log.info("=> "+ans)
	p.sendline(ans)

	p.recvuntil("psifer text: ")
	ctxt = p.recvline()
	log.info(ctxt)

	p.close()
