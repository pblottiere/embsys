pdf:
	python ../../tools/with_content.py full.tex
	xelatex full_content.tex
	mv full_content.pdf full.pdf

all: pdf

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb
	rm -f full_content.tex
