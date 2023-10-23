PROJECT = sandbox

CC = clang
SRC_DIR =./src
SRC = $(wildcard ${SRC_DIR}/*.c)
OUT_DIR = bin
OBJ = ${SRC:.c=.o}
LIBS = -lm -lraylib
CFLAGS = -std=c99 -pipe -Wno-unused -Wall -Wno-deprecated-declarations
BINARY = ${OUT_DIR}/${PROJECT}

main: ${OBJ}
	${CC} -o ${BINARY} ${OUT_DIR}/*.o ${LIBS}

.c.o:
	${CC} -c ${CFLAGS} $<
	@mv ./*.o ${OUT_DIR}/

run: main
	@${BINARY}

clean:
	@rm -v ${OUT_DIR}/*.o ${BINARY}