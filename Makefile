OBJDIR = obj
SRCDIR = src
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
LIBS = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
DEPS = ##$(wildcard $(SRCDIR)/*.h)


bomberman: $(OBJ) $(DEPS)
	gcc $^ -o $@ $(LIBS)

windows: $(OBJ) $(DEPS)
	gcc $^ -o bomberman.exe -lmingw32 $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	gcc -c $< -o $@

$(OBJDIR):
	mkdir $@


.PHONY: clean
clean:
	-rm -r $(OBJDIR)
	-rm bomberman*
