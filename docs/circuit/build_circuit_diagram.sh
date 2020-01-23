#!/bin/sh

trap 'rm -f circuit.aux circuit.log circuit.pdf' 0

# create circuit.png
pdflatex -shell-escape circuit
