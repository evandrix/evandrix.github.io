#!/bin/bash

8_britishindia () {
	for s in {1700..1800}; do
		url="https://sites.google.com/site/yafhunt/${s}"
		STATUS=$(curl -s -o /dev/null -w "%{http_code}" $url)
	#	if [[ "$STATUS" != "404" ]]; then
			echo -e "${s}\t${STATUS}\t${url}"
	#	fi
	done
}
8_britishindia
