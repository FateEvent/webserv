import os
import http.cookies as cookies

cookie_string = os.environ.get('HTTP_COOKIE', '')
cookie = cookies.SimpleCookie(cookie_string)

print('Content-type: text/html')
print("\r\n\r\n")
print('<!DOCTYPE html>')
print('<html>')
print('<head>')
print('<meta charset="utf-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1">')
print('<title>Evaluation page test</title>')
print('<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" crossorigin="anonymous">')
print('<title>See Cookie</title>')
print('</head>')
print('<body>')
print('<h1>Cookies</h1>')
print('<ul>')
for key, val in cookie.items():
    print('<li>{}: {}</li>'.format(key, val.value))
print('</ul>')
print('<form action="/index.html">')
print('<button type="submit" class="btn btn-primary">Return to index page</button>')
print('</form>')
print('</body>')
print('</html>')