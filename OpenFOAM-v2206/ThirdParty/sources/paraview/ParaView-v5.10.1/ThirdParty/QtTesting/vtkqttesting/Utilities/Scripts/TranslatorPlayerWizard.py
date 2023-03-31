#!/usr/bin/env python

import sys
import os
import fnmatch


def findSource(dir):
  fileList = []
  for root, subFolders, files in os.walk(dir):
      for file in files:
          if fnmatch.fnmatch(file, "*.h") or \
              fnmatch.fnmatch(file, "*.cxx") or \
              fnmatch.fnmatch(file, "*.cpp") or \
              fnmatch.fnmatch(file, "CMakeLists.txt") or \
              fnmatch.fnmatch(file, "*.ui") or \
              fnmatch.fnmatch(file, "*.qrc") or \
              fnmatch.fnmatch(file, "*.py") or \
              fnmatch.fnmatch(file, "*.png"):
            file = os.path.join(root,file)
            file = file[len(dir):] # strip common dir 
            fileList.append(file)
  return fileList

def copyAndReplace(inFile, template, target, key, moduleName):
  newFile = os.path.join( target, inFile.replace(key, moduleName) )
  #newFile = os.path.join(target + "EventTranslatorPlayerTest1.cxx")
  print "creating %s" % newFile
  path = os.path.dirname(newFile)
  if not os.path.exists(path):
      os.makedirs(path)
  
  fp = open(os.path.join(template,inFile))
  contents = fp.read()
  fp.close()
  contents = contents.replace(key, moduleName)
  contents = contents.replace(key.upper(), moduleName.upper())
  fp = open(newFile, "w")
  fp.write(contents)
  fp.close()
  

def usage():
  print ""
  print "Usage:"
  print "ModuleWizard [--template <dir>] [--templateKey <key>] [--target <dir>] <moduleName>"
  print "  --template default ../TemplateTranslatorPlayer/"
  print "  --templateKey default is Template"
  print "  --target default ../<moduleName>/"

def main(argv):

  template = ""
  templateKey = ""
  target = ""
  moduleName = ""

  while argv != []:
    arg = argv.pop(0)
    if arg == "--template":
      template = argv.pop(0)
      continue
    if arg == "--templateKey":
      templateKey = argv.pop(0)
      continue
    if arg == "--target":
      target = argv.pop(0)
      continue
    if arg == "--help":
      usage()
      exit()
    moduleName = arg

  if moduleName == "":
    print "Please specify module name"
    usage()
    exit()

  if template == "":
    template = "../TemplateTranslatorPlayer/"
  if template[-1] != '/':
    template += '/'

  if templateKey == "":
    templateKey = "Template"
    #templateKey = os.path.split(template[:-1])[-1]
  
  if target == "":
    target = "../" + moduleName 
    
  if os.path.exists(target):
    print target, "exists - delete it first"
    exit()

  if not os.path.exists(template):
    print template, "does not exist - run from source dir or specify with --template"
    usage()
    exit()

  print "\nWill copy \n\t%s \nto \n\t%s \nreplacing \"%s\" with \"%s\"\n" % (template, target, templateKey, moduleName)
  sources = findSource( template )
  print sources

  for file in sources:
    copyAndReplace(file, template, target, templateKey, moduleName)
  

  print '\nModule %s created!' % moduleName

if __name__ == "__main__":
  main(sys.argv[1:])
