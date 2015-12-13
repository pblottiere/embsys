pdf:
	xelatex ipc_net.tex

notes:
	python ../../tools/with_notes.py ipc_net.tex
	xelatex ipc_net_notes.tex
	rm *_notes.tex

handout: pdf
	python ../../tools/handout.py ipc_net.tex

all: pdf notes

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb *handout.tex *.fls

edit:
	blackbox_edit_start notes.tex.gpg

done:
	blackbox_edit_end notes.tex.gpg
