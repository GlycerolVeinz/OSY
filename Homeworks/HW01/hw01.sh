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
      zipname="output.tgz"
      tar -cf $zipname --files-from /dev/null
      ;;
    \?)
      echo "ERROR -$OPTARG" 1>&2
      exit 2
      ;;
  esac
done

# help option
if [[ $help -eq 1 ]]; then
  echo "Usage: ./hw01.sh [-h] [-z zipname]"
  echo ""
  echo "Reads lines from stdin,"
  echo "if line is in format PATH 'path_from_user' then prints all files in 'path_from_user' directory" 
  echo ""
  echo "Options:"
  echo "  -h: display help"
  echo "  -z: zip all FILES in directory"
  exit $ret
fi

# main
while IFS= read -r line || [[ -n "$line" ]]; do
  
  read -r line_start path_from_user <<< "$line"

  if [[ $line_start != "PATH" ]]; then
    continue   
  fi

  if [[ ! -d $path_from_user ]]; then
    echo "ERROR $path_from_user" 1>&2
    ret=1
    continue
  fi

  for file in "$path_from_user"/*; do
    file_type=$(file -b "${file}")

    # this one works, don't go back to prototype
    case $file_type in
      "symbolic link"*)
        echo "LINK $path_from_user/$file $(readlink -f "$file")"
        ;;
      "directory"*)
        echo "DIR $path_from_user/$file"
        ;;
      *)
        number_of_lines=$(wc -l < "$file")
        first_line=$(head -n 1 "$file")
        echo "FILE '$file' $number_of_lines '$first_line'"
        if [[ $zip -eq 1 ]]; then
          tar -rf $zipname "$file"
        fi
        ;;
    esac

    # ----PROTOTYPE CODE----
    # if $file_type | grep -q "symbolic link" ; then
    #   echo "LINK $path_from_user/$file $(readlink -f "$file")"

    # elif $file_type | grep -q "directory" ; then
    #   echo "DIR $path_from_user/$file"

    # else
    #   number_of_lines=$(wc -l < "$file")
    #   first_line=$(head -n 1 "$file")
    #   echo "FILE $path_from_user/$file $number_of_lines $first_line"
    # fi
    # ----------------------

  done
done

exit $ret
