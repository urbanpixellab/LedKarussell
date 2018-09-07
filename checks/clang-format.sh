cd $(dirname "$0")/..

clang-format \
-style=file -i src/*.[ch]pp

cd checks
