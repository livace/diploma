#!/Users/livace/miniforge3/bin/python

from fileinput import filename
import glob
import graphviz
import sys
import os

if len(sys.argv) == 1:
    names = glob.glob('**/*.dot')
else:
    names = sys.argv[1:]

for name in names:
    dest = name[:-4]

    print(name, dest)
    with open(name) as f:
        graph = graphviz.Source(f.read(), format='png')
        graph.render(filename=dest)
        os.remove(dest)
