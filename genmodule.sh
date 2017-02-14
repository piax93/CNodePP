#!/bin/bash

pushd $(dirname "$0") &> /dev/null

if [ -z $1 ]; then
  echo "Give me a name for the module"
else
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
fi

popd &> /dev/null
