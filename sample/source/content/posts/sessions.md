---
title: "Cookie-based Session Management"
date: 2018-10-16T14:40:27-04:00
draft: false
type: post
description: "The Bare Necessities of Authentication and Recording User Status"
---


Web frameworks had held my hand. Which for a beginner was great, but a lot of the fundamental aspects of back-end development had been abstracted from me, and I found annoying discrepancies between my knowledge and know-how. Sessions were one element I had neglected. I had taken them for granted; somehow magically some level of state is maintained between the server and the client.

It wasn't till I started writing my [News Aggregator](https://github.com/dgski/baseketbase-agg) Project in [Racket](https://racket-lang.org) that I realized I don't really know how sessions work. Racket's a great language, but it's not exactly popular and doesn't have any web frameworks. I needed to learn and write my own session manager. So I did.

How exactly does the server know whether a given client is logged in? Turns out it's mainly about **cookies**. Cookies are small collections of data that the server asks the client to store. They are transmitted in the HTTP request headers under the **Cookie** section, and held by the browser on the client's end. 

For sessions, the server generates a session id, tells the client to remember it, the client does so and attaches that sid to following requests, so the server knows whether a request belongs to an authenticated session or not.

To better understand how this works; let's write a simple HTTP request handler. We will use the basic web server provided in python's **http.server** package. 

We will start with three elements:

- address: this is the address and port at which the server will listen for connections
- handler: this is a class which has the necessary methods to handle requests
- server: we will initialize the built-in **HTTPServer** object with our address and handler.

Finally, we will start our server using the **serve_forever()** method:

``` python
from http.server import *

#Setup
address = ('', 8000)
handler = BaseHTTPRequestHandler
server = HTTPServer(address, handler)

#Start
server.serve_forever()
```

We can send a request to our new server using a browser at **localhost:8000**. However, we should still the following error:

``` text
Error response

Error code: 501

Message: Unsupported method ('GET').

Error code explanation: HTTPStatus.NOT_IMPLEMENTED -
Server does not support this operation.
```

Looks like our handler can't handle simple **GET** requests... We need to do something about that. Define a new class at the top of the file. We still use **BaseHTTPRequestHandler** as the parent class, but add the **do_GET()** method:

``` python

class SessionHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.response = 200
        self.send_header('Content-type','text/html')
        content = "Hello"
        self.wfile.write(bytes(content, "utf-8"))

```

Remember to change the handler to our new class:

``` python
handler = SessionHandler
```

## Dispatching

Now if you visit again with your browser, you should now see the raw text, "Hello" displayed. That's a good start, but we need our server to do different things based on different URL paths. Let's add a simple dispatcher and methods to handle the paths:

``` python
class SessionHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        routes = {
            "/login":   self.login,
            "/logout":  self.logout,
            "/":        self.home
        }
        try:
            response = 200
            content = routes[self.path]()
        except:
            response = 404
            content = "Not Found"
            
        self.send_response(response)
        self.send_header('Content-type','text/html')
        self.end_headers()
        self.wfile.write(bytes(content, "utf-8"))
        return
    
    def home(self):
        return "Welcome Home"

    def login(self):
        # todo: perform login
        return "Logged In"

    def logout(self):
        # todo: perform logout
        return "Logged Out"
```

Okay, things are taking shape. Let's run through what was added:

- The **routes** dictionary has paths as keys and page methods as values.
- We use the **try,except** statement to try to access the methods using the path. If it works, we set **response** to 200 (success) and prepare our content. Otherwise, we set **response** to 404 (not found) and set content to **Not Found**.
- The home, login and logout methods return a text string representing themselves.

## Logging In

Now let's implement our login process. After a user logs in, the time spent logged in is referred to as a **session**. For each login and consecutive logout, we will have a unique session id (sid). So obviously, our next immediate step is to generate a new **sid** when the user clicks on login. We will use a dictionary to store our sessions ids. Usually a database fulfills this role, but for our simple purposes, this should be enough. For incredibly basic security import the randint function:

``` python
from random import randint
```

Add the following method to generate the sid:

``` python
def generate_sid(self):
    return "".join(str(randint(1,9)) for _ in range(100))
```
Add a global dictionary to the top of the file:

``` python
sessions = {}
```
Make the following changes to the **login** method:
``` python
def login(self):
    # Password normally be checked here
    sid = self.generate_sid()
    self.cookie = "sid={}".format(sid)
    sessions[sid] = {"username", "useragent","ip address","expiry"}
    return "Logged In"
```

Add the following **send_header** function calls to bottom of the **do_GET** method:
``` python
def do_GET(self): 
    #...
    self.cookie = None # Addition
    self.send_response(response)
    self.send_header('Content-type','text/html')
    if self.cookie:
            self.send_header('Set-Cookie', self.cookie) # Addition
    self.end_headers()
    self.wfile.write(bytes(content, "utf-8"))
    return
```

What is happening should be clear: **login()** is setting the **self.cookie** member variable to a random string of numbers, and **self.send_header('Set-Cookie', self.cookie)** is sending it off as a cookie value. Now if you visit **/login** in your browser, and open up the web inspector, you should see **sid** as a cookie, with the value as whatever you generated. This cookie is held by the browser, and will be forwarded to our server with every request. It provides 'context' between requests.

Also, notice that **login()** adds the sid to the dictionary as a key with a dummy dictionary **{"username", "useragent","ip address","expiry"}** as a value. This is where a complete session manager would store those pieces of information about the user, to later verify the session. Add this on your own.

