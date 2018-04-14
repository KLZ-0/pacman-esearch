# pacman-esearch

Basic replacement for both pacman -Ss and pacman -Si
It was developed for personal use but you can use and imporove it however you like - if you find a bug, please open an issue

# Features

> - Prints output similarly to esearch -- Gentoo like experience ;)
> - Combination of pacman search (lists all packages matching a pattern) and pacman info (prints verbose information about only one package)
> - Using independent database which can be updated with the included eupdatedb script

# WHY?

I am a fan of Gentoo and when I turned to Arch, I missed the esearch functionality -- so here we go :)
Why C++ ? -- Combining the two command in a shell script would be significantly slower (I tried it..)

# Requirements
> - **Arch linux and pacman** (obliviously)
> - **GCC g++ with -std=c++14**
