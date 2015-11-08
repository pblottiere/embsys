"""
Replace portion of beamer code. Try with 'catchfilebetweentags' but
doesn't seem to work with beamer code such as frame. Moreover, I don't
want recursive includes.
"""

import os
import sys

#=============================================================================
# functions
#=============================================================================
#-----------------------------------------------------------------------------
def search_content(texfile, tag):

    lines_to_keep = []
    tag = tag.replace("\n", "")

    if os.path.isfile(texfile):
        data = open(include_file).readlines()

        begin_tag = "%<**" + tag + ">"
        end_tag = "%<//" + tag + ">"
        to_copy = False
        for line in data:
            if begin_tag in line:
                to_copy = True
            elif end_tag in line:
                to_copy = False

            if to_copy:
                rc = line.find("incnote")
                if int(rc) == -1:
                    lines_to_keep.append(line)

    if len(lines_to_keep) > 0:
        lines_to_keep.pop(0)

    return lines_to_keep

#=============================================================================
# main
#=============================================================================
if len(sys.argv) > 1:
    texfile = sys.argv[1]

    if os.path.isfile(texfile):
        # open input file and extract data
        texh = open(texfile, 'r')
        texlines = texh.readlines()
        texh.close()

        # create output file
        texfile_with_content = "%s_content.tex" % texfile.split(".")[0]
        texh = open(texfile_with_content, 'w')

        # parse data to search commands %include[texfile][tag]
        texdata_with_content = []
        for data in texlines:

            if "%include[" in data:
                cmd = data.split('[')[1:3]
                [include_file, tag] = [arg.replace(']','') for arg in cmd]

                content = search_content(include_file, tag)

                for cont in content:
                    texdata_with_content.append(cont)
            else:
                texdata_with_content.append(data)

        for line in texdata_with_content:
            texh.write(line)

        # close output file
        texh.close()