## Validating users

Now we need to perform the most fundamental aspect of session management: validating a session. Let's say there is some content that only registered, logged in users can see. We don't want our user to have to provide their password with each page request; That would be incredibly cumbersome. We need to check if a request contains a session and display the right thing. We need to extract a header's sid to get that information. Add the following method to our **SessionHandler** class:

``` python
def parse_cookies(self, cookie_list):
    return dict(((c.split("=")) for c in cookie_list.split(";"))) if cookie_list else {}
```

This will nicely extract the cookies string from the request and put it into a dictionary for our convenience. Also alter our **do_GET** **try** block so it looks like this:

``` python
try:
    response = 200
    cookies = self.parse_cookies(self.headers["Cookie"])
    if "sid" in cookies:
        self.user = cookies["sid"] if (cookies["sid"] in sessions) else False
    else:
        self.user = False
    content = routes[self.path]()
```

Our home page should display whether a user is currently logged in:
``` python
def home(self):
        return "Welcome User!" if self.user else "Welcome Stranger!"
```

What is happening here? Take a closer look at the try block. There are a few interesting things happening within:

- As before, we are setting the response to code 200, which for a HTTP request, it means that the page was found
- Secondly, we are parsing the **Cookie** section of the header. Our **parse_cookies** method splits the Cookie string by ";" to seperate the individual cookies, then it puts the cookie ids and their values into a dictionary. If there are no cookies, it responds with an empty dictionary
- Next, our try block checks if "sid" is in the **cookies** dictionary. If it isn't, it sets **self.user** to false. Otherwise, it then checks whether the sid value stored in the **sessions** global.
- From then on, it's business as usual.

We have altered a fundamental behaviour of our micro-program: when we visit the homepage without a generated sid cookie being broadcast by the browser we will see:
``` text
"Welcome Stranger!"
```
Clearly, unless the sid is stored in the **sessions** dictionary, the application instance does not recognize the web user. But if the user logs in using the rudimentary **/login** page, it will display:
``` text
"Welcome User!"
```
Try logging in, and then restarting our server. If you return to the home page, it won't recognize you, even if the browser holds and sends a **sid** cookie. Because the dictionary dies with the process, our server completely forgets about the session. If you visit the **login** page again, a new **sid** is generated, and we get **"Welcome User!"**.

To Recap: the login process generates a new sid, saves the users information(user agent, ip address, etc), and sends back a header asking for the user's browser to remember the new sid. Of course to be useful in most cases,  you would have to validate a login password before generating the sid and continuing, but the fundamental aspects of session management are already present.

## Logging Out

What if the user wants to end their session? Well, we should probably give them a log out page as well. Let's add something to our currently useless logout page:
``` python
    def logout(self):
        if not self.user:
            return "Can't Log Out: No User Logged In"
        self.cookie = "sid="
        del sessions[self.user]
        return "Logged Out"
```

This method will check if a valid user is logged in, set the cookie to empty and delete the session id from the **sessions** dictionary, before returning the **"Logged Out"** text.


Check out what happens now: restart our your server application, open it and validate the following happens in order:

1. On First Visit of the home page **"/"** it should display: "Welcome Stranger!".
2. If you now visit **"/login"**, it should display "Logged In", check your cookies for the "/" domain, you should see an **sid** cookie with the newly generated session.
3. Now if you visit the home page, your should see: "Welcome User!". Congratulations, your session has been validated!
4. Next test if the **"/logout"** page works. Visit it, it should say "Logged Out". Make sure it worked by once again checking your cookies in your browser. The **sid** cookie should be empty.
5. If you visit the home page again, it should display: "Welcome Stranger!". You have successfully logged out.


The complete code should now look like:
``` python
from http.server import *
from random import randint

sessions = {}

class SessionHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        routes = {
            "/login": self.login,
            "/logout": self.logout,
            "/": self.home
        }
        self.cookie = None
        try:
            response = 200
            cookies = self.parse_cookies(self.headers["Cookie"])
            if "sid" in cookies:
                self.user = cookies["sid"] if (cookies["sid"] in sessions) else False
            else:
                self.user = False
            content = routes[self.path]()
        except:
            response = 404
            content = "Not Found"
        
        self.send_response(response)
        self.send_header('Content-type','text/html')

        if self.cookie:
            self.send_header('Set-Cookie', self.cookie)

        self.end_headers()
        self.wfile.write(bytes(content, "utf-8"))
        return
    
    def home(self):
        return "Welcome User!" if self.user else "Welcome Stranger!"

    def login(self):
        # Password would normally be checked here
        sid = self.generate_sid()
        self.cookie = "sid={}".format(sid)
        sessions[sid] = {"username", "useragent","ip address","expiry"}
        return "Logged In"

    def logout(self):
        if not self.user:
            return "Can't Log Out: No User Logged In"
        self.cookie = "sid="
        del sessions[self.user]
        return "Logged Out"

    def generate_sid(self):
        return "".join(str(randint(1,9)) for _ in range(100))

    def parse_cookies(self, cookie_list):
        return dict(((c.split("=")) for c in cookie_list.split(";"))) \
        if cookie_list else {}

address = ('', 8000)
handler = SessionHandler
server = HTTPServer(address, handler)

server.serve_forever()
```

Please read more about cookies, their expiration setting, and elements of session security before deploying anything like this. There are many more standard practices to consider; including encrypting the session id. Stay Safe!