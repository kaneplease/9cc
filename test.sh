#!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

#try 0 0
#try 42 42
try 21 "5+20-4;"
try 41 " 12 + 34 - 5; "
try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"
try 10 "-10+20;"
try 9 "-3*-3;"
try 1 "1+1 == 2;"
try 0 "1+2 == 1;"
try 1 "5 >= 2*2;"
try 0 "5 < 2+3;"
try 2 "a = 2;"
try 3 "x = 3; x;"
try 5 "a = 3;b = 3 * 3 - 5;a + b / 2;"
try 6 "foo = 1; bar = 2+3; foo + bar;"
try 6 "A_a = 2; A_b = (1+3)*2; A_b - A_a;"
try 3 "a1 = 1; b2 = 2; a1+b2;"
try 14 "a = 3; b = 5*6 - 8; return a+b/2;"
try 1 "a = 1; bb = 0; return a; return bb;"
echo OK