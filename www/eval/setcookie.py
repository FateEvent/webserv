import os
import http.cookies as cookies

query_string = os.environ.get('QUERY_STRING', '')
key, value = query_string.split('&')[0].split('=')[1], query_string.split('&')[1].split('=')[1]

cookie_header = 'Set-Cookie: {}={};'.format(key, value)

print('Content-type: text/html')
print(cookie_header)
print("\r\n\r\n")
print('<!DOCTYPE html>')
print('<html>')
print('<head>')
print('<meta charset="utf-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1">')
print('<title>Evaluation page test</title>')
print('<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" crossorigin="anonymous">')
print('<title>Set Cookie</title>')
print('</head>')
print('<body>')
print('<main>')
print('<h1>Cookie are set coorectly</h1>')
print('<form action="/index.html">')
print('<button type="submit" class="btn btn-primary">Return to index page</button>')
print('</form>')
print('</main>')
print('</body>')
print('</html>')
