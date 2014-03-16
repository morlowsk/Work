(ns bst.core-spec
;  (:refer-clojure :exclude [])
  (:require [speclj.core :refer :all]
            [bst.core :refer :all])
;  (:import [bst.core ])
  )

;; # The Tests
;;
;; We are going to use [spelj](https://github.com/slagyr/speclj) for our tests.


(describe "Add"
          (it "should work on empty trees"
              (should= 5 (:key (:root (add (make-tree) 5 "vulvectomy")))))
          (it "should increase the size by the given amount"
              (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "dog") 7 "you" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= 8 (-> t  :size))))
          (it "should have a key at the root node."
              (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "dog") 7 "you" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= 4 (:key(:root t)))))
          (it "should have a value at the root node."
              (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "dog") 7 "you" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= "slamz" (:value (:root t)))))
          (it "should add a bigger element properly"
              (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "dog") 7 "you" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= 22 (:key (:right (:right (:right (:root t))))))))
          (it "should add a smaller element properly"
              (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "dog") 7 "you" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= 0 (:key (:left (:left (:left (:root t))))))))
          )
(describe "Find"
        (it "should find the largest element's value"
         (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "hleb") 7 "d" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= "cephalotripsy" (find t 22))))
        (it "should find the smallest element's value"
         (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "hleb") 7 "d" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= "uargh" (find t 0))))
         )

(describe "find-if-tree"
          (it "should say that it's a tree if it is."
             (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "hleb") 7 "d" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
               (should= true (find-if-tree t) ))))
  
        


(describe "Find-key"
        (it "should find the largest element's key"
         (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "hleb") 7 "d" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= 22 (find-key t "cephalotripsy"))))
        (it "should find the smallest element's key"
         (let [t (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "hleb") 7 "d" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy")]
              (should= 0 (find-key t "uargh"))))
         )
(describe "Delete"
          (it "should work on empty trees"
              (should= nil (:key (delete (make-tree) 5))))
          (it "should decrease the size by the given amount"
              (let [t (delete (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") 22)]
              (should= 7 (-> t :size))))
       ;   (it "should not decrease the size if the key isn't there"
        ;      (let [t (delete (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") 88)]
         ;     (should= 0 (-> t :size))))
          (it "should delete the largest node"
              (let [t (delete (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") 22)]
              (should= nil (:right (:right (:right (:root t)))))))
          (it "ahould delete the smallest node"
              (let [t (delete (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") 0)]
              (should= nil (:left (:left (:left (:root t)))))))
          (it "should copy the predecessor when in the case of two children nodes"
              (let [t (delete (add (add (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy")
                    6 "y") 5 "u") 8)]
              (should= 7 (:key (:right (:root t))))))
          (it "should delete the key of the predecessor when in the case of two children nodes"
              (let [t (delete (add (add (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy")
                    6 "y") 5 "u") 8)]
              (should=  nil (:left (:left (:left (:right (:root t))))))))
          (it "should truncate the subtree"
              (let [t (delete (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") 8)]
              (should= nil (:key (:left (:right (:root t)))))))
          )

(describe "Delete-value"
          (it "should work on empty trees"
              (should= nil (:key (delete-value (make-tree) "cerebral incubation"))))
          (it "should decrease the size by the given amount"
              (let [t (delete-value (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") "cephalotripsy")]
              (should= 7 (-> t :size))))
          (it "delete the largest node"
              (let [t (delete-value (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") "cephalotripsy")]
              (should= nil (:right (:right (:right (:root t)))))))
          (it "delete the smallest node"
              (let [t (delete-value (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") "uargh")]
              (should= nil (:left (:left (:left (:root t)))))))
          )
(describe "Map-tree"
         (it "should return an incremented tree"
         (let [y (map-tree (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "d") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "piss") 22 "cephalotripsy") count)]
              (should= (:value (:root y)) 5)))
          )

(run-specs)





