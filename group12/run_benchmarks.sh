#mkdir -p results
#./bin/measurement_overhead > results/measurement_overhead.txt
#./bin/loop_overhead > results/loop_overhead.txt
#./bin/process_creation_overhead > results/process_creation_overhead.txt
#touch results/process_context_switch.txt
#touch results/kernel_thread_context_switch.txt
#for value in {1..100}
#do
#  ./bin/context_switch process >> results/process_context_switch.txt
#done
#for value in {1..100}
#do
#  ./bin/context_switch kernel_thread >> results/kernel_thread_context_switch.txt
#done
#./bin/conn_teardown_client > results/conn_teardown.txt
#./bin/filesystem_contention -n 1 -b 1000 >> part4_results/filesystem_contention1.txt
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
./bin/filesystem_contention -n 12 -b 1000 >> part4_results/filesystem_contention12.txt


# File read sizes
# 512K
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 512 1 >> part4_results/fileread_local_sequential512k.txt
#done
## 1M
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
## 256M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 256 2 >> part4_results/fileread_local_sequential256m.txt
#done
## 512M
#for value in {1..50}
#do
#    ./bin/fileread_local_sequential 512 2 >> part4_results/fileread_local_sequential512m.txt
#done
#
##512K
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
#
