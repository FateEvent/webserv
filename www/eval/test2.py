import os
import sys

env = os.environ

print("content-type: text/html; charset=UTF-8\r\n\r\n")

print("<b>Your request body:</b></br>")
for line in sys.stdin:
    sys.stdout.write(line + "<br/>")

print("<br/><br/><b>Environement variable:</b></br>")
for key, value in env.items():
	print(key + " : " + value + "</br>")

