#! /usr/bin/env sh

./makelex-unk.py < word.syms \
  | fstcompile --isymbols=ascii.syms --osymbols=word.syms \
  | fstdeterminize > lex-unk.fst

echo "m a n <space> m a r s <space>" \
  | farcompilestrings --symbols=ascii.syms --far_type=fst --generate_keys=1 > in1.fst

echo "m a n <space> m a r <space>" \
  | farcompilestrings --symbols=ascii.syms --far_type=fst --generate_keys=1 > in2.fst

./fstphirhocompose in1.fst lex-unk.fst \
  | fstproject --project_output \
  | fstrmepsilon > out1.fst

./fstphirhocompose in2.fst lex-unk.fst \
  | fstproject --project_output \
  | fstrmepsilon > out2.fst
