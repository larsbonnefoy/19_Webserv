#!/usr/bin/env python3

import os
import sys
import urllib.parse

content_length = os.environ.get('CONTENT_LENGTH', 0)

if content_length == "NULL":
    request_body = os.environ.get("QUERY_STRING", 0)
else:
    content_length = int(content_length)
    request_body = sys.stdin.read(content_length - 1)

params = urllib.parse.parse_qs(request_body)
print("Content-Type: text/html")  # HTML is following
print("")                         # Leave a blank line
print("<h1>Addition Results</h1>")
try:
    num1 = int(params["num1"][0])
    num2 = int(params["num2"][0])
except:
    print("<output>Sorry, the script cannot turn your inputs")
    print("into numbers (integers).</output>")
    raise SystemExit(1)
print("<output>{0} + {1} = {2}</output>".format(num1, num2, num1 + num2))
