import glob

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

def ecv(documents):
        """Extracts the vocabulary given the training documents. Additionally this method returns a dictionary composed of keys which are the vocabulary terms and the values of which are dictionaries composed of keys 'spam' and 'ham' whose values are the counts of the keys of the first dictionary (the terms in the vocabulary) for each respective class.
         >>> train_docs = [Document(f) for f in glob.glob("train/*.txt")]
         >>> nb = NaiveBayes()
         >>> nb.ecv(train_docs)
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

def total_occurences_word_in_class(vocab):
    """"The following function adds the total number of occurences of each term belonging for every document of a different class.
    >>> train_docs = [Document(f) for f in glob.glob("train/*.txt")]
    >>> nb = NaiveBayes()
    >>> nb.total_occurences_word_in_class(train_docs)
    1
    """

    spam_occurences = 0
    ham_occurences = 0

    for word in vocab:
        spam_occurences += vocab[word]['spam']
        ham_occurences += vocab[word]['ham']

    return spam_occurences, ham_occurences

def main():
    train_docs = [Document(f) for f in glob.glob("train/*.txt")]
    vocab = ecv(train_docs)
    total_occurences_word_in_class(vocab)

if __name__ == '__main__':
    main()
  
