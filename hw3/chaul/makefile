TARGET=chaul_proj3_writeup
HTML=main_html
SRC={chaul_proj3_writeup}

default: all 

all:
	latex chaul_proj3_writeup.tex
	dvips -R -Poutline -t letter chaul_proj3_writeup.dvi -o chaul_proj3_writeup.ps
	ps2pdf chaul_proj3_writeup.ps
	rm chaul_proj3_writeup.aux
	rm chaul_proj3_writeup.dvi
	rm chaul_proj3_writeup.log
	rm chaul_proj3_writeup.out
	rm chaul_proj3_writeup.ps
