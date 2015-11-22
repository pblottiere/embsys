pdf:
	a2x -v README -f pdf --dblatex-opts "-P latex.output.revhistory=0" \
		--dblatex-opts="--param=latex.encoding=utf8"

clean:
	rm -f README.pdf README.xml

edit:
		blackbox_edit_start corr/ld_preload/Makefile.gpg
		blackbox_edit_start corr/ld_preload/hack.c.gpg
		blackbox_edit_start corr/ld_preload/run.sh.gpg

done:
		blackbox_edit_end corr/ld_preload/Makefile.gpg
		blackbox_edit_end corr/ld_preload/hack.c.gpg
		blackbox_edit_end corr/ld_preload/run.sh.gpg
