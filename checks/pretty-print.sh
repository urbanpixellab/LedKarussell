cd ../src
for i in *.[ch]pp
do
    trueprint -2 $i > ../checks/$i.ps
done
cd ../checks
ls -ltrh *.ps
