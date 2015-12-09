#!/bin/bash


awk -F '\t' '{print $1}' $1 | iconv -f gbk -t utf8 > nl.txt
awk -F '\t' '{print $1}' $2 | iconv -f gbk -t utf8 > gb.txt
iconv -f gbk -t utf8 $2 > gb.utf8

while read line
do
  gp=`grep -n ^$line$ gb.txt`
  if [ "$gp" != "" ]
  then
    num=${gp%%:*}
    sed -n "${num}p" gb.utf8 >> nb.txt
  else
    echo "$line 10.000000" >> nb.txt
  fi
done < nl.txt

sort -u nb.txt | iconv -f utf8 -t gbk > $3
rm nl.txt gb.txt gb.utf8 nb.txt


