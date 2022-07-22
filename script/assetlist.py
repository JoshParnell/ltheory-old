import os
from subprocess import *

sourceDir = '.'
extensions = ('txt', 'ttf', 'jsl', 'wav', 'ogg', 'jpg', 'png', 'ico')

files = []
for dirname, dirnames, filenames in os.walk(sourceDir):
    for filename in filenames:
        if filename.split('.')[-1].lower() in extensions:
            files.append((os.path.join(dirname, filename)).replace('\\', '/')[2:])

open('assetlist.txt', 'w').write('\n'.join(files))
