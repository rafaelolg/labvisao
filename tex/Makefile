TEX=$(shell egrep -l '^[^%]*\\begin\{document\}' *tex)
PDF=$(TEX:%.tex=%.pdf)

all: Ep1.pdf presentation.pdf

Ep1.pdf:Ep1.tex
	pdflatex $<
	if test -f references.bib; then bibtex $<; pdflatex $<; pdflatex $<; fi 

presentation.pdf:presentation.tex
	latex $<
	if test -f references.bib; then bibtex $<; latex $<; latex $<; fi 
	dvipdf presentation.dvi


clean:
	rm -f *.log *.aux *.blg *.toc *.bm *.dvi

cleanall:clean
	rm -f *.pdf 


.PHONY:clean package
