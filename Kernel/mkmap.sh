#!/bin/sh
tmp=$(mktemp)
if [ -f Kernel32 ]; then
    kernel_binary=Kernel32
else
    kernel_binary=Kernel64
fi
nm -n $kernel_binary | awk '{ if ($2 != "a") print; }' | uniq > "$tmp"
printf "%08x\n" "$(wc -l "$tmp" | cut -f1 -d' ')" > kernel.map
cat "$tmp" >> kernel.map
rm -f "$tmp"
