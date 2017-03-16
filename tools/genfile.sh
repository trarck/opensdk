#！/bin/bash

for file in $(find $1 -name "*.*"); do
    echo "'"$file"'",
done