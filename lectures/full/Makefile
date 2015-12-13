pdf: copy-code
	python ../../tools/with_content.py full.tex
	xelatex -recorder full_content.tex
	mv full_content.pdf full.pdf

handout: copy-code pdf
	python ../../tools/with_content.py full.tex
	python ../../tools/handout.py full_content.tex

all: pdf

copy-code:
	rm -rf code
	mkdir code
	cd .. && find */code -name *.c | xargs cp -t full/code/

clean:
	rm -rf code
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb *.fls
	rm -f full_content.tex full_content_handout.tex
