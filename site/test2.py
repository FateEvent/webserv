import os
import sys

env = os.environ

print("content-type: text/html; charset=UTF-8\r\n\r\n")

for line in sys.stdin:
    sys.stdout.write(line)
	
for key, value in env.items():
	print("<b>" + key + " : " + value + "<b/><br/>")

