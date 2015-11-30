GCC = g++
CPPFLAGS = -g -finline-functions -Wall -Winline -pipe -ffast-math
TARGET = WordBind
OBJ = main.o Dict.o Hash.o FindLetterAndNumber.o Bind.o

all: $(TARGET)
$(TARGET) : $(OBJ)	
	$(GCC) -g -o $@ $^

%.o	: %.cpp	
	$(GCC) -O0 $(CPPFLAGS) -c $< -o $@

clean:	
	rm -rf *.o


