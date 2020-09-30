SOURCES = $(wildcard *.tex *.bib *.sty)
TARGETS = $(patsubst %.tex, %.pdf, $(filter %.tex,${SOURCES}))

${TARGETS} : %.pdf : ${SOURCES}
	@pdflatex ${*}
	@-bibtex  ${*}
	@pdflatex ${*}
	@-bibtex  ${*}
	@pdflatex ${*}

all      : ${TARGETS}

clean    :
	@rm -f *.aux *.bbl *.bcf *.blg *.log *.out *.pdf *.toc *.xml

spotless : clean
	@rm -f ${TARGETS}
