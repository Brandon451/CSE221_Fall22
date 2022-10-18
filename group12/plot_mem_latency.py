import subprocess
import pandas as pd
from  matplotlib import pyplot as plt
import numpy as np
import time

def run_experiment(size, stride, iterations):
    cmd = f'./bin/memory_latency -n {size} -s {stride} -i {iterations}'.split()
    start = time.time()
    with open('results/memory_latency.csv', 'a') as f:
        f.write(f'{size},{stride},')
        f.flush()
        p = subprocess.Popen(cmd, stdout=f, stderr=subprocess.STDOUT)
        p.communicate()
        f.flush()
    return time.time() - start

def run_experiments(stride_lim, size_lim, iterations):
    experiments  = [] 
    stride = 2
    with open('results/memory_latency.csv', 'w') as f:
        f.write('size,stride,cycles\n')
    prev_runtime = 0
    while stride <= stride_lim:
        size = 1024
        next_interval = size*4
        interval_size = 32
        increments = (next_interval - size)//interval_size
        prev_runtime = 0
        while size <= size_lim:
            if size == next_interval:
                next_interval = size*4
                increments = (next_interval - size)//interval_size
            runtime = run_experiment(size, stride, iterations)
            if runtime > 2*prev_runtime:
                interval_size = interval_size // 2
            prev_runtime = runtime
            size = size + increments
        experiments.append(stride)
        stride *= 2
    return experiments 

def plot_experiments(experiments):
    df = pd.read_csv('results/memory_latency.csv')
    # get unique experiments
    experiments = list(set(experiments))
    for exp in experiments:
        df_subset = df.loc[df['stride'] == exp]
        plt.plot(np.log2(df_subset['size']), df_subset['cycles'], label=str(exp), marker='o')
    plt.legend()
    plt.show()

experiments = run_experiments(32, 6291456*8, 10000)
plot_experiments(experiments)
#run_experiment(4096,4,1000)
