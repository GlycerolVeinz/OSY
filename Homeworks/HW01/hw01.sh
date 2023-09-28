#!/bin/bash
zip=0
help=0
ret=0


# options
while getopts ":hz:" opt; do
  case $opt in
    h)
      help=1
      ;;
    z)
      zip=1
      zipname=$OPTARG
      ;;
    \?)
      echo "ERROR -$OPTARG" 1>&2
      exit 2
      ;;
  esac
done

if [[ $help -eq 1 ]]; then
  echo "Usage: ./hw01.sh [-h] [-z zipname]"
  echo "Reads lines from input, if line starts with PATH prints all files on that path"
  echo "Options:"
  echo "  -h: display help"
  echo "  -z: zip all FILES in directory"
  exit $ret
fi

while IFS= read -r line || [[ -n "$line" ]]; do
  
  if [[ "${line%% *}" != "PATH" ]]; then
    continue   
  fi
  line_arr=($line)
  path_from_user=${line_arr[1]}

  if [[ ! -d $path_from_user ]]; then
    echo "ERROR $path_from_user" 1>&2
    ret=1
    continue
  fi

  for file in $path_from_user/*; do
    file_type=$(file ${file})

    if [[ $($file_type | grep -q "symbolic link") ]]; then
      echo "LINK $path_from_user/$file $(readlink -f $file)"
    
    elif [[ $($file_type | grep -q "directory") ]]; then
      echo "DIR $path_from_user/$file"
    
    else
      number_of_lines=$(wc -l < $file)
      first_line=$(head -n 1 $file)
      echo "FILE $path_from_user/$file $number_of_lines $first_line"
    fi

  done
done

exit $ret
