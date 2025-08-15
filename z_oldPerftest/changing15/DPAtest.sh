for i in 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432 67108864;
do 
    g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sampling::DynamicProposalArrayStar"                "-DWEIGHTNUM=$i" -o testUNIFORM uniform_changing_multi.cpp
    g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sampling::DynamicProposalArrayStar"                "-DWEIGHTNUM=$i" -o testNORMAL normal_changing_multi.cpp
    g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sampling::DynamicProposalArrayStar"                "-DWEIGHTNUM=$i" -o testWEIBULL weibull_changing_multi.cpp
    ./test1
    ./test2
    ./test3
    echo "Finished tests for weight number $i"
done
