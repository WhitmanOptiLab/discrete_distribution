# for i in 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072
for i in 10 100 1000 10000 100000 1000000 10000000 100000000
do
  ./testscript.sh 100 $i
done
