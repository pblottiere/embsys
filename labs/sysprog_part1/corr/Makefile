pdf:
	a2x -v README_CORR -f pdf --dblatex-opts "-P latex.output.revhistory=0" \
		--dblatex-opts="--param=latex.encoding=utf8"

pdf_fop:
	a2x -v README_CORR -f pdf --fop

clean:
	rm -f README_CORR.pdf README_CORR.xml

edit:
	blackbox_edit_start README_CORR.gpg

done:
	blackbox_edit_end README_CORR.gpg
