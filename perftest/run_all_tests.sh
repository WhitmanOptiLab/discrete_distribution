echo "SELECTNUM, normal_static_wrs, normal_static_heap, normal_changing_wrs, normal_changing_heap, weibull_static_wrs, weibull_static_heap, weibull_changing_wrs, weibull_changing_heap" > results.csv
#for i in 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072
for i in 10 100 1000 10000 100000 1000000 10000000
do
  ./testscript.sh 10 $i
done
