#! /bin/sh
#comments
#replace all the  to whitespace and replace all the tab to 4's whitespace
find ./ -name "*.h"|xargs sed -i 's///g'
find ./ -name "*.c"|xargs sed -i 's///g'
find ./ -name "*.h"|xargs sed -i 's/\t/    /g'
find ./ -name "*.c"|xargs sed -i 's/\t/    /g'
