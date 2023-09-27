#!/bin/bash

while IFS= read -r line || [[ -n "$line" ]]; do
  echo $line
  if [[ "${line%% *}" != "PATH"]]; then
    continue   
  fi
  path_from_user= ${line#*}


done
