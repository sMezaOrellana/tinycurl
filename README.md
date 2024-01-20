# tinycurl

tinycurl is a small re-implementation of the curl command-line tool, focusing solely on supporting HTTP requests. This project is intended for educational purposes to help individuals understand the basics of building a simplified HTTP client. 

## Getting Started


### Installation

make

### Usage

```
Usage: ./tinycurl [options]
Options:
  -p <port>       Specify the port to connect to
  -X <method>     Specify the HTTP method to use
  -H <header>     Specify an HTTP header to include in the request
  -u <uri>        Specify the URI to request
  -help           Display this help message
```

```bash
# Example command
$ tinycurl http://example.com
```