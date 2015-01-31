#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys

import string
import itertools
# product(lst, repeat=X): cartesian product ~=nested for-loop
# permutations(lst, r): r-len tuples, all possible orderings, no repeats
# combinations(lst, r): r-len tuples, in sorted order, no repeats
# combinations_with_replacement(lst, r): r-len tuples, in sorted order, w/ repeats
#for w in itertools.product(string.lowercase, repeat=4):
#	w = ''.join(w)
#	print w

import re
import requests
from collections import defaultdict
words = defaultdict(set)
lines = map(str.strip, open("dict-en", "r").readlines())
for word in lines:
	word = re.sub(r"[^a-z]", "", word.lower())
	words[len(word)].add(word)
#print "\n".join(words[6])
for word in words[6]:
	url = "https://sites.google.com/site/yafhunt/%s"%word
	r = requests.get(url)
	sys.stdout.write("%s\t%d\t%s\n"%(word,r.status_code,url))
	sys.stdout.flush()
