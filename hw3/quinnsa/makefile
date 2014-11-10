TARGET=quinnsa_proj3_writeup
HTML=main_html
SRC={quinnsa_proj3_writeup}

default: all 

all:
	latex quinnsa_proj3_writeup.tex
	dvips -R -Poutline -t letter quinnsa_proj3_writeup.dvi -o quinnsa_proj3_writeup.ps
	ps2pdf quinnsa_proj3_writeup.ps
	rm quinnsa_proj3_writeup.aux
	rm quinnsa_proj3_writeup.dvi
	rm quinnsa_proj3_writeup.log
	rm quinnsa_proj3_writeup.out
	rm quinnsa_proj3_writeup.ps
