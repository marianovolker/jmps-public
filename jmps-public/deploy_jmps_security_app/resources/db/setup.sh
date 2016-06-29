#!/bin/sh

final_output=""
rc=0
for i in ./*.sql
do
	output=$(mysql -uroot -ppatricia4195 <$i 2>&1)
	final_output="$final_output [$output]\n"
	rc=$(expr $rc + $?)
done

if test $rc -ne 0
then
	echo "HAY ERRORES en la creacion de la base de datos"
	echo $final_output
	exit 1
fi
exit 0

