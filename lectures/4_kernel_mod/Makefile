pdf:
	xelatex kernel_mod.tex

handout: pdf
	python ../../tools/handout.py kernel_mod.tex

all: pdf

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb *handout.tex *.fls
