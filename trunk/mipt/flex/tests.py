#!/usr/bin/env python3

import subprocess

tests = [
         ".",
         "a *",
         "a * *",
         "1111",
         "a b .",
         "a b +", 
         "aa bb cc + +",
         "a b . c . d . e . f . a +",
         "a b . c . d . e . f .",
         "o . L . o . L . o",
         "abaca###bad@@@abacaba "
        ]

index = 0

for test in tests:
  test = "'" + test + "'"
  cmd = "echo " + test +" | ./task1"
  print("Test " + str(index))
  subprocess.call(["/bin/bash", "-c", "echo " + test])
  subprocess.call(["/bin/bash", "-c", cmd])
  index = index + 1
  print()

