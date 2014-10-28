"""
Assignment 4. Implement a Naive Bayes classifier for spam filtering.

You'll only have to implement 3 methods below:

train: compute the word probabilities and class priors given a list of documents labeled as spam or ham.
classify: compute the predicted class label for a list of documents
evaluate: compute the accuracy of the predicted class labels.

"""

import glob
from collections import defaultdict
from decimal import Decimal
import math


class Document(object):
    """ A Document. DO NOT MODIFY.
    The instance variables are:

    filename....The path of the file for this document.
    label.......The true class label ('spam' or 'ham'), determined by whether the filename contains the string 'spmsg'
    tokens......A list of token strings.
    """

    def __init__(self, filename):
        self.filename = filename
        self.label = 'spam' if 'spmsg' in filename else 'ham'
        self.tokenize()

    def tokenize(self):
        self.tokens = ' '.join(open(self.filename).readlines()).split()


class NaiveBayes(object):

    def __init__(self):
        self.spam_prior = 0.0
        self.ham_prior = 0.0
        self.condprob_terms = {}
        self.vocab = {}
    
    def ecv(self, documents):
        """Extracts the vocabulary given the training documents. Additionally this method returns a dictionary composed of keys which are the vocabulary terms and the values of which are dictionaries composed of keys 'spam' and 'ham' whose values are the counts of the keys of the first dictionary (the terms in the vocabulary) for each respective class.
        # >>> train_docs = [Document(f) for f in glob.glob("train/*.txt")]
        # >>> nb = NaiveBayes()
        # >>> nb.ecv(train_docs)
         {}
        """
        vocab = {}
        for doc in documents:
            if doc.label == 'spam':
                for word in doc.tokens:
                    if word not in vocab:
                        vocab[word] = {'spam': 0, 'ham': 0}
                        vocab[word]['spam'] += 1 
                    else:
                        vocab[word]['spam'] += 1

            else:
                for word in doc.tokens:
                    if word not in vocab:
                        vocab[word] = {'spam': 0, 'ham': 0}
                        vocab[word]['ham'] += 1 
                    else:
                        vocab[word]['ham'] += 1
        return vocab

    def total_occurences_word_in_class(self, vocab):
        """"The following function adds the total number of occurences of each term belonging for every document of a different class.
         train_docs = [Document(f) for f in glob.glob("train/*.txt")]
         nb = NaiveBayes()
         nb.total_occurences_word_in_class(train_docs)
        """
 
        spam_occurences = 0
        ham_occurences = 0

        for word in vocab:
            spam_occurences += vocab[word]['spam']
            ham_occurences += vocab[word]['ham']

        return spam_occurences, ham_occurences

    def count_docs(self, documents):
        return len(documents)

    def count_docs_in_class(self, documents):
        """
         >>> train_docs = [Document(f) for f in glob.glob("train/*.txt")]
         >>> nb = NaiveBayes()
         >>> nb.count_docs_in_class(train_docs)
         (311, 1582)
        """

        spam_count = 0
        ham_count = 0

        for doc in documents:
            if doc.label == 'spam':
                spam_count += 1
            else:
                ham_count += 1

        return (spam_count,ham_count)


    def train(self, documents):
        """
        TODO: COMPLETE THIS METHOD.

        Given a list of labeled Document objects, compute the class priors and
        word conditional probabilities, following Figure 13.2 of your book.
         >>> train_docs = [Document(f) for f in glob.glob("train/*.txt")]
         >>> nb = NaiveBayes()
         >>> nb.train(train_docs)
         {} 
        """
        vocab = self.ecv(documents)
        self.vocab = vocab
        V = len(vocab)
        
        tup = (spam_count, ham_count) = self.count_docs_in_class(documents)
        num_docs = self.count_docs(documents) 
        
        #PRIORS: N_c/ N
        spam_prob = float(tup[0]) / num_docs
        ham_prob = float(tup[1]) / num_docs

        cond_probs_terms = defaultdict()

        spam_occurences, ham_occurences = self.total_occurences_word_in_class(vocab)

        for term in vocab:
            cond_probs_term = {'spam': 0.0, 'ham': 0.0} #different variable from above
            cond_probs_term['spam'] += float(vocab[term]['spam'] + 1)/ (spam_occurences + V)
            cond_probs_term['ham'] += float(vocab[term]['ham'] + 1)/ (ham_occurences + V)

            cond_probs_terms[term] = cond_probs_term
        
        self.spam_prior = spam_prob
        self.ham_prior = ham_prob
        self.condprob_terms = cond_probs_terms

        return spam_prob, ham_prob, cond_probs_terms

    
    def classify(self, documents):
        """
        TODO: COMPLETE THIS METHOD.

        Return a list of strings, either 'spam' or 'ham', for each document.
        documents....A list of Document objects to be classified.
        """
        
        return_list = []
        prior_spam = self.spam_prior
        prior_ham = self.ham_prior
        cond_probs_terms = self.condprob_terms

        #document dictionary with key being doc id and value being dictionary  of scores
        docs = defaultdict()
        
        for doc in documents:
            doc_score = {'spam': 0.0, 'ham': 0.0}
            docs[doc] = doc_score
            docs[doc]['spam'] += math.log10(prior_spam)
            docs[doc]['ham'] += math.log10(prior_ham)
            for token in doc.tokens:
                if token in cond_probs_terms:
                    docs[doc]['spam'] += math.log10(cond_probs_terms[token]['spam'])
                    
                    docs[doc]['ham'] += math.log10(cond_probs_terms[token]['ham'])
            
            if docs[doc]['spam'] > docs[doc]['ham']:
                return_list.append('spam')
            else:
                return_list.append('ham')

        return return_list

