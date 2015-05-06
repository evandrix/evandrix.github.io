#!/bin/bash

#for f in c19-keys; do ./script.sh $f 2>/dev/null &; disown; done
#watch -n1 'ps -eo pid,pcpu,pmem,stat,cmd --sort pid | egrep "script\.sh"'
#ps aux | egrep "script\.sh" | cut -d' ' -f2 | xargs sudo kill -9

f=c19
while read k; do
	>&2 echo "=== $k ==="
	OUT=$(base64 -d "${f}" | openssl enc -d -aes-256-cbc -pass "pass:$k" | base64 -d)
	MAGIC=$(echo "${OUT}" | head -c4 | tail -c +2)
	if [[ "${MAGIC}" == "PNG" ]]; then
		base64 -d "${f}" | openssl enc -d -aes-256-cbc -pass "pass:$k" | base64 -d >"${k}.png"
		exit
	fi
done <${1:-c19-keys}
