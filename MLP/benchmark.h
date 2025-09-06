
#!/ bin / bash
make

DIR="instances"

for file in "$DIR"/*; do
    if [ -f "$file" ]; then
        echo "Processando nova instância: $file"
        ./mlp "$file"
    fi
done

echo "Finalizado"

