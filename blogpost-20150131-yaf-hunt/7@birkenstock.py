#!/usr/bin/env python
#-*- coding: utf-8 -*-

import re
import sys
import string

#@ https://sites.google.com/site/yafhunt/birkenstock
txt ="""
Ehzduh wkh lghv ri Pdufk
Vsqerw, gsyrxvcqir, erh pszivw
Ny xjjrx yt rj rtxy xywfslj ymfy rjs xmtzqi kjfw;
Wkh olyh-orqj gdb.
M eq rsx keqiwsqi: M hs pego wsqi tevx
Xzhm rjs rd kwnjsix ymfs jsjrnjx
Kh lv d guhdphu; ohwxv ohdyh klp: sdvv.
Vsqerw, gsyrxvcqir, pirh qi csyv ievw
Frgnynts'x ijgy nx ufni.
Pajmksktz! znua gxz lrkj zu hxazoyn hkgyzy
"""

words = set(map(str.strip, open("dict-en", "r").readlines()))

t = len(string.lowercase)
offset = 3
lines = txt.split("\n")[1:-1]
for i, ln in enumerate(lines):
#	for o in xrange(1,t):
	line = ""
	for c in ln:
		if c.isalpha():
			idx = ord(c.lower())-ord('a')
			if i == len(lines)-1: o = 6
			else: o = offset%3+3
			idx = (idx-o+t)%t
			cc = string.lowercase[idx]
			if c.isupper():
				line += cc.upper()
			else:
				line += cc
		else: line += c
	# ok = False
	# for tok in re.sub(r"[^a-z]", "", line.lower()).split(" "):
	# 	if tok in words:
	# 	#	print "token:", tok
	# 		ok = True
	# 		break
	# if ok:
	print "%d: %s"%(o, line)
	offset += 1
	print
