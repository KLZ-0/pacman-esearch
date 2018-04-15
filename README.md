# pacman-esearch

Basic replacement for both pacman -Ss and pacman -Si
It was developed for personal use but you can use and imporove it however you like - if you find a bug, please open an issue

# Features

> - Prints output similarly to esearch -- Gentoo like experience ;)
> - Combination of pacman search (lists all packages matching a pattern) and pacman info (prints verbose information about only one package)
> - Using independent database which can be updated with the included eupdatedb script
> - It does not support every feature of gentoo esearch, but:
>   - It can search **explicitly** Installed || Not installed packages
>   - It can search by **regex** (pattern must be in quotes)
>   - It can show detailed information about **multiple** packages at the same time unlike pacman -Si

# Installation

Compile from source

`make`

Install to /usr/bin/

`sudo make install`

NOTE: databases are stored in $HOME/.cache/ on per-user basis so run eupdatedb as regular user[s] (the one[s] who will be using esearch) [For example the database for user "john" will be stored in /home/john/.cache/ or wherever the $HOME variable points]

# Usage

`eupdatedb [-q|--quiet]`

-q, --quiet     displays nothing, just updates the databases - can be used in cron jobs or other scripts.


`esearch <pkgname> [-I|-N]`

-I              Find only packages which are installed

-N              Find only packages which are NOT installed

# WHY?

I am a fan of Gentoo and when I turned to Arch, I missed the esearch functionality -- so here we go :)
Why C++ ? -- Combining the two command in a shell script would be significantly slower (I tried it..)

# Requirements
> - **Arch linux and pacman** (obliviously) - It uses pacman -Si and pacman -Qqe to fill up its databases
> - **GCC g++ with -std=c++14**
