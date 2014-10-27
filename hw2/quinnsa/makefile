TARGET=quinnsa_hw1_writeup
HTML=main_html
SRC={quinnsa_hw1_writeup}

default: all 

all:
	latex quinnsa_hw1_writeup.tex
	dvips -R -Poutline -t letter quinnsa_hw1_writeup.dvi -o quinnsa_hw1_writeup.ps
	ps2pdf quinnsa_hw1_writeup.ps
	rm quinnsa_hw1_writeup.aux
	rm quinnsa_hw1_writeup.dvi
	rm quinnsa_hw1_writeup.log
	rm quinnsa_hw1_writeup.out
	rm quinnsa_hw1_writeup.ps
