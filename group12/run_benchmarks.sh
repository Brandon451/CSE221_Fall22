mkdir -p results
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


# File read sizes
# 512K
for value in {1..50}
do
    ./bin/fileread_local_sequential 512 1 >> results/fileread_local_sequential512k.txt
done512 * 1024;
# 1M
for value in {1..50}
do
    ./bin/fileread_local_sequential 1 2 >> results/fileread_local_sequential1m.txt
done
# 4M
for value in {1..50}
do
    ./bin/fileread_local_sequential 4 2 >> results/fileread_local_sequential4m.txt
done
# 16M
for value in {1..50}
do
    ./bin/fileread_local_sequential 16 2 >> results/fileread_local_sequential16m.txt
done
# 128M
for value in {1..50}
do
    ./bin/fileread_local_sequential 128 2 >> results/fileread_local_sequential128m.txt
done
# 256M
for value in {1..50}
do
    ./bin/fileread_local_sequential 256 2 >> results/fileread_local_sequential256m.txt
done
# 512M
for value in {1..50}
do
    ./bin/fileread_local_sequential 512 2 >> results/fileread_local_sequential512m.txt
done

#512K
for value in {1..50}
do
    ./bin/fileread_local_random 512 1 >> results/fileread_local_random512k.txt
done
#1M
for value in {1..50}
do
    ./bin/fileread_local_random 1 2 >> results/fileread_local_random1m.txt
done
# 4M
for value in {1..50}
do
    ./bin/fileread_local_random 4 2 >> results/fileread_local_random4m.txt
done
# 16M
for value in {1..50}
do
    ./bin/fileread_local_random 16 2 >> results/fileread_local_random16m.txt
done
# 128M
for value in {1..50}
do
    ./bin/fileread_local_random 128 2 >> results/fileread_local_random128m.txt
done
# 256M
for value in {1..50}
do
    ./bin/fileread_local_random 256 2 >> results/fileread_local_random256m.txt
done
# 512M
for value in {1..50}
do
    ./bin/fileread_local_random 512 2 >> results/fileread_local_random512m.txt
done