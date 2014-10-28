""" Assignment 6: PageRank. """
from collections import defaultdict
import glob
from bs4 import BeautifulSoup
from collections import defaultdict


def parse(folder, inlinks, outlinks):
    """
    Read all .html files in the specified folder. Populate the two
    dictionaries inlinks and outlinks. inlinks maps a url to its set of
    backlinks. outlinks maps a url to its set of forward links.
    """
    files = glob.glob("%s/*.html" % folder)
    
    for html_file in files:
        soup = BeautifulSoup(open(html_file))
        links = set(a['href'] for a in soup.find_all('a'))
        file_name = html_file.split("/")
        outlinks[file_name[1]] = links
        for link in links:
            inlinks[link].add(file_name[1])

def compute_pagerank(urls, inlinks, outlinks, b=.85, iters=20):
    """ Return a dictionary mapping each url to its PageRank.
    The formula is R(u) = 1-b + b * (sum_{w in B_u} R(w) / (|F_w|)

    Initialize all scores to 1.0
    """
    url_dict_scores = defaultdict(float)
    for url in urls: #initialization
        url_dict_scores[url] = 1.0

    for i in range(iters):
        for url in url_dict_scores:
            sum = 0
            for w in inlinks[url]:
                sum += (url_dict_scores[w] / len(outlinks[w]))
            url_dict_scores[url] = (1-b) + b*sum
    
    return url_dict_scores

def run(folder, b):
    """ Do not modify this function. """
    inlinks = defaultdict(lambda: set())
    outlinks = defaultdict(lambda: set())
    parse(folder, inlinks, outlinks)
    urls = sorted(set(inlinks) | set(outlinks))
    ranks = compute_pagerank(urls, inlinks, outlinks, b=b)
    print 'Result for', folder, '\n', '\n'.join('%s\t%.3f' % (url, ranks[url]) for url in sorted(ranks))


def main():
    """ Do not modify this function. """
    run('set1', b=.5)
    run('set2', b=.85)


if __name__ == '__main__':
    main()
