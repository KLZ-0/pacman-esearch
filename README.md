# pacman-esearch

[![Build Status](https://travis-ci.org/KLZ-0/pacman-esearch.svg?branch=master)](https://travis-ci.org/KLZ-0/pacman-esearch)
[![AUR Status](https://img.shields.io/badge/AUR-esearch-blue.svg)](https://aur.archlinux.org/packages/esearch/)

Print verbose information about packages matching a regular expression

Inspired by [https://github.com/fuzzyray/esearch](https://github.com/fuzzyray/esearch)

# Features

- Output and options similar to Gentoo esearch
- Search index
- Support for regular expressions and filters (list of all options can be obtained by `-h`)
- Print information about **all matched** packages at the same time (unlike `pacman -Si`)

# Installation

## From AUR

`git clone https://aur.archlinux.org/esearch.git && cd esearch && makepkg -si`

## From source

Clone this repository:

`git clone https://github.com/KLZ-0/pacman-esearch.git && cd pacman-esearch`

Compile the executable:

`make`

Install to `/usr/bin/`:

`sudo make install`

Create the search index after installation:

`eupdatedb`

NOTE: the search index is stored in `$HOME/.cache/esearch/` on per-user basis so run eupdatedb as regular user (the one who will be using esearch) -- For example the search index for user `john` will be stored in `/home/john/.cache/esearch/`

# Usage

**`eupdatedb [-q|--quiet]`**

`-q, --quiet`     displays nothing, just update the search index - can be used in cron jobs or other scripts (Don't forget to set the HOME variable explicitly when the system crontab is used).


**`esearch [options] <pattern>`**

`--instonly, -I` Find only packages which are installed

`--notinst, -N` Find only packages which are NOT installed

`--nocolor, -n` Don't use ANSI codes for colored output

`--nowarndb, -w` Do not complain about database age

`--exact-match, -e` Show only exact match

`--version, -v` Show version

`--help, -h` Show help message

# Requirements
- GCC
- CMake
- Python 3
