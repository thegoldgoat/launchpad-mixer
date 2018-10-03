# Cose del compilatore
CXX = g++
CFLAGS = -std=c++11 -g
# Oggetti
OBJ = obj/launchpad-mixer.o obj/mixer.o
# Cartelle sorgenti e oggetti
OBJDIR = obj
SRCDIR = src
# Librerie da linkare
LIBS = -lrtmidi -lasound
# File eseguibile
OUT = launchpad-mixer

# Compile and run
all: $(OUT) run

# Compile
$(OUT): $(OBJDIR) $(OBJ)
	$(CXX) $(OBJ) -o $(OUT) $(CFLAGS) $(LIBS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: run
run:
	./$(OUT)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $< -c -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm $(OBJDIR) -rf
	rm $(OUT) -f

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
