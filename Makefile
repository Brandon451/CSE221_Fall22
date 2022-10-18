CC=gcc
CPP=g++
OPTS=-O0
SRC=./src/
BIN=./bin
INC=./inc/

all: context_switch loop_overhead measurement_overhead procedure_overhead process_creation_overhead thread_creation_overhead syscall_overhead memory_latency page_fault_service mem_bandwidth rtt_server rtt_client conn_setup_teardown_server conn_setup_client conn_teardown_client fileread_local_sequential fileread_local_random file_contention network_peak_bandwidth file_cache

%: $(SRC)%.cpp
	 @mkdir -p $(BIN)
	 $(CPP) $(OPTS) -I $(INC) -o $(BIN)/$@ $< -lpthread

%: $(SRC)%.c
	 @mkdir -p $(BIN)
	 $(CPP) $(OPTS) -I $(INC) -o $(BIN)/$@ $< -lpthread

.PHONY: clean

clean:
	rm $(BIN)*;
