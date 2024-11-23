# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -D_REENTRANT -fPIC \
           -I/usr/include/x86_64-linux-gnu/qt5 \
           -I/usr/include/x86_64-linux-gnu/qt5/QtCore \
           -I/usr/include/x86_64-linux-gnu/qt5/QtGui \
           -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
           -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork

# Qt tools
MOC = moc
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lQt5Core -lQt5Gui -lQt5Widgets -lQt5Network

# Source files and build targets
SOURCES = server.cpp main.cpp
HEADERS = server.h
OBJECTS = server.o main.o moc_server.o
TARGET = ChatServer

# Build Rules
all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile server.cpp
server.o: server.cpp server.h
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

# Compile main.cpp (entry point for server)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Create moc file from server.h (for Qt signals/slots)
moc_server.o: moc_server.cpp
	$(CXX) $(CXXFLAGS) -c moc_server.cpp -o moc_server.o

# Generate moc file from header (for Qt meta-object compiler)
moc_server.cpp: server.h
	$(MOC) server.h -o moc_server.cpp

# Clean up build files
clean:
	rm -f $(OBJECTS) moc_server.cpp $(TARGET)
