#!/bin/csh
echo "proj.4:"
echo "ARRAYSIZE,SIMD MM/s, Non-SIMD MM/s"
# for each size (1KiB, 2KiB, 4KiB, 8KiB, 16KiB, 32KiB, 64KiB, 256KiB, 512KiB, 1MiB, 2MiB, 4MiB, 8MiB)
foreach ARRAYSIZE (1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608)
    g++ -DARRAYSIZE=$ARRAYSIZE proj.4.cpp -o proj.4 -lm -fopenmp
    ./proj.4
end