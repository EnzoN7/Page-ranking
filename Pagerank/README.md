## Description

It is a small page ranking project in which you enter a network of web pages and in which a ranking of the pages by decreasing weight emerges.

## To use it
**Compile:** <br/>
	> g++ .\*.cpp -o main <br/>


**Execute:** <br/>
*100 iterations, 0.85 dumping factor, exemple_sujet.net, adjcency matrix printed* <br/>
	> .\main 100 -I 0.85 -D -P exemple_sujet.net <br/>
*by default (50 -I 0.5 -D)* <br/>
	> .\main exemple_sujet
