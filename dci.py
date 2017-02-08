#!/usr/bin/env python3

import sys, os, readline

if len(sys.argv) > 1:
    path = sys.argv[1]
else:
    path = os.getcwd()

path = os.path.join(path, 'in')
    
if not os.path.exists(path):
    print("'" + path + "' file not found")
    sys.exit(1)

out = open(path, 'w')
    
while True:
    cmd = input('> ')
    print(cmd)
    out.write(cmd + '\n')
    out.flush()

