echo "Configuring clangd in a project"
echo $(pwd)
cat > .clangd << 'EOF'
CompileFlags:
  Add: 
    - -I/home/ivan/Desktop/Coding/MIPT/4sem/Complexity/source
    - -std=c++20
EOF

echo "Start building the project"
cmake -S . -B source/build && cmake --build source/build

rm main 2>/dev/null
mv source/build/main .
