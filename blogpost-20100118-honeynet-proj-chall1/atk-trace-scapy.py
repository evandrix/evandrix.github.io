#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
from scapy.utils import rdpcap

# @ http://stackoverflow.com/questions/8726881/sending-packets-from-pcap-with-changed-src-dst-in-scapy#answer-8843436
def replay(file):
	packets = rdpcap(file)

	# try:
	# 	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# 	s.connect(("127.0.0.1", 445))
	# except:
	# 	print>>sys.stderr, "err: connect"
	# 	return

	for p in packets:
		try:
			if p.haslayer(TCP) and p.getlayer(TCP).dport == 445 and len(p.getlayer(TCP).payload)>6:
				try:
					if p.getlayer(TCP).flags > 1 and "\x90" in p.getlayer(TCP).load:
					#	s.sendall(str(p.getlayer(TCP).payload))
					#	print p.getlayer(TCP).payload
						print repr(p.getlayer(TCP).load)
						break
				except:
					print>>sys.stderr, "err: send"
					return
		except:
			print>>sys.stderr, "err: read"
			return
		# time.sleep(1)
	# s.shutdown(0)
	return

if __name__ == "__main__":
	replay("atk-trace.pcap")
