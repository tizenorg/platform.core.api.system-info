#!/bin/bash

sourcePath=$1
configFile=/etc/config/model-config.xml
headerFile=$sourcePath/usr/include/system/system_info_tizen_prop.h
sourceFile=$sourcePath/system_info_tizen_prop.c
libraryFile=$sourcePath/libsystem-info-tizen-prop.so

echo "#include \"system_info_tizen_prop.h\"" > $sourceFile

echo "const struct tizen_prop tizen_prop_impl =" >> $sourceFile
echo "{" >> $sourceFile
while read KEY NAME VAL
do
	if [ $KEY != \<key ]; then
		continue
	fi

	nameSuffix=${NAME#n*\"}
	varName=${nameSuffix%\"}
	varName=${varName//./_}
	varName=${varName//\//_}

	while read VARTYPE VARNAME ETC
	do
		if [ $VARNAME ]; then
			if [ $VARNAME = "tizen_prop_type" ]; then
				break
			fi
		fi
		vnPrefix=${VARNAME%;}
		if [ $vnPrefix ]; then
			if [ $varName = $vnPrefix ]; then
				varPrefix=${VAL#t*>}
				varValue=${varPrefix%<*>}

				if [ $VARTYPE = "char" ]; then
					if [ $varValue = "true" ]; then
						varValue=TIZEN_PROP_TRUE
					else
						varValue=TIZEN_PROP_FALSE
					fi
				elif [ $VARTYPE = "char*" ]; then
					varValue=\"$varValue\"
				fi

				echo "	."$varName "=" $varValue"," >> $sourceFile
			fi
		fi
	done < $headerFile

done < $configFile
echo "};" >> $sourceFile

echo "const struct tizen_prop_type tizen_prop_type_impl =" >> $sourceFile
echo "{" >> $sourceFile
while read KEY NAME VAL
do
	if [ $KEY != \<key ]; then
		continue
	fi

	nameSuffix=${NAME#n*\"}
	varName=${nameSuffix%\"}
	varName=${varName//./_}
	varName=${varName//\//_}

	while read VARTYPE VARNAME ETC
	do
		if [ $VARNAME ]; then
			if [ $VARNAME = "tizen_prop_type" ]; then
				break
			fi
		fi
		vnPrefix=${VARNAME%;}
		if [ $vnPrefix ]; then
			if [ $varName = $vnPrefix ]; then
				echo "	."$varName "=" \"$VARTYPE"\"," >> $sourceFile
			fi
		fi
	done < $headerFile
	
done < $configFile
echo "};" >> $sourceFile

gcc -fPIC -c $sourceFile -o $sourceFile".o" -Wall
gcc -shared -o $libraryFile $sourceFile".o"
rm $sourceFile
rm $sourceFile".o"
