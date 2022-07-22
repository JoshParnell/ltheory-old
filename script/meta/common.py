def indent(block):
  return ['  ' + l for l in block]

def include(name):
  return ['#include "%s.h"' % name]

def includeGuard(code, name):
  result  = ['#ifndef %s_h__' % name, '#define %s_h__' % name, '']
  result += code
  result += ['', '#endif']
  return result

def macro(name, args, body):
  definition = '#define %s' % name
  if len(args) > 0:
    definition += '('
    for i, a in enumerate(args):
      if i > 0:
        definition += ', '
      definition += a
    definition += ')'
  result = [definition]
  result += indent(body);
  return [l + (' ' * (79 - len(l))) + '\\' for l in result] + ['']
