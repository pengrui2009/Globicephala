# !/bin/sh
#comments
#replace all the 
find ./ -name "*.h"|xargs sed -i 's/
find ./ -name "*.c"|xargs sed -i 's/
find ./ -name "*.h"|xargs sed -i 's/\t/    /g'
find ./ -name "*.c"|xargs sed -i 's/\t/    /g'