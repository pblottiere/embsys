"""
Create a handout version of a PDF beamer presentation.
"""

import os
import sys
import subprocess

#-----------------------------------------------------------------
def copy_tex_with_handout_mode(texfile):

    texh = open(texfile, 'r')
    texdata = texh.read()
    texh.close()

    to_replace = "]{beamer}"
    replacement = ", handout]{beamer}"
    texdata = texdata.replace(to_replace, replacement)

    texfile_handout = "%s_handout.tex" % texfile.split(".")[0]

    texh = open(texfile_handout, 'w')
    texh.write(texdata)
    texh.close()

    return texfile_handout

#-----------------------------------------------------------------
if len(sys.argv) == 2:
    tex = sys.argv[1]

    if os.path.isfile(tex):
        texfile = copy_tex_with_handout_mode(tex)

        cmd = "xelatex -recorder %s" % texfile
        proc = subprocess.Popen(cmd, shell=True)
        proc.communicate()
