from collections import defaultdict
from decimal import Decimal
import codecs
import math
import re

class Index(object):
    def __init__(self, filename=None, filename1=None):
        if filename:  # filename may be None for testing purposes.
            self.documents = self.read_lines(filename)
            self.queries = self.read_queries(filename1)
            self.doc_freqs = self.count_doc_frequencies(self.documents)
            self.tf_index = self.create_tf_index(self.documents)
            self.tfidf_index = self.create_tfidf_index(self.tf_index, self.doc_freqs) #SHOULD USE THIS ONLY IN Q_TO_V
            self.doc_lengths = self.compute_doc_lengths(self.tfidf_index)


    def count_doc_frequencies(self, docs):
        """ Return a dict mapping terms to document frequency.
        >>> res = Index().count_doc_frequencies([['a', 'b', 'a'], ['a', 'b', 'c'], ['a']])
        >>> res['a']
        3
        >>> res['b']
        2
        >>> res['c']
        1
        """
        frequencyIndex = {}
        doc_id = 0
        for doc in docs:
             for term in doc:
                 if term not in frequencyIndex:
                     frequencyIndex[term] = [doc_id]
                 else:
                     for id in frequencyIndex[term]:
                         if doc_id == id:
                             break
                     else:
                         frequencyIndex[term].append(doc_id)
             doc_id+=1

        for term in frequencyIndex:
            occurences = len(frequencyIndex[term])
            frequencyIndex[term] = occurences

        return frequencyIndex

    def create_tf_index(self, docs):

        tfIndex = {}
        totalNumDocs = len(docs)
        
        doc_id = 0
        #gets the occurences of term in  document
        for doc in docs:
            for term in doc:
                if term not in tfIndex:
                    l = [doc_id]
                    l.append(1)
                    tfIndex[term] = [l]
                else:
                    if doc_id == (len(tfIndex[term]) - 1): 
                        tfIndex[term][doc_id].append(1)
                    else:
                        l = [doc_id]
                        l.append(1)
                        tfIndex[term].append(l)
            doc_id+=1
        
        for term in tfIndex:
            pairList = []
            i = 0
            for pair in tfIndex[term]:
                tf = sum(pair[:0:-1])
                p = [pair[0], tf]
                pairList.append(p)
            tfIndex[term] = pairList

        return tfIndex


    def create_tfidf_index(self, tf_index ,doc_freqs):
        """
        >>> index = Index().create_tfidf_index([['a', 'b', 'a'], ['a']], {'a': 2., 'b': 1., 'c': 1.})
        >>> sorted(index.keys())
        ['a', 'b']
        >>> index['a']
        [[0, 0.0], [1, 0.0]]
        >>> index['b']  # doctest:+ELLIPSIS
        [[0, 0.301...]]
        """
        totalNumDocs = len(self.documents)
        tfidfIndex = tf_index
    
        for term in tfidfIndex:
            d_ft = 0
            rightTerm = 0
            leftTerm = 0
            pairList = []
            for pair in tfidfIndex[term]:
                d_ft = doc_freqs[term]
                rightTerm = math.log10(totalNumDocs / d_ft)
                leftTerm = (1 + math.log10(pair[1]))
                w_td = leftTerm*rightTerm
                p = [pair[0], w_td]
                pairList.append(p)
            tfidfIndex[term] = pairList

        return tfidfIndex


    def BM25(self, k, b, query, docs, doc_freqs):
        
        N = len(docs) 
        avgDocLength = self.computeAvgDocLength(docs)

        tf_Index = self.tf_index

        bm25 = defaultdict(int)
        for word in query:
            for index,doc in enumerate(docs): #index will keep track of the
                if word in doc:               #doc-id
                    for tfdoc in tf_Index[word]: #I have to loop again... :/
                        if tfdoc[0] == index:
                            if (k*((1 - b) + b*(len(doc) / avgDocLength)) + tfdoc[1]) == 0:
                                break
                            else:
                                bm25[index+1] += ( math.log10((N*1.0) / doc_freqs[word]) * ((k + 1)*(tfdoc[1])) / (k*((1 - b) + b*(len(doc) / avgDocLength)) + tfdoc[1]))

        return sorted(bm25.items(), key = lambda x: x[1], reverse = True)


    def RSV(self, query, docs, doc_freqs):
        N = len(docs)
        rsv = defaultdict(int)
        for word in query:
            for index,doc in enumerate(docs):
                if word in doc:
                    rsv[index+1] += ((N*1.0) / doc_freqs[word])
        return sorted(rsv.items(), key = lambda x: x[1], reverse = True)

    def computeAvgDocLength(self, docs):
        totalLengths = 0
        n = len(docs)
        for doc in docs:
           totalLengths += len(doc)
        return totalLengths/n

    def compute_doc_lengths(self, index):
        length = {}
        for key in index:
            for docs in index[key]: 
                if docs[0] in length:
                    length[docs[0]].append(docs[1])
                else:
                    length[docs[0]] = [docs[1]]
                    
        for key in length:
            nuList = []
            for weights in length[key]:
                nuList.append(weights**2)
            length[key] = nuList

        for key in length:
            blegh = math.sqrt(sum(length[key]))
            #output = round(blegh, 2)
            length[key] = blegh#output 
        return length
 

    def search_by_cosine(self, query_vector, index, doc_lengths):
        """        
        >>> Index().search_by_cosine({'a': 1}, {'a': [[0, 1], [1, 2]]}, {0: 1, 1: 1})
        [(1, 2), (0, 1)]
        """
        numerator = defaultdict(lambda: 0)
        for query, query_weight in query_vector.items():#really term in query
            if query not in index:
                break
            for doc_id, doc_weight in index[query]:
                numerator[int(doc_id+1)]+= query_weight*doc_weight

        for doc_id in numerator:
            if (int(doc_id) + 1) in numerator:
                numerator[int(doc_id)+1] /=  doc_lengths[doc_id]
        return sorted(numerator.items(), key = lambda x: x[1], reverse = True)
    

    def query_to_vector(self, query):

        index = {}
        N = len(self.documents)
        
        for word in set(query): #calculating the tf-idf values for each word in the query
            if word not in self.doc_freqs:
                break
            else:
                d_ft = self.doc_freqs[word]
                index[word] = math.log10(N/d_ft)*(1+math.log10(query.count(word)))           #index[word]
        return index


    def searchCSV(self, queries):
        #SEARCH BY COSINE SIMILARITY
        queryVectors = self.query_to_vector(queries)
        results = self.search_by_cosine(queryVectors, self.tfidf_index, self.doc_lengths)
        return results

    
    def tokenize(self, doc):
        return [t.lower() for t in re.findall(r"\w+(?:[-']\w+)*", doc)]

    def read_lines(self, filename):
        documents = []
        doc = ""
        for line in codecs.open(filename, 'rt', 'utf-8').readlines():
            splitLines = line.split(" ")
            for term in splitLines:
                if term == "*TEXT":
                    tokenizedDoc = self.tokenize(doc)
                    if len(tokenizedDoc) == 0:
                        break
                    else:
                        documents.append(tokenizedDoc)
                        doc = ""
                        break
                elif term == "\n":
                    break
                else:
                    doc += line
                    break
        return documents

    def read_queries(self, filename1):
        queries = []
        query = ""
        for line in codecs.open(filename1, 'rt', 'utf-8').readlines():
            splitLines = line.split(" ")
            for term in splitLines:
                if term == "*FIND" or term == "*STOP\n":
                    tokenizedDoc = self.tokenize(query)
                    if len(tokenizedDoc) == 0:
                        break
                    else:
                        queries.append(tokenizedDoc)
                        query = ""
                        break
                elif term == "\n":
                    break
                else:
                    query += line
                    break
        return queries

