make
echo "num_threads,length,bc,parallel_scan_time,scan_time" > data/scan2.csv
for ((p=1; p<=44; p+=1))
do
	bc=$((1000000/p))
	./bin/scan2 10 1000000 $p $bc >> data/scan2.csv
	echo executing with $p cores and base case $bc
done	

