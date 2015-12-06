pdf:
	a2x -v README -f pdf --dblatex-opts "-P latex.output.revhistory=0" \
		--dblatex-opts="--param=latex.encoding=utf8"

pdf_fop:
	a2x -v README -f pdf --fop

clean:
	rm -f README.pdf README.xml
