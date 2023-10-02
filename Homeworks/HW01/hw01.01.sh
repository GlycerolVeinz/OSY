#!/bin/bash
zip=0
help=0
ret=0

# options
while getopts ":hz" opt; do
  case $opt in
    h)
      help=1
      ;;
    z)
      zip=1
      zipname="output.tgz"
      if ! tar -cf $zipname --files-from /dev/null; then
        ret=2
      fi
      ;;
    \?)
      echo "ERROR -$OPTARG" 1>&2
      ret=2
      exit $ret
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


# "main"
while IFS= read -r line || [[ -n "$line" ]]; do

  read -r line_start path_from_user <<< "$line"

  if [[ $line_start != "PATH" ]]; then
    continue   
  fi

  # non existant path
  if [[ (! -e $path_from_user) && (! -L $path_from_user) ]]; then
    echo "ERROR '$path_from_user'" 1>&2
    ret=1
    continue
  fi

  file_type=$(file -b "${path_from_user}")

  # determine file type, write output
  case $file_type in
      "symbolic link"*)
        link_path=$(readlink "$path_from_user")
        echo "LINK '$path_from_user' '$link_path'"
        ;;

      "broken symbolic link"*)
        link_path=$(readlink "$path_from_user")
        echo "LINK '$path_from_user' '$link_path'"
        ;;

      "directory"*)
        echo "DIR '$path_from_user'"
        ;;

      *)
        number_of_lines=$(wc -l < "$path_from_user")
        
        if ! first_line=$(head -n 1 "$path_from_user"); then
          ret=2
          exit $ret
        fi

        echo "FILE '$path_from_user' $number_of_lines '$first_line'"
        if [[ $zip -eq 1 ]]; then
          tar -rf "$zipname" "$path_from_user"
        fi
        ;;
  esac

done

exit $ret
