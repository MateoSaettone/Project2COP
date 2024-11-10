CXX = g++
CXXFLAGS = -std=c++11
TARGET = filesystem
SOURCES = FileSystem.cpp FileSystemTester.cpp
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET).exe
