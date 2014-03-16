(ns traversals.core-spec
  (:require [speclj.core :refer :all]
            [traversals.core :refer :all])
  (:import [traversals.core BNode])
  )

;; # The Tests
;;
;; We are going to use [spelj](https://github.com/slagyr/speclj) for our tests.

(describe "Levelorder"
          (it "should return a list containing the levelorder traversal of a tree."
              (let [t (add (add (add (add (add (add (add nil 6) 3) 8) 2) 5) 7) 9)]
              (should= '(6 3 8 2 5 7 9) (levelorder t)))))


(describe "Frontier"
          (it "should return the frontier of a tree."
              (let [t (add (add (add (add (add (add (add nil 6) 3) 8) 2) 5) 7) 9)]
              (should= '(2 5 7 9) (frontier t)))))

(describe "Preorder"
          (it "should return a list containing the preorder traversal of a tree."
              (let [t (add (add (add (add (add (add (add nil 6) 3) 8) 2) 5) 7) 9)]
              (should= '(6 3 2 5 8 7 9) (preorder t)))))

(describe "Postorder"
          (it "should return a list containing the postorder traversal of a tree."
              (let [t (add (add (add (add (add (add (add nil 6) 3) 8) 2) 5) 7) 9)]
              (should= '(2 5 3 7 9 8 6) (postorder t)))))

(describe "Inorder"
          (it "should return a list containing the inorder traversal of a tree."
              (let [t (add (add (add (add (add (add (add nil 6) 3) 8) 2) 5) 7) 9)]
              (should= '(2 3 5 6 7 8 9) (inorder t)))))

(run-specs)



