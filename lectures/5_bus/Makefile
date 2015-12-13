pdf:
	xelatex bus.tex

handout: pdf
	python ../../tools/handout.py bus.tex

all: pdf

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb *handout.tex *.fls
