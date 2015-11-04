pdf:
	xelatex processus.tex

notes:
	python ../../tools/with_notes.py processus.tex
	xelatex processus_notes.tex
	rm *_notes.tex

all: pdf notes

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc

edit:
	blackbox_edit_start notes.tex.gpg

done:
	blackbox_edit_end notes.tex.gpg
