CC=gcc

LIBS=-lm -lreadline
DEPS=shell.h

_OBJ=cat.o  cd.o chmod.o  clear.o  cp.o  echo.o  help.o  home.o ln.o  ls.o  mkdir.o  mv.o  path.o  pwd.o  rm.o  rmdir.o  touch.o
OBJ=obj/main.o $(patsubst %, obj/commands/%, $(_OBJ))

out/shell.out: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

obj/main.o: main.c $(DEPS)
	$(CC) -c -o $@ $< 

obj/commands/%.o: commands/%.c  $(DEPS)
	$(CC) -c -o $@ $<	

.PHONY: clean

clean:
	rm -f obj/*.o obj/commands/*.o out/*