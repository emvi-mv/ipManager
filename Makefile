NAME = ipManager
FILES = db cli main

CPPFLAGS = -Ofast -Wall -std=c++17 -fPIC

OBJECTS = ${addsuffix .o,${FILES}}

all: ${FILES}
	g++ ${CPPFLAGS} ${OBJECTS} -o ${NAME}

clean:
	rm ${NAME} ${OBJECTS}

${FILES}:
	g++ ${CPPFLAGS} -c $@.cpp -o $@.o
