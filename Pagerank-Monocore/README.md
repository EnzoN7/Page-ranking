## Description

It is a small page ranking project in which you enter a network of web pages and in which a ranking of the pages by decreasing weight emerges.

## To use it
**Compile:** <br/>
* `g++ .\*.cpp -o main`


**Execute:** <br/>
* `.\main -I 100 -D 0.85 -P exemple_sujet.net` *iterations = 100, dumping factor = 0.85, network = exemple_sujet.net, adjcency matrix printed*
* `.\main exemple_sujet` *by default (-I 50 -D 0.5)*
