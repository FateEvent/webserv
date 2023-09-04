Material to start:
* <https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa>;

An history of the internet and how the web works:
* <https://www.freecodecamp.org/news/brief-history-of-the-internet>;
* <https://www.freecodecamp.org/news/how-the-web-works-a-primer-for-newcomers-to-web-development-or-anyone-really-b4584e63585c>;
* <https://www.freecodecamp.org/news/how-the-web-works-part-ii-client-server-model-the-structure-of-a-web-application-735b4b6d76e3>;
* <https://www.freecodecamp.org/news/how-the-web-works-part-iii-http-rest-e61bc50fa0a>.

HTTP response status codes:
* <https://developer.mozilla.org/en-US/docs/Web/HTTP/Status>

For a network application example:
* <https://www.ibm.com/docs/ja/zvm/7.2?topic=interface-network-application-example>

info Pascal:
* <https://github.com/wpf68/webserv/tree/main/___hors_push_42/Doc>
* <https://github.com/cclaude42/webserv>

Une __Guide pour la programmation réseaux__:
* <http://vidalc.chez.com/lf/socket.html#clientserver>.

To understand the difference between TCP and UDP:
* <https://stackoverflow.com/questions/5815675/what-is-sock-dgram-and-sock-stream>

As concerning CGI (Common Gateway Interface):
* <https://web.developpez.com/cgic.htm?>;
* <https://en.wikipedia.org/wiki/Common_Gateway_Interface>;

URL:
* <https://en.wikipedia.org/wiki/URL>

HTTP Headers
* <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers>

NGINX config files
* <https://www.plesk.com/blog/various/nginx-configuration-guide>;
* <https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms>

Multipart and chunked request:
* <https://stackoverflow.com/questions/16958448/what-is-http-multipart-request>;
* <https://learn.microsoft.com/en-us/previous-versions/office/developer/exchange-server-2010/aa493937(v=exchg.140)>;
* <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding>.

CGI:
* <https://perlhacks.com/articles/cgi-programming/cgi-programming-part-1>;
* <https://www.tutorialspoint.com/perl/perl_cgi.htm>;
* <https://www.tutorialspoint.com/python/python_cgi_programming.htm>;

Perl tainted mode:
* <https://docstore.mik.ua/orelly/linux/cgi/ch08_04.htm>;

Difference between PUT and POST:
* <https://stackoverflow.com/questions/630453/what-is-the-difference-between-post-and-put-in-http>;

About Flexbox:
* <https://css-tricks.com/snippets/css/a-guide-to-flexbox>;
* <https://css-tricks.com/designing-a-product-page-layout-with-flexbox>;

For opening Firefox from MacOs terminal:
* <https://superuser.com/questions/277565/start-firefox-from-terminal-on-mac-os-x>;

Testing with CURL:
* <https://reqbin.com/req/c-dot4w5a2/curl-post-file>;
* <https://reqbin.com/req/c-tmyvmbgu/curl-head-request-example>;
* <https://reqbin.com/req/c-tm07oqh4/curl-custom-headers>;
* <https://serverfault.com/questions/140149/difference-between-curl-i-and-curl-x-head>;
* <https://medium.com/@petehouston/upload-files-with-curl-93064dcccc76>;

To launch the server:
- modify, in `config/home.conf`, the IP address (by typing the command `ifconfig`, for example, that displays the current configuration for a network interface) and the root path;
- compile the program files, using the command `make`;
- execute the program with the command `./webserv config/home.conf`.