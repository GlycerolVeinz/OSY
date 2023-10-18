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
    pdf_files=$(find . -type f -iname "*.pdf")
    original_ifs=$IFS

    IFS=$'\n'
    for pdf in $pdf_files; do
        echo "$pdf"
    done

    IFS=$original_ifs
    exit $SUCCESS
}


function print_numbered_lines(){
    sed -n 's/^[+-]\?[0-9]\+//p'
    exit $SUCCESS
}


function find_sentences_intext(){
    original_ifs=$IFS

    IFS=$'.!?'
   
    IFS=$original_ifs
    exit $SUCCESS
}


# options
while getopts ":habcd" opt; do
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
            find_sentences_intext
            ;;
        d)
            exit $SUCCESS
            ;;
        \?)
            print_help "$ERR_INVALID_ARG"
            ;;
    esac
done

print_help "$ERR_INVALID_ARG"
