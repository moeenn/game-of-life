PROJECT = automata

CC = clang++
SRC_DIR =./src
SRC = $(wildcard ${SRC_DIR}/*.cpp)
OUT_DIR = bin
OBJ = ${SRC:.cpp=.o}
LIBS = -lraylib
CFLAGS = -Wextra -Werror -Wall -std=c++20 -Wc++20-extensions -O3
BINARY = ${OUT_DIR}/${PROJECT}

main: ${OBJ}
	${CC} -o ${BINARY} ${OUT_DIR}/*.o ${LIBS}

.cpp.o:
	${CC} -c ${CFLAGS} $<
	@mv ./*.o ${OUT_DIR}/

run: main
	@${BINARY}

build:
	@bear -- make

clean:
	@rm -v ${OUT_DIR}/*.o ${BINARY}
	@rm -v ./compile_commands.json
