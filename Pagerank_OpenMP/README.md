## Description

It is a small page ranking project in which you enter a network of web pages and in which a ranking of the pages by decreasing weight emerges.

* *Standard version with brainlinks.net* < 19 seconds
* *Parallel programming version with brainlinks.net* < 6 seconds

## To use it
**Compile:** <br/>
* `export OMP_NUM_THREADS=4` *can be change according to your processor*
* `g++ -o main -fopenmp .\*.cpp`


**Execute:** <br/>
* `.\main -I 100 -D 0.85 -P exemple_sujet.net` *iterations = 100, dumping factor = 0.85, network = exemple_sujet.net, adjcency matrix printed*
* `.\main exemple_sujet` *by default (-I 50 -D 0.5)*
