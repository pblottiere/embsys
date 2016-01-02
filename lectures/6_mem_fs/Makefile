pdf:
	xelatex mem_fs.tex

handout: pdf
	python ../../tools/handout.py mem_fs.tex

all: pdf

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb *handout.tex *.fls
