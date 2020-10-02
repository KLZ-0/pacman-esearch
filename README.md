# pacman-esearch

[![Build Status](https://travis-ci.org/KLZ-0/pacman-esearch.svg?branch=master)](https://travis-ci.org/KLZ-0/pacman-esearch)
[![AUR Status](https://img.shields.io/badge/AUR-v2.1.7-blue.svg)](https://aur.archlinux.org/packages/esearch/)

This is the rewritten version of pacman-esearch in plain C

The original C++ version can be found in the `v1` branch of this repository

Inspired by [https://github.com/fuzzyray/esearch](https://github.com/fuzzyray/esearch)

# Features

- Output similar to gentoo esearch
- Combination of pacman search (lists all packages matching a pattern) and pacman info (prints verbose information about a package)
- Offline search
- Options and features similar to those of gentoo esearch, including:
  - **regex support**
  - filter **Installed or NOT installed** packages
  - show detailed information about **multiple** packages at the same time (unlike pacman -Si)
  - support for **colored output using ANSI codes** by default (can be disabled with `-n`)

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

# Troubleshooting

`Error: Failed to open database!` - run `eupdatedb`.

# Requirements
- **GCC**
