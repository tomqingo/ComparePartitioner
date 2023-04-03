for num in {01..18}
do
echo "Pratition$num"
./compare -in_file /data/ssd/qluo/partitioning_benchmark/ISPD98/ISPD98_ibm${num}.hgr -out_file case${num}.hgr >>log_13.txt
done
