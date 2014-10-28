"""
Assignment 5: K-Means. See the instructions to complete the methods below.
"""

from collections import Counter
import io
import math

import numpy as np


class KMeans(object):

    def __init__(self, k=2):
        """ Initialize a k-means clusterer. Should not have to change this."""
        self.k = k

    def cluster(self, documents, iters=10):
        """
        Cluster a list of unlabeled documents, using iters iterations of k-means.
        Initialize the k mean vectors to be the first k documents provided.
        Each iteration consists of calls to compute_means and compute_clusters.
        After each iteration, print:
        - the number of documents in each cluster
        - the error rate (the total Euclidean distance between each document and its assigned mean vector)
        See Log.txt for expected output.
        """
	#first step: autoinitialize intial means, taking first k documents as means
	#then compute the distance between every document and every mean
	#assign documents to clusters
	#for ir in iters: -compute clusters-distance, assign each doc to closest mean
	#then compute means again.
	#means list of Counter objects,compute distance between mean and counter object [documents]

        self.means = documents[:self.k]
        self.clusters = [] #values clusters 0-9
        self.documents = documents

        for i in range(iters):
            t = []
            for j in range(self.k):
                 t.append([])
            self.clusters = t
            self.compute_clusters(self.documents)
            l = []
            for cluster in self.clusters:
                l.append(len(cluster))
            print str(l)

            self.compute_means()
            error = self.error(self.documents)
            print 'Current error: ' + str(error)
            
    def compute_means(self):
        """ Compute the mean vectors for each cluster (storing the results in an
        instance variable)."""
        nu_means = []
        for cluster in self.clusters:
            c = Counter()
            for doc in cluster:
                c += self.documents[int(doc)] #adding two Counters together
            for key,value in c.iteritems():
                c[key] = float(value) / len(cluster)
            nu_means.append(c)
        self.means = nu_means

    def compute_clusters(self, documents):
        """ Assign each document to a cluster. (Results stored in an instance
        variable). """
        list_of_mean_norms = []
        for mean_doc in self.means:
                mean_norm = 0
                for term_freq in mean_doc.values():
                    mean_norm += term_freq ** 2
                list_of_mean_norms.append(mean_norm)
        
        for doc_id,doc in enumerate(documents):
            distance_list_for_doc = []
            for i,mean_norm in enumerate(list_of_mean_norms):
                distance_list_for_doc.append(self.distance(doc,self.means[i],mean_norm))
            min_value = min(distance_list_for_doc)
            index_of_min = distance_list_for_doc.index(min_value)
            self.clusters[index_of_min].append(doc_id)
 
    def distance(self, doc, mean, mean_norm):
        """ Return the Euclidean distance between a document and a mean vector.
        See here for a more efficient way to compute:
        http://en.wikipedia.org/wiki/Cosine_similarity#Properties"""
        #need to do ||A|| + ||B|| - 2A^TB
        a_norm = 0
        distance = 0
        for term in doc.keys(): #doing -2A^TB
            a_norm += (doc[term] ** 2) #doing ||A||
            distance += (-2*(doc[term] * mean[term]))
        distance += (a_norm + mean_norm)
        return math.sqrt(distance)


    def error(self, documents):
        """ Return the error of the current clustering, defined as the sum of the
        Euclidean distances between each document and its assigned mean vector."""
        #have two variables, means and clusters, take every vector in cluster and take the distance between its mean
        #there's also an error for each cluster.
        list_of_mean_norms = []
        for mean_doc in self.means:
                mean_norm = 0
                for term_freq in mean_doc.values():
                    mean_norm += term_freq ** 2
                list_of_mean_norms.append(mean_norm)
        
        error_list = []        
        for mean_position,cluster in enumerate(self.clusters):
            for document in cluster:
                error_list.append(self.distance(self.documents[document],self.means[mean_position],list_of_mean_norms[mean_position]))
        
        return sum(error_list)

    def print_top_docs(self, n=10):
        """ Print the top n documents from each cluster, sorted by distance to the mean vector of each cluster.
        Since we store each document as a Counter object, just print the keys
        for each Counter (which will be out of order from the original
        document).
        Note: To make the output more interesting, only print documents with more than 3 distinct terms.
        See Log.txt for an example."""

        list_of_mean_norms = []
        for mean_doc in self.means:
                mean_norm = 0
                for term_freq in mean_doc.values():
                    mean_norm += term_freq ** 2
                list_of_mean_norms.append(mean_norm)

        for mean_position,cluster in enumerate(self.clusters):
            distance_list = []
            print 'Cluster' + str(mean_position)
            for document in cluster:
                distance_list.append((document, self.distance(self.documents[document],self.means[mean_position],list_of_mean_norms[mean_position])))
            distance_list.sort(key=lambda x: x[1])
            counter = 0
            for doc in distance_list:
                if len(self.documents[int(doc[0])].keys()) > 3:
                    counter += 1
                    if counter <= 10:
                        for key in self.documents[int(doc[0])].keys():
                            print key,
                        else:
                            print '\n'
                    else:
                        counter = 0
                        break


def prune_terms(docs, min_df=3):
    """ Remove terms that don't occur in at least min_df different
    documents. Return a list of Counters. Omit documents that are empty after
    pruning words.
    >>> prune_terms([{'a': 1, 'b': 10}, {'a': 1}, {'c': 1}], min_df=2)
    [Counter({'a': 1}), Counter({'a': 1})]
    >>> prune_terms([{'a': 2, 'b': 3, 'c': 8}, {'b': 2, 'd': 1}, {'e': 7, 'c': 6}, {'c': 1}], min_df=2)
    [Counter({'c': 8, 'b': 3}), Counter({'b': 2}), Counter({'c': 6}), Counter({'c': 1})]
    """
    c = Counter() 
    for dictionary in docs:
        for term in dictionary.keys():
            c[term] += 1

    delete_list = []
    for key,value in c.iteritems():
        if c[key] < min_df:
            delete_list.append(key)
    
    for shit in delete_list:
        del c[shit]

    list_of_counters = []
    for counter in docs:
       returned_counter = c & Counter(counter)
       for key in returned_counter.keys():
           returned_counter[key] = counter[key]
       if returned_counter:
           list_of_counters.append(returned_counter)

    return list_of_counters

def read_profiles(filename):
    """ Read profiles into a list of Counter objects.
    DO NOT MODIFY"""
    profiles = []
    with io.open(filename, mode='rt', encoding='utf8') as infile:
        for line in infile:
            profiles.append(Counter(line.split()))
    return profiles


def main():
    """ DO NOT MODIFY. """
    profiles = read_profiles('profiles.txt')#was, profiles.txt
    print 'read', len(profiles), 'profiles.'
    profiles = prune_terms(profiles, min_df=2)
    km = KMeans(k=10)
    km.cluster(profiles, iters=20)
    km.print_top_docs()

if __name__ == '__main__':
    main()
