import os, sys

extensions = ('.cpp', '.c', '.h', '.lts')
count_empty_lines = True

def read_line_count(fname):
    return len(open(fname).readlines())

lineCount = 0
fileCount = 0

def walk(path):
  global lineCount
  global fileCount

  for base, dirs, files in os.walk(path):
    for file in files:
      # Check the sub directorys            
      if file.find('.') < 0:
          continue
      ext = (file[file.rindex('.'):]).lower()
      if ext in extensions:
        fileCount += 1
        path = (base + '/'+ file)
        c = read_line_count(path)
        print "%d \t %s \t %s" % (c, path, "" if c < 100 else "." if c < 500 else "?" if c < 1000 else "***")
        lineCount += c

walk("src/")
walk("resource/")

print 'File count : %d' % fileCount
print 'Line count : %d' % lineCount
print 'Avg Line/File : %d' % (lineCount / fileCount)
