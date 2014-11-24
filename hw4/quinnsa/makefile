TARGET=quinnsa_proj4_writeup
HTML=main_html
SRC={quinnsa_proj4_writeup}

default: all 

all:
	latex quinnsa_proj4_writeup.tex
	dvips -R -Poutline -t letter quinnsa_proj4_writeup.dvi -o quinnsa_proj4_writeup.ps
	ps2pdf quinnsa_proj4_writeup.ps
	rm quinnsa_proj4_writeup.aux
	rm quinnsa_proj4_writeup.dvi
	rm quinnsa_proj4_writeup.log
	rm quinnsa_proj4_writeup.out
	rm quinnsa_proj4_writeup.ps
