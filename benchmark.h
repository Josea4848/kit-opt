
#!/bin/bash
make

DIR="instances"

for file in "$DIR"/*; do
    if [ -f "$file" ]; then
        echo "Processando nova instância: $file"
        ./tsp "$file"
    fi
done

echo "Finalizado"

