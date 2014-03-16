from collections import defaultdict
import codecs
import math
import re

class Relevance(object):
    def __init__(self, filename1=None, filename2=None, filename3=None, filename4=None,filename5=None,filename6=None,filename7=None ):
        if filename1 and filename2 and filename3 and filename4 and filename5 and filename6 and filename7:
            self.relevance_judgements = self.readRelevance(filename1)
            self.BM25S1 = self.read_your_relevance(filename2)
            self.BM25S2 = self.read_your_relevance(filename3)
            self.BM25S3 = self.read_your_relevance(filename4)
            self.BM25S4 = self.read_your_relevance(filename5)
            self.RSV = self.read_your_relevance(filename6)
            self.CSV = self.read_your_relevance(filename7)

        

    def Precision(self, returned_docs, relevant_docs):

        precision_index = defaultdict(float)
        true_pos = 0
        false_pos = 0

        for query in relevant_docs: 
            for doc in returned_docs[query]:
                if len(returned_docs[query]) == 0:
                    break
                if doc in relevant_docs[query]:
                    true_pos += 1
                else:
                    false_pos += 1
            if true_pos != 0 or false_pos != 0:
                precision_index[query] =  ( float(true_pos) / (true_pos + false_pos))
                true_pos = 0
                false_pos = 0

        return precision_index

    
    def Recall(self, returned_docs, relevant_docs):

        recall_index = defaultdict(float)
        true_pos = 0
        false_neg = 0

        for query in relevant_docs:
            for doc in returned_docs[query]:
                if doc in relevant_docs[query]:
                    true_pos += 1
            if true_pos != 0 or false_neg != 0:
                false_neg = len(relevant_docs[query]) - true_pos
                recall_index[query] =  ( float(true_pos) / (true_pos + false_neg))
                true_pos = 0
                false_neg = 0
            else:
                recall_index[query] = 0.0
        
        return recall_index


    def MAP(self, returned_docs, relevant_docs):
        map_index = defaultdict(float)

        for query in returned_docs:
            counter = 0 
            for denominator,doc in enumerate(returned_docs[query]): 
                if doc in relevant_docs[query]:
                    counter += 1
                    map_index[query] += float(counter) / (denominator + 1)
            if len(returned_docs[query]) != 0:
                map_index[query] = float(map_index[query] / len(returned_docs[query]) ) #fixed the miscalculation here.

        return map_index

    def F1(self, returned_docs, relevant_docs):
        precision_index = self.Precision(returned_docs, relevant_docs)
        recall_index = self.Recall(returned_docs, relevant_docs)
        f1_index = defaultdict(float)

        for query in precision_index:
            if (precision_index[query] * recall_index[query] != 0):
                f1_index[query] = (float) (2 * precision_index[query] * recall_index[query]) / (precision_index[query] * recall_index[query])
            else:
                f1_index[query] = 0
        return recall_index

    def computeAvgPrecision(self, returned_docs, relevant_docs):
        precision_index = self.Precision(returned_docs, relevant_docs)
        n = len(precision_index)
        precisions = 0.0

        for query in precision_index:
            precisions += precision_index[query]
        
        return precisions/ n
    
    def computeAvgRecall(self, returned_docs, relevant_docs):
        recall_index = self.Recall(returned_docs, relevant_docs)
        n = len(recall_index)
        recalls = 0.0

        for query in recall_index:
            recalls += recall_index[query]
        
        return recalls/ n
    
    def computeAvgF1(self, returned_docs, relevant_docs):
        f1_index = self.F1(returned_docs, relevant_docs)
        n = len(f1_index)
        f1s = 0.0

        for query in f1_index:
            f1s += f1_index[query]
        
        return f1s/ n

    def computeAvgMAP(self, returned_docs, relevant_docs):
        map_index = self.MAP(returned_docs, relevant_docs)
        n = len(map_index)
        maps = 0.0

        for query in map_index:
            maps += map_index[query]

        return maps/n
        
