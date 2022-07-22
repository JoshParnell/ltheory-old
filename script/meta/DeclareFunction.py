from common import *

maxParams = 12;

code = []
code += include('AutoClass')
code += include('Function')
code += include('Type')
code += ['']

code += [
  '#define _DeclareFunction(Name, ReturnType, %s, x, ...) \\'
    % ', '.join(['_%d' % i for i in range(2 * maxParams)]),
  '  MACRO_IDENTITY(DeclareFunction##x(Name, ReturnType, %s))'
    % ', '.join(['_%d' % i for i in range(2 * maxParams)]), '',
  '#define DeclareFunction(Name, ReturnType, ...) \\',
  '  MACRO_IDENTITY(_DeclareFunction(Name, ReturnType, __VA_ARGS__, %s))'
    % ', '.join(['%d' % (2 * maxParams - i) for i in range(2 * maxParams)]), '',

  '#define _DeclareFunctionArgBind(Name, ReturnType, %s, x, ...) \\'
    % ', '.join(['_%d' % i for i in range(2 * maxParams)]),
  '  MACRO_IDENTITY(DeclareFunctionArgBind##x(Name, ReturnType, %s))'
    % ', '.join(['_%d' % i for i in range(2 * maxParams)]), '',
  '#define DeclareFunctionArgBind(Name, ReturnType, ...) \\',
  '  MACRO_IDENTITY(_DeclareFunctionArgBind(Name, ReturnType, __VA_ARGS__, %s))'
    % ', '.join(['%d' % (2 * maxParams - i) for i in range(2 * maxParams)]), '',

  '#define DeclareFunctionNoParams(Name, ReturnType) \\',
  '  MACRO_IDENTITY(DeclareFunction0(Name, ReturnType))',

  '#define DefineFunction(Name) RegisterFunction(Name) Name##_ReturnType Name(Name##_ArgRefs const& args)', ''
]

register = [
  'Function Name##_GetMetadata() {',
  '  static Function fn;',
  '  if (!fn) {',
  '    fn = Function_Create(#Name);',
  '    fn->description = "None";',
  '    fn->call = Name##_Call;',
  '    fn->paramCount = Name##_ParamCount();',
  '    fn->params = nullptr;',
  '    fn->returnType = Type_Get<Name##_ReturnType>();',
  '    Infer_MetaData(fn, &Name##_ExplicitCall);',
  '    for (uint i = 0; i < fn->paramCount; ++i)',
  '      Mutable(fn->params[i]).name = Name##_ParamName(i);',
  '  }',
  '  return fn;',
  '}',
  'static Function Name##_Metadata = Name##_GetMetadata();'
]

code += macro('RegisterFunction', ['Name'], register)

