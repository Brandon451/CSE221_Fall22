#mkdir -p part1_results/ part2_results/ part3_results/ part4_results/
####Part1
#./bin/measurement_overhead > part1_results/measurement_overhead.txt
#./bin/loop_overhead > part1_results/loop_overhead.txt
#./bin/procedure_overhead > part1_results/procedure_overhead.txt
#./bin/syscall_overhead > part1_results/syscall_overhead.txt
#./bin/process_creation_overhead > part1_results/process_creation_overhead.txt
#./bin/thread_creation_overhead > part1_results/thread_creation_overhead.txt
#./bin/context_switch process >> part1_results/process_context_switch.txt

####Part2
#./bin/memory_latency -n 32 -s 128 -i 100 > part2_results/memory_latency.txt  ## n means array size, s means stride and i means number of iterations.
#./bin/mem_bandwidth > part2_results/mem_bandwidth.txt  
#./bin/page_fault_service > part2_results/page_fault_service.txt 

#####Part3
#./bin/rtt_client.c >> part3_results/rtt_client.txt  ##Make sure you run ./bin/rtt_server before client and  edit the server Ip address to the server you are testing.
#./bin/network_peak_bandwidth c >> part3_results/network_peak_bandwidth.txt ##Make sure you run ./bin/network_peak_bandwidth s in server before client, edit the server Ip address to the server you are testing.
#./bin/conn_setup_client >> part3_results/conn_setup_client.txt ##Make sure you run ./bin/conn_setup_teardown_server before client and in edit the server Ip address to the server you are testing. 
#./bin/conn_teardown_client >> part3_results/conn_teardown_client.txt ##Make sure you run ./bin/conn_setup_teardown_server before client and in edit the server Ip address to the server you are testing. 


####Part4
#./create_files.sh ##Create files of different sizes
#./bin/cache_file >> part4_results/cache_file_size.txt

# File read sizes
# 512K
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 512 1 >> part4_results/fileread_local_sequential512k.txt
#done
### 1M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 1 2 >> part4_results/fileread_local_sequential1m.txt
#done
## 4M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 4 2 >> part4_results/fileread_local_sequential4m.txt
#done
## 16M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 16 2 >> part4_results/fileread_local_sequential16m.txt
#done
## 128M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 128 2 >> part4_results/fileread_local_sequential128m.txt
#done
# 256M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 256 2 >> part4_results/fileread_local_sequential256m.txt
#done
#### 512M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 512 2 >> part4_results/fileread_local_sequential512m.txt
#done
#
#512K
#for value in {1..50}
#do
#    ./bin/fileread_local_random 512 1 >> part4_results/fileread_local_random512k.txt
#done
##1M
#for value in {1..50}
#do
#    ./bin/fileread_local_random 1 2 >> part4_results/fileread_local_random1m.txt
#done
## 4M
#for value in {1..50}
#do
#    ./bin/fileread_local_random 4 2 >> part4_results/fileread_local_random4m.txt
#done
## 16M
#for value in {1..50}
#do
#    ./bin/fileread_local_random 16 2 >> part4_results/fileread_local_random16m.txt
#done
## 128M
#for value in {1..50}
#do
#    ./bin/fileread_local_random 128 2 >> part4_results/fileread_local_random128m.txt
#done
## 256M
#for value in {1..50}
#do
#    ./bin/fileread_local_random 256 2 >> part4_results/fileread_local_random256m.txt
#done
## 512M
#for value in {1..50}
#do
#    ./bin/fileread_local_random 512 2 >> part4_results/fileread_local_random512m.txt
#done

#./bin/filesystem_contention -n 1 -b 1000 >> part4_results/filesystem_contention1.txt       ##n is number of Process and b is number of blocks to read.
#./bin/filesystem_contention -n 2 -b 1000 >> part4_results/filesystem_contention2.txt
#/bin/filesystem_contention -n 3 -b 1000 >> part4_results/filesystem_contention3.txt
#./bin/filesystem_contention -n 4 -b 1000 >> part4_results/filesystem_contention4.txt
#./bin/filesystem_contention -n 5 -b 1000 >> part4_results/filesystem_contention5.txt
#./bin/filesystem_contention -n 6 -b 1000 >> part4_results/filesystem_contention6.txt
#./bin/filesystem_contention -n 7 -b 1000 >> part4_results/filesystem_contention7.txt
#./bin/filesystem_contention -n 8 -b 1000 >> part4_results/filesystem_contention8.txt
#./bin/filesystem_contention -n 9 -b 1000 >> part4_results/filesystem_contention9.txt
#./bin/filesystem_contention -n 10 -b 1000 >> part4_results/filesystem_contention10.txt
#./bin/filesystem_contention -n 11 -b 1000 >> part4_results/filesystem_contention11.txt
#./bin/filesystem_contention -n 12 -b 1000 >> part4_results/filesystem_contention12.txt


