S_FLAGS = -std=c++11 -Wall -c
S_FILES = Test.cpp ConsoleReporter.cpp TestCollection.cpp
O_FILES = $(S_FILES:%.cpp=%.o)

all:  libfrisk.a

libfrisk.a:
	@echo Building Frisk...
	@$(CXX) $(S_FLAGS) $(S_FILES)
	@ar cq ../../lib/libfrisk.a $(O_FILES)
	@rm -rf $(O_FILES)
	@echo Finished.
