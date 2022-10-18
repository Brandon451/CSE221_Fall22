Run ``make`` to generate binaries. Binaries for each benchmark are generated in the /bin folder.
Measurements are done after setting freq to constant 1.7Ghz with no dynamic freq scaling. For this we are running with intel acpi-cpufreq driver on i7-1165g7
Benchmarks set cpu affinity to a single core to remove core-migration and set priority to -20 (highest priority linux process)
