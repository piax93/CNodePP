#!/bin/bash

pushd $(dirname "$0") &> /dev/null

if [ -z $1 ]; then
	echo "Give me a name for the module"
	exit
fi

if [ $1 == "static" ]; then
	echo "'static' is not permitted as name"
	exit
fi

filename="module/$1.cpp"
if [ -e $filename ]; then
	echo "Module already exists"
else
	cat <<EOT > $filename
#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
	response.setBody("<h2>Hi! I'm your new module, nice to meet you.</h2>");
}

}
EOT
fi

popd &> /dev/null
