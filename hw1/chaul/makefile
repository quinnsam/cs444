TARGET=chaul_hw1_writeup
HTML=main_html
SRC={chaul_hw1_writeup}

default: all 

all:
	latex chaul_hw1_writeup.tex
	dvips -R -Poutline -t letter chaul_hw1_writeup.dvi -o chaul_hw1_writeup.ps
	ps2pdf chaul_hw1_writeup.ps
	rm chaul_hw1_writeup.aux
	rm chaul_hw1_writeup.dvi
	rm chaul_hw1_writeup.log
	rm chaul_hw1_writeup.out
	rm chaul_hw1_writeup.ps
