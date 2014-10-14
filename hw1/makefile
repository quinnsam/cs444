for i in quinnsa_hw1_writeup group_22_writeup ; do \
	TARGET=$$i 
	echo $$TARGET 
	all 
done


#TARGET=quinnsa_hw1_writeup
HTML=main_html
SRC={$TARGET}

default: all

all: pdf  

dvi: ${TARGET}.tex
	latex ${TARGET}.tex
	latex $(TARGET).tex

ps: dvi
	dvips -R -Poutline -t letter ${TARGET}.dvi -o ${TARGET}.ps

pdf: ps
	ps2pdf ${TARGET}.ps
	rm ${TARGET}.aux
	rm ${TARGET}.dvi
	rm ${TARGET}.log
	rm ${TARGET}.out
	rm ${TARGET}.ps
