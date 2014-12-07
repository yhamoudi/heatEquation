if [ $# -ne 1 ]
then
    echo "Syntax: " $0 " <file>"
    exit
fi
for i in {1..10}
do
    mpirun -n $i average < $1 #> /dev/null
    if [ $? -ne 0 ]
    then
        echo "Error for n="$i
        exit
    fi
done
echo "ok"
