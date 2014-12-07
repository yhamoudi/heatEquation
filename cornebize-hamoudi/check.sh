if [ $# -ne 1 ]
then
    echo "Syntax: " $0 " <file>"
    exit
fi
mpirun -n 1 average < $1 > tmp1.out
for i in {2..10}
do
    mpirun -n $i average < $1 > tmp2.out
    diff tmp1.out tmp2.out
    if [ $? -ne 0 ]
    then
        echo "Error for n="$i
        exit
    fi
done
echo "ok"
