pdf:
	xelatex uv.tex

handout: pdf
	python ../../tools/handout.py uv.tex

all: pdf

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb \
		*handout.tex *.fls
