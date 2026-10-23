CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread
LDFLAGS = -pthread

COMMON_OBJS = common.o channel.o signals.o
SERVER_BINS = finance logging file
CLIENT_BIN = client

all: $(SERVER_BINS) $(CLIENT_BIN)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

finance: finance.o $(COMMON_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

logging: logging.o $(COMMON_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

file: file.o $(COMMON_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

client: client.o $(COMMON_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

privatetest: clean all
	@echo "Running comprehensive test..."
	@make clean > /dev/null 2>&1
	@make all > /dev/null 2>&1
	@bash lab4-tests.sh
	@make clean > /dev/null 2>&1

clean:
	rm -f *.o $(SERVER_BINS) $(CLIENT_BIN)
	rm -f *.log
	rm -f fifo_*
	rm -rf storage
	rm -f test_*
	rm -rf test_results
	rm -f *_attributes.txt

distclean:
	rm -f *.o $(SERVER_BINS) $(CLIENT_BIN)
	rm -f *.log
	rm -f fifo_*
	rm -rf storage
	rm -f test_*
	rm -rf test_results
	rm -f *_attributes.txt

.PHONY: all clean test