#### READ IN RELEVANCE JUDGMENTS FROM TIME.REL AND RETRIEVED DOCUMENTS ####
    def readRelevance(self,filename):
        relevanceIndex = {}
        for line in codecs.open(filename, 'rt', 'utf-8').readlines():
            relevance = []
            splitLines = line.split(" ")
            BLEGH = 0
            for ct,term in enumerate(splitLines):
                if term == "\n":
                    break
                if ct == 0:
                    BLEGH = term
                    relevanceIndex[int(BLEGH) - 1] = relevance
                else:
                    if term != '':
                        relevanceIndex[int(BLEGH) - 1].append(int(term))
        return relevanceIndex

    def read_your_relevance(self, filename):
        queryRelevanceIndex = {}
        for line in codecs.open(filename, 'rt', 'utf-8').readlines():
            splitLines = line.split("\t")
            for term in splitLines:
                if term == "\n":
                    BLEGH = 0
                    break
                elif len(splitLines) == 1:
                    BLEGH = int(term)
                    queryRelevanceIndex[BLEGH] = []
                    break
                else:
                    queryRelevanceIndex[BLEGH].append(int(term))
                    break
        return queryRelevanceIndex

def main():
    relevance = Relevance('TIME.REL', 'BM25Search1.txt', 'BM25Search2.txt', 'BM25Search3.txt', 'BM25Search4.txt', 'RSVSearch.txt', 'CosineSimilaritySearch.txt')
    print 'BM25 Search 1' + '\n'
    print relevance.computeAvgPrecision(relevance.BM25S1, relevance.relevance_judgements)
    print relevance.computeAvgRecall(relevance.BM25S1, relevance.relevance_judgements)
    print relevance.computeAvgF1(relevance.BM25S1, relevance.relevance_judgements)
    print relevance.computeAvgMAP(relevance.BM25S1, relevance.relevance_judgements)
    print '\n'
    

    print 'BM25 Search 2' + '\n'
    print relevance.computeAvgPrecision(relevance.BM25S2, relevance.relevance_judgements)
    print relevance.computeAvgRecall(relevance.BM25S2, relevance.relevance_judgements)
    print relevance.computeAvgF1(relevance.BM25S2, relevance.relevance_judgements)
    print relevance.computeAvgMAP(relevance.BM25S2, relevance.relevance_judgements)
    print '\n'
    

    print 'BM25 Search 3' + '\n'
    print relevance.computeAvgPrecision(relevance.BM25S3, relevance.relevance_judgements)
    print relevance.computeAvgRecall(relevance.BM25S3, relevance.relevance_judgements)
    print relevance.computeAvgF1(relevance.BM25S3, relevance.relevance_judgements)
    print relevance.computeAvgMAP(relevance.BM25S3, relevance.relevance_judgements)
    print '\n'
    

    print 'BM25 Search 4' + '\n'
    print relevance.computeAvgPrecision(relevance.BM25S4, relevance.relevance_judgements)
    print relevance.computeAvgRecall(relevance.BM25S4, relevance.relevance_judgements)
    print relevance.computeAvgF1(relevance.BM25S4, relevance.relevance_judgements)
    print relevance.computeAvgMAP(relevance.BM25S4, relevance.relevance_judgements)
    print '\n'
    

    print 'RSV Search' + '\n'
    print relevance.computeAvgPrecision(relevance.RSV, relevance.relevance_judgements)
    print relevance.computeAvgRecall(relevance.RSV, relevance.relevance_judgements)
    print relevance.computeAvgF1(relevance.RSV, relevance.relevance_judgements)
    print relevance.computeAvgMAP(relevance.RSV, relevance.relevance_judgements)
    print '\n'
    

    print 'CSV Search' + '\n'
    print relevance.computeAvgPrecision(relevance.CSV, relevance.relevance_judgements)
    print relevance.computeAvgRecall(relevance.CSV, relevance.relevance_judgements)
    print relevance.computeAvgF1(relevance.CSV, relevance.relevance_judgements)
    print relevance.computeAvgMAP(relevance.CSV, relevance.relevance_judgements)
    print '\n'
 

if __name__ == '__main__':
    main()
