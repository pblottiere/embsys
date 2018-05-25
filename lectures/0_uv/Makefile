pdf:
	xelatex note.tex

handout: pdf
	python ../../tools/handout.py note.tex

all: pdf

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb \
		*handout.tex *.fls
