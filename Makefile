all:	check

default: check
	
clean:
	rm -rf ckptagent

agent:
	gcc ckptagent.c -o ckptagent

build: agent

check:	clean build