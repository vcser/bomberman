OBJDIR = obj
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c))
LIBS = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

bomberman: $(OBJ)
	gcc $^ -o $@ $(LIBS)

windows: $(OBJ)
	gcc $^ -o bomberman.exe -lmingw32 $(LIBS)

$(OBJDIR)/%.o: %.c utils.h gamelogic.h graphics.h | $(OBJDIR)
	gcc -c $< -o $@

$(OBJDIR):
	mkdir $@

clean:
	rm -r $(OBJDIR)
	rm bomberman*
