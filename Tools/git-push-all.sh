#!/bin/bash
#
# Perform `git push` for every git repo in the current directory
# Inspired by: https://stackoverflow.com/a/28348077/7710071
#
# Equivalent to the one liner:
# for i in */.git; do ( echo $i; cd $i/..; git push; ); done

for i in */.git
do
	(
	echo $i
	cd $i/..
	git push
	)
done
