for entry in "../instances"/*".instance.json"
do
    echo "$entry" | sed 's/\.\.\/instances\///' | sed 's/\.instance\.json$//'
    
done