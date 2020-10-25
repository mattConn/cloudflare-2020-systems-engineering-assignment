# makereq
Made for Cloudflare's 2020 Systems Engineering Assignment, makereq is a simple client socket that can make a specified number of requests to a host, display the responses and response metrics.

## Building
Run `make` to compile.

`make check` will send a request to the website I made for Cloudflare's 2020 General Engineering Assignment.

`make links` will request my website's links url.

`make profile` will request my website's links url 4 times.

## Usage
`makereq --url <url> [--profile <number of requests>]`

`makereq --url <url>` will send a request to `<url>`, display the response body and the following metrics :

- Number of requests
- Time of fastest and slowest response
- Mean and median response times
- Response success rate
- Error codes
- Byte size of largest and smallest responses

`makereq --url <url> --profile <number>` will do what `makereq --url <url>` does but `<number>` times.

`makereq --help` will display a simple help message.

## Testing Results

*makereq sending 20 requests to `cloudflare2020-assignment.foobiebletch.net/links`*
![](response_cloudflare2020-assignment.foobiebletch.net_links.png)

*makereq sending 20 requests to `www.gnu.org`*
![](response_cloudflare2020-assignment.foobiebletch.net_links.png)