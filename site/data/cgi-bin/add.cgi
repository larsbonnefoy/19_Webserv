#!/usr/bin/env python3

import cgi
import os
import sys

# print("IN SCRIPT")
# form = cgi.FieldStorage()

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
print(content_length)

#request_body = sys.stdin.read(10)
#print(request_body)
# print(input_data)

print("Content-Type: text/html")  # HTML is following
print("")                         # Leave a blank line
print("<h1>Addition Results</h1>")
try:
    num1 = 1
    num2 = 2
except:
    print("<output>Sorry, the script cannot turn your inputs")
    print("into numbers (integers).</output>")
    raise SystemExit(1)
print("<output>{0} + {1} = {2}</output>".format(num1, num2, num1 + num2))
