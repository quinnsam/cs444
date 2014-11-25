TARGET=chaul_proj4_writeup
HTML=main_html
SRC={chaul_proj4_writeup}

default: all 

all:
	latex chaul_proj4_writeup.tex
	dvips -R -Poutline -t letter chaul_proj4_writeup.dvi -o chaul_proj4_writeup.ps
	ps2pdf chaul_proj4_writeup.ps
	rm chaul_proj4_writeup.aux
	rm chaul_proj4_writeup.dvi
	rm chaul_proj4_writeup.log
	rm chaul_proj4_writeup.out
	rm chaul_proj4_writeup.ps
