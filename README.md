# pacman-esearch

[![Build Status](https://travis-ci.org/KLZ-0/pacman-esearch.svg?branch=master)](https://travis-ci.org/KLZ-0/pacman-esearch)

Replacement for both pacman -Ss and pacman -Si

# Features

- Output similar to esearch
- Combination of pacman search (lists all packages matching a pattern) and pacman info (prints verbose information about a package)
- It uses a search index which can be updated with the included eupdatedb script
- It does not support every feature of gentoo esearch, but:
  - It can search **explicitly Installed or NOT installed**
  - It **supports regex**
  - It can show detailed information about **multiple** packages at the same time unlike pacman -Si
  - It supports **colored output using ANSI codes** by default - there is also an option to disable it
  - it can search optionally also in **descriptions**

# Installation

Compile from source:

`make`

Install to `/usr/bin/`:

`sudo make install`

Create the search index:

`eupdatedb`

NOTE: the search index is stored in `$HOME/.cache/` on per-user basis so run eupdatedb as regular user (the one who will be using esearch) -- For example the search index for user `john` will be stored in `/home/john/.cache/` or wherever the `$HOME` variable points

Optionally remove redundant objects: `make clean`

If you no longer want esearch: `sudo make uninstall`

# Usage

**`eupdatedb [-q|--quiet]`**

`-q, --quiet`     displays nothing, just updates the search index - can be used in cron jobs or other scripts (Don't forget to set the HOME variable explicitly when the system crontab is used).


**`esearch [options] <pattern>`**

`--instonly, -I` Find only packages which are installed

`--notinst, -N` Find only packages which are NOT installed

`--nocolor, -n` Don't use ANSI codes for colored output

`--searchdesc, -S` Search also in package description

`--version, -v` Show version

`--help, -h` Show help message

# FAQ

Q: I am getting `Failed to open database!` whenever I run esearch.

A: This error means esearch can't find the search index, check if it exists in the desired location or run `eupdatedb`. Optionally check if you have write permissions in `~/.cache/`.

Q: Why was it created?

A: I am a fan of Gentoo and when I started to use Arch on my laptop, I missed [esearch](https://github.com/fuzzyray/esearch) and I wasn't happy with the search capabilities of pacman

**Why C++ ?** -- Combining the two command in a shell script would be significantly slower (I tried it..)

# Requirements
- **Arch linux and pacman** (obviously) - It uses pacman -Si and pacman -Qqe to fill up its databases
- **GCC g++ with -std=c++11** (or -std=c++14) - already present in most cases
