#!/bin/bash

SUCCESS=0
ERR_INVALID_ARG=1

function print_help() {
    echo "Usage: ./hw02.sh [-h] [-a] [-b] [-c] [-d <prefix>]"
    echo ""
    echo "Options:"
    echo "  -h: display help"
    echo "  -a: print all pdf files in current directory"
    echo "  -b: print all lines from stdin that start with a number"
    exit "$1"
}

function print_pdf_files(){
    all_files=$(ls -a)
    grep -E "\.[pP][dD][fF]$" <<< "$all_files" | sort
   
    exit $SUCCESS
}

function print_numbered_lines(){
    sed -n 's/^[+-]\?[0-9]\+//p'
    exit $SUCCESS
}

function split_to_sentences(){
    text_line=$(tr '\n' ' ')

    while [[ $text_line =~ [[:upper:]][^\.?!]*[\.?!] ]]; do
        echo "${BASH_REMATCH[0]}"
        text_line=${text_line#*"${BASH_REMATCH[0]}"}
    done

    exit $SUCCESS
}

function add_include_path(){
    original_IFS=$IFS
    IFS=
    dop="$OPTARG"

    while read -r line; do    
        sed -e 's|\(#[[:space:]]*include[[:space:]]*<\)\(.*>\)|\1'"$dop"'\2|g' -e 's|\(#[[:space:]]*include[[:space:]]*"\)\(.*"\)|\1'"$dop"'\2|g' <<< "$line"
    done

    IFS=$original_IFS
    exit $SUCCESS
}



while getopts ":habcd:" opt; do
    case $opt in
        h)
            print_help "$SUCCESS"
            ;;
        a)
            print_pdf_files
            ;;
        b)
            print_numbered_lines
            ;;
        c)
            split_to_sentences
            ;;
        d)
            add_include_path
            ;;
        \?)
            print_help "$ERR_INVALID_ARG"
            ;;
    esac
done

print_help "$ERR_INVALID_ARG"