def evaluate(predictions, documents):
    """
    TODO: COMPLETE THIS METHOD.

    Evaluate the accuracy of a set of predictions.
    Print the following:
    accuracy=xxx, yyy false spam, zzz missed spam
    where
    xxx = percent of documents classified correctly
    yyy = number of ham documents incorrectly classified as spam
    zzz = number of spam documents incorrectly classified as ham

    See the provided log file for the expected output.

    predictions....list of document labels predicted by a classifier.
    documents......list of Document objects, with known labels.
    """
    doc_labels = []

    num_correct_spam_classified = 0
    num_incorrect_spam_classified = 0

    num_correct_ham_classified = 0
    num_incorrect_ham_classified = 0

    for doc in documents:
        doc_labels.append(doc.label)

    for labels in zip(predictions,doc_labels): #list of tuples (pred_label,doc_label)
        if labels[0] == labels[1]:
            if labels[1] == 'spam':
                num_correct_spam_classified += 1
            else:
                num_correct_ham_classified += 1
        else:
            if labels[1] == 'ham':
                num_incorrect_spam_classified += 1
            else:
                num_incorrect_ham_classified += 1

    xxx = float(num_correct_spam_classified + num_correct_ham_classified) / (num_correct_spam_classified + num_correct_ham_classified + num_incorrect_spam_classified + num_incorrect_ham_classified)

    yyy = num_incorrect_ham_classified
    zzz = num_incorrect_spam_classified

    print "Percent of correctly classified documents: ", xxx
    print "Number of documents incorrecly classified as spam: ", yyy
    print "Number of documents incorrecly classified as ham: ", zzz

def main():
    """ DO NOT MODIFY. """
    train_docs = [Document(f) for f in glob.glob("train/*.txt")]
    print 'read', len(train_docs), 'training documents.'
    nb = NaiveBayes()
    nb.train(train_docs)
    test_docs = [Document(f) for f in glob.glob("test/*.txt")]
    print 'read', len(test_docs), 'testing documents.'
    predictions = nb.classify(test_docs)
    evaluate(predictions, test_docs)

if __name__ == '__main__':
    main()
