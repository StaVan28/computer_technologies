#!/usr/bin/bash

# set -Eeuo pipefail

NUM_OF_PTHREADS=$1
MAX_PTHREADS=16

# if [[ $NUM_OF_PTHREADS -gt 0 && $NUM_OF_PTHREADS -le $MAX_PTHREADS ]]; then
#    NUM_OF_PTHREADS=$1
# else
#    NUM_OF_PTHREADS=0
# fi

for ((pthread=1; pthread <= $NUM_OF_PTHREADS; pthread++)); do
    echo ""
    echo "thread : $pthread"
    time ./integral.out $pthread
done
