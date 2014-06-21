#! /usr/bin/env python

import sys

eps = '<epsilon>'
spa = '<space>'
phi = '<phi>'
rho = '<rho>'
unk = '<unk>'
cost = 1

def make_lexicon(ifile):
    s = 2
    for line in ifile:
        word, label = line.split()
        if int(label) <= 1:
            continue  # epsilon
        for i, letter in enumerate(word):
            if i == 0:
                print 0, s, letter, eps
            else:
                print s, s + 1, letter, eps
                s = s + 1
            print s, 1, phi, eps, cost  # unknown tokens will match
        print s, 1, phi, eps, cost      # non-consuming phi symbols
        print s, 0, spa, word           # whitespace required (even at the end)
        s = s + 1
    print 0, 1, phi, eps, cost          # first letter does not match any word
    print 1, 1, rho, eps                # consume rest of the unknown token
    print 1, 0, spa, unk                # whitespace required (even at the end)
    print 0

if __name__ == '__main__':
    make_lexicon(sys.stdin)
