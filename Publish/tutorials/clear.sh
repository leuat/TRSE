find . -name "*.asm" -type f -delete
find . -name "*.prg" -type f -delete
find . -name "*.nes" -type f -delete
find . -name "*.trse" -type f  -exec sed -i /project_path/d {} +
find . -name "*.trse" -type f  -exec sed -i /open_files/d {} +
