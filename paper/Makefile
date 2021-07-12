# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

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
