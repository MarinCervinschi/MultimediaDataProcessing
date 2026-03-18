CXX = g++
CXXFLAGS = -Wall -std=c++20 -g
SRCS = $(wildcard *.cpp)
BINS = $(SRCS:.cpp=)

all: $(BINS)

# Regola per compilare ogni singolo .cpp nel proprio eseguibile indipendente
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf *.dSYM

fclean: clean
	rm -f $(BINS)

re: fclean all

.PHONY: all clean fclean re