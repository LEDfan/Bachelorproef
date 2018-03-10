counter=1
while [ $counter -le 32 ]
do
	export OMP_NUM_THREADS=$counter
	rm -rf simulations/test
	python3 mean.py
	echo $counter
	((counter++))
done
