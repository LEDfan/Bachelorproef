clean:
	rm -rf main.log main.aux main.bbl main.blg main.out main.pdf main.tex.project.vim

all:
	pdflatex main.tex
	bibtex main.aux
	pdflatex main.tex
	pdflatex main.tex

