import statistics
def compute_stats(file_list):
    for f in file_list:
        fd = open('results/' + f + '.txt')
        flines = fd.readlines()
        cycles = []
        for l in flines:
            if l.startswith('cycles'):
                run_cycles = l.split(' ')[-1]
                try:
                    cycles.append(float(run_cycles.strip()))
                except:
                    continue
        avg = sum(cycles)/len(cycles)
        sd = statistics.stdev(cycles)
        print(f"{f} avg: {avg} stdev: {sd}")

#resultfiles = ['measurement_overhead', 'syscall_overhead', 'loop_overhead', 'process_creation_overhead', 'process_context_switch', 'kernel_thread_context_switch',
#        'thread_creation_overhead']
resultfiles = ['procedure_overhead0','procedure_overhead2', 'procedure_overhead3', 'procedure_overhead4', 'procedure_overhead5', 'procedure_overhead6', 'procedure_overhead7' ]
compute_stats(resultfiles)