for params in range(0, maxParams + 1):
  body = []

  body += ['inline uint Name##_ParamCount() { return %i; }' % params]

  # Call helper
  code += [
    'template <%s>' %
      ', '.join(['class RT'] + ['class T%i' % i for i in range(params)]),
    'inline void CallAndAssign(void** in, void* out, RT (*fn)(%s)) {' %
      ', '.join(['T%i const&' % i for i in range(params)]),
    '  *(RT*)out = fn(%s);' %
      ', '.join(['*(T%i*)in[%i]' % (i, i) for i in range(params)]),
    '}',
    ''
  ]

  code += [
    'template <%s>' %
      ', '.join(['class T%i' % i for i in range(params)]),
    'inline void CallAndAssign(void** in, void* out, void (*fn)(%s)) {' %
      ', '.join(['T%i const&' % i for i in range(params)]),
    '  fn(%s);' %
      ', '.join(['*(T%i*)in[%i]' % (i, i) for i in range(params)]),
    '}',
    ''
  ]

  # Automatic metadata inference functions
  if params > 0:
    code += [
      'template <class RT, %s>' %
        ', '.join(['class T%i' % i for i in range(params)]),
      'void Infer_MetaData(Function const& type, RT (*fn)(%s)) {' %
        ', '.join(['T%i const&' % i for i in range(params)]),
      '  type->params = new Parameter[%i];' % params
    ]

    code += [
      '  Mutable(type->params[%i]).type = Type_Get<T%i>();' %
        (i, i) for i in range(params)
    ]

    code += [
      '}',
      ''
    ]
  else:
    code += [
      'template <class RT>',
      'void Infer_MetaData(Function const& type, RT (*fn)()) {}',
      ''
    ]

  # ParamType / ReturnType typedefs
  if params > 0:
    body += [
      'inline char const* Name##_ParamName(uint i) {',
      '  char const* const table[] = {%s};' %
        ', '.join(['#N%i' % i for i in range(params)]),
      '  return table[i];',
      '}'
    ]
  else:
    body += ['inline char const* Name##_ParamName(uint i) { return 0; }']

  body += ['typedef ReturnType Name##_ReturnType;']
  body += ['typedef T%d Name##_ParamType%d;' % (i, i) for i in range(params)]
  body += ['']

  if params > 0:
    body += ['struct Name##_ArgRefs {']
    body += ['  T%d const& N%d;' % (i, i) for i in range(params)]
    body += [
      '  Name##_ArgRefs(%s) : %s {}' %
        (', '.join(['T%d const& N%d' % (i, i) for i in range(params)]),
         ', '.join(['N%d(N%d)' % (i, i) for i in range(params)])),
      '};',
      ''
    ]

    body += ['LT_API ReturnType Name(Name##_ArgRefs const&);']

    for n in ('Name', 'Name##_ExplicitCall'):
      body += [
        'inline ReturnType %s(%s) {' %
          (n, ', '.join(['T%d const& N%d' % (i, i) for i in range(params)])),
        '  return Name(Name##_ArgRefs(%s));' %
          ', '.join(['N%d' % i for i in range(params)]),
        '}'
      ]
  else:
    body += ['typedef int Name##_ArgRefs;']
    body += [
      'LT_API ReturnType Name(Name##_ArgRefs const&);',
      'inline ReturnType Name() { return Name(0); }',
      'inline ReturnType Name##_ExplicitCall() { return Name(0); }'
    ]

  # Type-agnostic call
  body += [
    'inline void Name##_Call(void** in, void* out) {',
    '  CallAndAssign(in, out, Name##_ExplicitCall);',
    '}'
  ]

  macroArgs = ['Name', 'ReturnType']
  for i in range(maxParams) if params > 0 else []:
    macroArgs += ('T%d' % i, 'N%d' % i)

  code += macro('DeclareFunction%d' % (2 * params), macroArgs, body)

  # Automatic arg structure creation
  if params > 0:
    bind = ['DeclareFunction%d(Name, ReturnType, %s)' %
      (2 * params, ', '.join(['T%d, N%d' % (i, i) for i in range(maxParams)]))]

    bind += [
      'AutoClass(Name##_Args, %s) ' %
        ', '.join(['T%d, N%d' % (i, i) for i in range(params)]),
      '  Name##_Args() {}',
      '  Name##_Args(Name##_ArgRefs const& args) : %s {}' %
        ', '.join(['N%d(args.N%d)' % (i, i) for i in range(params)]),
      '};'
    ]

    bind += [
      'inline ReturnType Name(Name##_Args const& args) {',
      '  return Name(Name##_ArgRefs(%s));' %
        ', '.join(['args.N%d' % i for i in range(params)]),
      '}'
    ]

    code += macro('DeclareFunctionArgBind%d' % (2 * params), macroArgs, bind)

code = includeGuard(code, 'LTE_DeclareFunction')
code = ['/* DeclareFunction.py ~ Automatically-generated code */', ''] + code

with open('/home/josh/lt/src/liblt/LTE/DeclareFunction.h', 'w') as f:
  f.write('\n'.join(code))
  f.write('\n')
