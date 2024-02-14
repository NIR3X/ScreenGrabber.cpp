AR = ar
CXX = g++
CXXFLAGS = -std=c++2a -Wall -O2 -march=x86-64 -fno-rtti
LDFLAGS = -s -static -lgdi32 -lgdiplus -lole32 -lshcore

SRC = MonitorInfo.cpp ScreenGrabber.cpp StreamOnHGlobal.cpp
OBJ = $(SRC:.cpp=.o)
DEST = ScreenGrabber.a
TEST_SRC = ScreenGrabberTest.cpp
TEST_DEST = ScreenGrabberTest.exe

build: $(OBJ)
	$(AR) r -o $(DEST) $(OBJ)

build-test: build
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(DEST) -o $(TEST_DEST) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(DEST) $(TEST_DEST)
	rm -f *.jpg
