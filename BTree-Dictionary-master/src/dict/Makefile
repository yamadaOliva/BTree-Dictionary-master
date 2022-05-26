# Simple makefile for BT Library sample programs


TARGETS=${patsubst %.c,%,${wildcard ./*.c}}

default:	${TARGETS}

%:	%.c
	${CC} -o $@ $< -L../btree-5.0.1/lib -lbt -I../btree-5.0.1/inc

clean:
	rm -f ${TARGETS}

