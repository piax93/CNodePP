# CNodePP

Have you ever dreamed of creating your own HTTP server from scratch? Me neither, but I was curious to see how far I could go. What I'm trying to do is to bring the code-style of the Node.js http framework in a C++ environment. The idea is to have a central core handling swappable submodules, compiled as shared objects, which represent the various routes/functionalities of the web service.

#### What has been done so far
* Structure to hot-swap modules.
* 0.01% of the HTTP standard (just enough to answer a GET requests).
* Template parser to insert variables in html files.

#### What needs to be done
* Complete HTTP specifications (at least the most important features).
* Add the functionality to include submodules in templates.
* Add compression library.
* Add SSL compatibility.
* Add more parameters to the settings.
* Make everything a little more robust and write some documentation.

**If you think this is awfully implemented right now but you like the idea, hop on board and join me in this time wasting project.**

## How to run

To compile everything and run:

    make core
    make all_modules
    bin/core


To create a new module:

    ./genmodule.sh <module_name>


To compile a single module:

    make module/<module_name>

