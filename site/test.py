import os

env = os.environ

print("Content-Type: text/html; charset=UTF-8\r\n\r\n")

for key, value in env.items():
	print(key + " : " + value + "<br/>")
