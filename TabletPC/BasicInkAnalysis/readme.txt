Basic Ink Analysis
------------------

This sample is an updated version of the Ink Divider Sample, using the InkAnalysis API.  The InkAnalysis API combines the RecognizerContext and Ink Divder into one API and expands on the functionality of both.

When the form is updated, the sample draws either a rotated or non-rotated rectangle around each analyzed unit: words, lines, paragraphs, writing regions, drawings and bullets. Besides using different colors, these rectangles are enlarged by different amounts to ensure that none of the rectangles is obscured by others. 

The following table specifies the color and enlargement for each analyzed unit.

Analyzed Unit  	Color  	Pixel Enlargement  
.........................................
Word		Green	1
Line		Magenta	3
Paragraph	Blue	5
Writing Region	Yellow	7
Drawing		Red	1
Bullet		Orange	1

This sample also demonstrates Background (asynchronous) Analysis.