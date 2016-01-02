pdf:
	xelatex compil_tools.tex

handout: pdf
	python ../../tools/handout.py compil_tools.tex

all: pdf

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb *handout.tex *.fls
