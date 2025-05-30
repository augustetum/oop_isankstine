ifeq ($(OS),Windows_NT)
    TARGET = programa.exe
    RM = del
else
    TARGET = programa
    RM = rm -f
endif

$(TARGET): programa.cpp programosFunkcijos.cpp
	g++ -std=c++11 -o $(TARGET) programa.cpp programosFunkcijos.cpp

clean:
	$(RM) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run