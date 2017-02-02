* For dynamic loading:
    - keep a global map of module handlers
    - watcher thread on the compiled objects folder
    - on folder modification, refesh map of handlers
* Use timer to avoid module update duplicates
