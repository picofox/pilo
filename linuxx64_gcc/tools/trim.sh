# !/bin/sh
find -name *.cpp -exec sed -i 's/\r//g' {} \; 
find -name *.c -exec sed -i 's/\r//g' {} \; 
find -name *.h -exec sed -i 's/\r//g' {} \; 
find -name *.hpp -exec sed -i 's/\r//g' {} \; 
