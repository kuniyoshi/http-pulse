.SUFFIXES: .html .haml
all: index.html wave.html open-al.html problems.html
.haml.html:
	haml -q $< $(basename $<).html
.sass.css:
	sass -t expanded --stop-on-error --update $<

