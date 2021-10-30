# Simple Markov Chain Sentence Generator written in Python

> What is it?

A Markov chain is a chain where the next phase of it can't be predicted using the former phases. This is what is used in this little program to create sentences starting from a word.

A text is passed to it and used as a base to choose the words from. Starting from the word given it will choose between the different words that are located right after in the text.

> How does it work? 

It is written in the form of a Python class `MarkovChain` with four methods `find_next`, `_find_ender`, `make_sentence` and `make_text` (`_find_ender` is used to find words that are at the end of sentences to have more sensible results)

If you run the `MarkovChain.py` script directly a simple CLI interface is offered to you; run `python MarkovChain.py --help` to get information on how to use it:

```
usage: MarkovChain.py [-h] [-t TYPE] [-w WORD] [-l LENGTH] filepath

positional arguments:
  filepath              Text file used as dictionnary

optional arguments:
  -h, --help            show this help message and exit
  -t TYPE, --type TYPE  Choose whether to print a sentence or a text.
  -w WORD, --word WORD  The first word of the sentence/text (takes a random
                        word if not set)
  -l LENGTH, --length LENGTH
                        Length of the sentence in words/text in sentences
                        (default:10)
```
