# esp-index-I  
esp-index-I is a C++ implementation of improved ESP-index[1].  
ESP-index[2] is a grammar based self-index for large scale and highly repetitive texts.  
Using rank/select dictionaries for a bit array[3] and large alphabets (GMR)[4],
imporved ESP-index improves query search time of ESP-index.
Especially, long query search time of improved ESP-index is faster than other grammar based self indexes.  
esp-index-I uses [RSDic](https://code.google.com/p/rsdic/) as a rank/select dictionary for a bit array
and a self implementation as GMR.

# Quick Start
    $cd esp-index-I/src  
    $make

To build an index from an input text, please use the below command.  
    
    $./build-index -i input_file -o output_file  
    
To search frequency of queries, please use the below command.

    $./search -i index_file -q query_file -m count

To search location of queries, please use the below command.
  
    $./search -i index_file -q query_file -m locate

An example of query_file are esp-index-I/query/query.  
To extract substrings of arbitrary position, please use the below command.

    $./extract -i index_file -p position_file

An example of position_file are esp-index-I/query/position.  
To decompress an index to an input text, please use the below command.

    $./decompress -i index_file -o output_text_file

# Options
```
usage: ./build-index --input_file=string --output_file=string [options] ... 
options:
  -i, --input_file                 input file name (string)
  -o, --output_file                index file name (string)
  -e, --extraction_length_ratio    store extracation length of nodes per ESP tree level e (unsigned int [=2])
  -m, --mode                       1: read whole input string on working memory and build ESP tree
                                   2: build ESP tree while reading input characters(variables) w by w mega length (unsigned int [=1])
  -w, --input_space                input space is limited by w mega length in mode 2 (unsigned long long [=10])
  -?, --help                       print this message
```

```
usage: ./search --index_file=string --query_file=string --mode=string [options] ... 
options:
  -i, --index_file    index file name (string)
  -q, --query_file    query file name (string)
  -m, --mode          mode "count" or "locate" (string)
  -?, --help          print this message
```

```
usage: ./extract --index_file=string --position_file=string [options] ... 
options:
  -i, --index_file       index file name (string)
  -p, --position_file    position file name (string)
  -?, --help             print this message
```

```
usage: ./decompress --input_file=string --output_file=string [options] ... 
options:
  -i, --input_file     index file name (string)
  -o, --output_file    output file name (string)
  -?, --help           print this message
```

# Refferences
[1] Yoshimasa Takabatake, Yasuo Tabei, Hiroshi Sakamoto: Improved ESP-index: A  
Practical Self-index for Highly Repetitive Texts. SEA 2014: 338-350

[2] Shirou Maruyama, Masaya Nakahara, Naoya Kishiue, Hiroshi Sakamoto: ESP-index:  
A compressed index based on edit-sensitive parsing. J. Discrete Algorithms 18:  
100-112, 2013

[3]  Gonzalo Navarro, Eliana Providel: Fast, Small, Simple Rank/Select on Bitmaps.  
SEA 2012: 295-306

[4] Alexander Golynski, J. Ian Munro, S. Srinivasa Rao: Rank/select operations on  
large alphabets: a tool for text indexing. SODA 2006: 368-373  
