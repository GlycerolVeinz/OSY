#!/bin/bash
zip=0
help=0

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
      echo "Invalid option: -$OPTARG" 1>&2
      ;;
  esac
done

if [[ $help -eq 1 ]]; then
  echo "Usage: ./hw01.sh [-h] [-z zipname]"
  echo "Reads lines from input, if line starts with PATH prints all files on that path"
  echo "Options:"
  echo "  -h: display help"
  echo "  -z: zip all FILES in directory"
  exit 0
fi

while IFS= read -r line || [[ -n "$line" ]]; do
  if [[ "${line%% *}" != "PATH" ]]; then
    continue   
  fi
  line_arr=($line)
  path_from_user=${line_arr[1]}
  echo $path_from_user

done

exit 0
