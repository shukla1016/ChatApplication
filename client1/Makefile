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

# Linker flags for Qt5 libraries
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lQt5Core -lQt5Gui -lQt5Widgets -lQt5Network

# Source files and build targets
SOURCES = main.cpp chat_window.cpp
HEADERS = chat_window.h
OBJECTS = main.o chat_window.o moc_chat_window.o
TARGET = ChatClient

# Build Rules
all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile chat_window.cpp
chat_window.o: chat_window.cpp chat_window.h
	$(CXX) $(CXXFLAGS) -c chat_window.cpp -o chat_window.o

# Compile main.cpp (entry point for the client)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Create moc file from chat_window.h (for Qt signals/slots)
moc_chat_window.o: moc_chat_window.cpp
	$(CXX) $(CXXFLAGS) -c moc_chat_window.cpp -o moc_chat_window.o

# Generate moc file from header (for Qt meta-object compiler)
moc_chat_window.cpp: chat_window.h
	$(MOC) chat_window.h -o moc_chat_window.cpp

# Clean up build files
clean:
	rm -f $(OBJECTS) moc_chat_window.cpp $(TARGET)
