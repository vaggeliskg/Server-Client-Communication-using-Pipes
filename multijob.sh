# #!/bin/bash


for file in "$@"; do
    
    while read line || [ -n "$line" ]; do # [ -n "$line" ] used to execute the final line as well
        
        output="./jobCommander issueJob $line"
        
        eval "$output"
    done < "$file"  
done

echo 