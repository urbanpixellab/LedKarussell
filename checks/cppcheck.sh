# If header file config.h is missing, first run: autoreconf -vfi; ./configure

cd ..

#cppcheck --check-config \
#--enable=all --suppress=missingIncludeSystem -Isrc src/*.[ch]pp  \
#2> checks/cppcheck_checkconfig.txt

cppcheck \
--enable=all --suppress=missingIncludeSystem -Isrc src/*.[ch]pp --suppress=unusedFunction:tests/string_utils_test.cxx:50 \
2> checks/cppcheck.txt

#cppcheck \
#--xml-version=2 \
#--enable=all --suppress=missingIncludeSystem -Isrc src/*.[ch]pp tests/*.[ch]xx --suppress=unusedFunction:tests/string_utils_test.cxx:50 \
#2> checks/cppcheck.xml

cd checks
