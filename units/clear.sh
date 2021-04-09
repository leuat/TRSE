find . -name "*.asm" -type f -delete
find . -name "*.prg" -type f -delete
find . -name "*.sym" -type f -delete
find . -name "*.gb" -type f -delete
find . -name "*.o" -type f -delete
find . -name "*.dup" -type f -delete
find . -name "*.nes" -type f -delete
find . -name "*.com" -type f -delete

#find . -name "*.trse" -type f  -exec sed -i /project_path/d {} +
#find . -name "*.trse" -type f  -exec sed -i /open_files/d {} +
#find . -name "*.trse" -type f  -exec sed -i /current_file/d {} +

find . -name .backup -exec rm -rf {} \;

