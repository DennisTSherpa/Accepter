APP = lexer
LEX = basic3.lex
SRC = lex.yy.cc
FLAGS = -Wall -Werror -std=c++17

${APP} : ${LEX}
	flex ${LEX}
	g++ ${FLAGS} ${SRC} -o $@

clean : 
	rm -f ${APP} ${SRC}


