""" Assignment 0

You will implement a simple in-memory boolean search engine over the jokes
from http://web.hawkesnest.net/~jthens/laffytaffy/.

The documents are read from documents.txt.
The queries to be processed are read from queries.txt.

Your search engine will only need to support AND queries. A multi-word query
is assumed to be an AND of the words. E.g., the query "why because" should be
processed as "why AND because."
"""

# Some imports you may want to use.
from collections import defaultdict
from operator import itemgetter
import re


def read_lines(filename):
    """ Read a file to a list of strings. You should not need to modify
    this. """
    return [l.strip() for l in open(filename, 'rt').readlines()]


def tokenize(document):
    """ Convert a string representing one document into a list of
    words. Remove all punctuation and split on whitespace.
    >>> tokenize("Hi there. What's going on?")
    ['hi', 'there', 'what', 's', 'going', 'on']
    """
    return [word.lower() for word in re.split('[\W]', document) if word != ''] 


def create_index(tokens):
    """
    Create an inverted index given a list of document tokens. The index maps
    each unique word to a list of document ids, sorted in increasing order.
    >>> index = create_index([['a', 'b'], ['a', 'c']])
    >>> sorted(index.keys())
    ['a', 'b', 'c']
    >>> index['a']
    [0, 1]
    >>> index['b']
    [0]
    >>> index['c']
    [1]
    """
    index = {}
    i = 0
    for token in tokens:
        for term in token:
            if (term in index) and (i not in index[term]):
                index[term].append(i)
            else:
                index[term] = [i]

        i = i + 1
    return index


def intersect(list1, list2):
    """ Return the intersection of two posting lists. Use the optimize
    algorithm of Figure 1.6 of the MRS text.
    >>> intersect([1, 3, 5], [3, 4, 5, 10])
    [3, 5]
    >>> intersect([1, 2], [3, 4])
    []
    """
    done=False
    listr=[]
    list1Pointer=0
    list2Pointer=0
    while(not done):
        if( (list1Pointer >= len(list1) ) or ( list2Pointer>=len(list2)) ):
            return listr
        if(list1[list1Pointer] == list2[list2Pointer]):
            listr.append(list1[list1Pointer])
            list1Pointer=list1Pointer+1
            list2Pointer=list2Pointer+1
        elif(list1[list1Pointer] > list2[list2Pointer]):
            list2Pointer=list2Pointer+1
        else:
            list1Pointer=list1Pointer+1

def sort_by_num_postings(words, index):
    """
    Sort the words in increasing order of the length of their postings list in
    index.
    >>> sort_by_num_postings(['a', 'b', 'c'], {'a': [0, 1], 'b': [1, 2, 3], 'c': [4]})
    ['c', 'a', 'b']
    """
    listCopy=words
    lengthList = []
    listReturn = []
    for letter in words:
        lengthList.append(len(index[letter]))
    while((len(lengthList))>0):
        minValuepos = lengthList.index(min(lengthList))
        listReturn.append(listCopy[minValuepos])
        del listCopy[minValuepos]
        del lengthList[minValuepos]
    return listReturn

def search(index, query):
    """ Return the document ids for documents matching the query. Assume that query is a single string, possible containing multiple words. The steps are to:
    1. tokenize the query
    2. Sort the query words by the length of their postings list
    3. Intersect the postings list of each word in the query.
    E.g., below we search for documents containing 'a' and 'b':
    >>> search({'a': [0, 1], 'b': [1, 2, 3], 'c': [4]}, 'a b')
    [1]
    """
    i = 0
    listReturn = []
    tq = [word.lower() for word in re.split('[\W]', query) if word != '']
    sortedQuery = sort_by_num_postings(tq, index)
    for token in sortedQuery:
        if (i + 1) < len(sortedQuery):
            listReturn = intersect(index[sortedQuery[i]], index[sortedQuery[i + 1]])       
            i = i + 2
    return listReturn


    

def main():
    """ Main method. You should not modify this. """
    documents = read_lines('documents.txt')
    tokens = [tokenize(d) for d in documents]
    index = create_index(tokens)
    queries = read_lines('queries.txt')
    for query in queries:
        results = search(index, query)
        print '\n\nQUERY:', query, '\nRESULTS:\n', '\n'.join(documents[r] for r in results)


if __name__ == '__main__':
    main()
