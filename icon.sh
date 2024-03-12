#!/bin/bash

## How do I embed multiple sizes in an .ico file?
## https://superuser.com/a/683203

## usage: $0 file.svg

for size in 16 32 48 128 256; do
    inkscape --export-filename=$size.png -w $size -h $size $1 &>/dev/null
done
convert 16.png 32.png 48.png 128.png 256.png -colors 256 $1.ico
rm -f 16.png 32.png 48.png 128.png 256.png

