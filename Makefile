CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SRC      = prop.cpp test.cpp
TARGET   = prop.exe
DLL      = prop.dll

.PHONY: build build.dll run clean

build: $(TARGET)

$(TARGET): $(SRC) value.h test.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

build.dll: $(DLL)

$(DLL): $(SRC) value.h test.h
	$(CXX) $(CXXFLAGS) -shared $(SRC) -o $(DLL)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET) $(DLL)
