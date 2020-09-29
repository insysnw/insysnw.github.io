This presentation is mostly written in Markdown markup language and intended to be build into revealjs site with pandoc.

To build:
```
pandoc -t revealjs --slide-level 2 -s tcp-presentation.md -o tcp-presentation.html
```
This builds html file which is dependant on revealjs sources from the internet.

`-s/--standalone` flag makes resulting html a link reveal.js in `reveal.js` directory next to html file.

`--self-contained` flags resulting presentation contain all of the necessary resources.
