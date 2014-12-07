Report.pdf: Report.tex macros.tex
	@pdflatex -halt-on-error -interaction batchmode $<
	@pdflatex -halt-on-error -interaction batchmode $<

clean:
	rm -f *.aux *.out *.log *.toc *.blg *.bbl
