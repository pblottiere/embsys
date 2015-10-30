"""
Replace \setbeameroption{hide notes} to \setbeameroption{show notes}
"""

import os
import sys

if len(sys.argv) > 1:
    texfile = sys.argv[1]

    if os.path.isfile(texfile):
        texh = open(texfile, 'r')
        texdata = texh.read()
        texh.close()

        texdata_with_notes = texdata.replace("\setbeameroption{hide notes}",
                                             "\setbeameroption{show notes}")

        texfile_with_data = "%s_notes.tex" % texfile.split(".")[0]

        texh = open(texfile_with_data, 'w')
        texh.write(texdata_with_notes)
        texh.close()
