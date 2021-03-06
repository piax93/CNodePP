# CNodePP

Have you ever dreamed of creating your own HTTP server from scratch? Me neither, but I was curious to see how far I could go. What I'm trying to do is bringing the code-style of the Node.js http framework in a C++ environment. The idea is to have a central core handling swappable submodules, compiled as shared objects, which represent the various routes/functionalities of the web service. It is compatible with Linux only, I don't see any good reason to run a server on another OS.

#### What has been done so far
* Structure to hot-swap modules.
* 0.01% of the HTTP standard (just enough to answer GET and POST requests).
* Template parsing to insert submodules and variables in html files.
* Worker threads are "preforked" to limit the maximum number of concurrent requests.
* Interface to GET static resources.

#### What would be cool to get done
* Complete HTTP specifications (at least the most important features).
* Add compression library.
* Add SSL compatibility.
* Add easy database connection interface.
* Make it more configurable by adding more parameters to the settings.
* Make everything a little more robust and write some documentation and tests.

**If you think this is awfully implemented right now but you like the idea, hop on board and join me in this time wasting project.**

## How to run

To compile everything and run:

```bash
make core
make all_modules
bin/core
```

To create a new module:

```bash
./genmodule.sh <module_name>
```

To compile a single module:

```bash
make module/<module_name>
```
