pdf:
	xelatex intro.tex

notes:
	python ../../tools/with_notes.py intro.tex
	xelatex intro_notes.tex
	rm *_notes.tex

handout: pdf
	python ../../tools/handout.py intro.tex

all: pdf notes

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb \
		*handout.tex *.fls

edit:
	blackbox_edit_start notes.tex.gpg

done:
	blackbox_edit_end notes.tex.gpg
