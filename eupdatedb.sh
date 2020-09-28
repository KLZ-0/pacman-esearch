#!/bin/bash

quiet=0
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -q|--quiet)
    quiet=1
    shift
    ;;
esac
done

# if .cache not exists, create it
mkdir --parents ~/.cache

test $quiet -eq 0 && echo -e "\033[1;34m [1/2] \033[0m Gathering installed database.."
pacman -Qe > $HOME/.cache/esearch-database-installed || echo " * eupdatedb: Failed to load/save database"
pacman -Qd >> $HOME/.cache/esearch-database-installed || echo " * eupdatedb: Failed to load/save database"
test $quiet -eq 0 && echo -e "\033[1;34m [2/2] \033[0m Gathering global database.."
pacman -Si > $HOME/.cache/esearch-database || echo " * eupdatedb: Failed to load/save database"

test $quiet -eq 0 && echo -e "\033[1;33m *** \033[0m eupdatedb finished successfully with $(pacman -Ssq | wc -l) global and $(pacman -Qqe | wc -l) installed packages\nSize of search index:"
test $quiet -eq 0 && du -sh $HOME/.cache/esearch-database $HOME/.cache/esearch-database-installed
