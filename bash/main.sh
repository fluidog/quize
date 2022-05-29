#! /bin/bash
A=$1
echo $A
A=${A:-ello world}
echo $A

[ -v A ]
echo $?



case $1 in 
	a | b )
		echo hello ;;
esac

exit 0;

test()
{
	echo hello-$1
	echo hello-$2
}

VAL="\"a b\" c"
name=zhang
case $name in 
	zhang ) echo "Im zhang" ;;
	liu ) echo "Im liu" ;;
	* ) echo "Im other" ;;
esac


echo hello | (cat ; echo abc) | cat
