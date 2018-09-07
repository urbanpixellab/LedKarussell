cd ..

cloc \
--3 --by-file src \
> checks/cloc.txt

#cloc \
#--3 --by-file src \
#--xml > checks/cloc.xml

cd checks
