echo "num_threads,length,parallel_scan_time" > data/scan1.csv
for ((p=1; p<=22; p+=1))
do
	./bin/scan1 10 1000000 $p >> data/scan1.csv
	echo executing with $p cores.
done	