def main():
    index = Index('TIME.ALL', 'TIME.QUE') 

    queryResults = open("CosineSimilaritySearch.txt", "w")
    for qnum,query in enumerate(index.queries):
         idx = str(qnum)
         #q = str(query)
         #string = idx + ' QUERY= ' + q
         queryResults.write(idx)
         queryResults.write("\n")
         queryResults.write('\n'.join(['%d\t%e' % (doc_id, score) for doc_id, score in index.searchCSV(query)[:20]]))
         queryResults.write("\n\n")
    queryResults.close()
    
    queryResults = open("BM25Search1.txt", "w")
    for qnum1,query1 in enumerate(index.queries):
        idx = str(qnum1)
        #q = str(query1)
        #string = idx + ' QUERY= ' + q
        queryResults.write(idx)
        queryResults.write("\n")
        queryResults.write('\n'.join(['%d\t%e' % (doc_id, score) for doc_id, score in index.BM25(1,.5, query1, index.documents, index.doc_freqs)[:20]]))
        queryResults.write("\n\n")
    queryResults.close()

    queryResults = open("BM25Search2.txt", "w")
    for qnum2,query2 in enumerate(index.queries):
        idx = str(qnum2)
        #q = str(query2)
        #string = idx + ' QUERY= ' + q
        queryResults.write(idx)
        queryResults.write("\n")
        queryResults.write('\n'.join(['%d\t%e' % (doc_id, score) for doc_id, score in index.BM25(1,1, query2, index.documents, index.doc_freqs)[:20]]))
        queryResults.write("\n\n")
    queryResults.close()
 
    queryResults = open("BM25Search3.txt", "w")
    for qnum3,query3 in enumerate(index.queries):
        idx = str(qnum3)
        #q = str(query3)
        #string = idx + ' QUERY= ' + q
        queryResults.write(idx)
        queryResults.write("\n")
        queryResults.write('\n'.join(['%d\t%e' % (doc_id, score) for doc_id, score in index.BM25(2,.5, query3, index.documents, index.doc_freqs)[:20]]))
        queryResults.write("\n\n")
    queryResults.close()
  
    queryResults = open("BM25Search4.txt", "w")
    for qnum4,query4 in enumerate(index.queries):
        idx = str(qnum4)
        #q = str(query4)
        #string = idx + ' QUERY= ' + q
        queryResults.write(idx)
        queryResults.write("\n")
        queryResults.write('\n'.join(['%d\t%e' % (doc_id, score) for doc_id, score in index.BM25(2,1, query4, index.documents, index.doc_freqs)[:20]]))
        queryResults.write("\n\n")
    queryResults.close()
 
    queryResults = open("RSVSearch.txt", "w")
    for qnum5,query5 in enumerate(index.queries):
        idx = str(qnum5)
        #q = str(query5)
        #string = idx + ' QUERY= ' + q
        queryResults.write(idx)
        queryResults.write("\n")
        queryResults.write('\n'.join(['%d\t%e' % (doc_id, score) for doc_id, score in index.RSV(query5, index.documents, index.doc_freqs)[:20]]))
        queryResults.write("\n\n")
    queryResults.close()

if __name__ == '__main__':
    main()
