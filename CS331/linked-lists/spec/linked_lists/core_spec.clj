(ns linked_lists.core-spec
  (:require [speclj.core :refer :all]
            [linked_lists.core :refer :all])
  (:import [linked_lists.core Cons]))

;; # The Tests
;;
;; We are going to use [spelj](https://github.com/slagyr/speclj) for our tests.


(describe "The record declaration"
          (it "should create something"
              (should (Cons. 10 20)))

          (it "should have a car"
              (should= 10 (:car (Cons. 10 20))))

          (it "should have a cdr"
              (should= 20 (:cdr (Cons. 10 20))))

          (it "should be chainable"
              (should= 40 (-> (Cons. 10 (Cons. 20 (Cons. 30 40))) :cdr :cdr :cdr))))

(describe "insert-at-beginning"
          (it "creates a cons cell"
              (should-not= nil (insert-at-beginning 10 nil)))

          (it "should work with empty lists"
              (should= (Cons. 10 nil) (insert-at-beginning 10 nil) ))
          
          (it "should work with lists that have data"
              (let [xx (Cons. 10 (Cons. 20 (Cons. 30 nil)))]
                (should= (Cons. 5 xx) (insert-at-beginning 5 xx) ))))

(describe "insert-at-end"
	  (it "creates a cons cell."
		(should-not= nil (insert-at-end 10 nil)))
          (it "handles empty lists."
              (should= (Cons. 5 nil) (insert-at-end 5 nil)))
  	  (it "handles full lists."
              (should= (Cons. 2 (Cons. 5 nil)) (insert-at-end 5 (Cons. 2 nil)))))


(describe "sorted-insert"
	  (it "should create a cons cell."
	      (should= (Cons. 10 nil) (sorted-insert 10 nil)))
          (it "Should insert an element in the middle if the average."
              (should= (Cons. 2 (Cons. 4 (Cons. 5 (Cons. 6 (Cons. 8 nil))))) (sorted-insert 5 (Cons. 2 (Cons. 4 (Cons. 6 (Cons. 8 nil)))))))
          (it "Should work with empty lists."
	      (should= (Cons. 1 nil) (sorted-insert 1 nil )))
	  (it "Should insert an element in the beginning if it's the smallest."
	      (should= (Cons. 1 (Cons. 2 (Cons. 3 nil))) (sorted-insert 1 (Cons. 2 (Cons. 3 nil)))))
	  (it "Should put an element in the end if it's the biggest element."
	      (should= (Cons. 1 (Cons. 2 (Cons. 3 nil))) (sorted-insert 3 (Cons. 1 (Cons. 2 nil)))))
	  (it "Should recycle memory."
	     (let [xx (Cons. 10  (Cons. 20 (Cons. 30 (Cons. 44 (Cons. 55 nil)))))]
		(should= true (identical? (-> xx :cdr :cdr) (-> (sorted-insert 25 xx) :cdr :cdr :cdr))))))

	  

(describe "search"
	  (it "should work with empty lists."
	   (should= false (search 10 nil ))
          (it "should find an element within the list."
	   (should= true (search 2 (Cons. 2 (Cons. 3 nil)))))
   	  (it "should return nil if not found."
	   (should= false (search 4 (Cons. 2 (Cons. 3 nil)))))))


(describe "delete"
          (it "should return an empty list if the original were empty."
	      (let [xx nil]
	       (should= xx (delete 25 xx))))
          (it "should delete the item in an unsorted list."
               (should= (Cons. 5 (Cons. 3 nil)) (delete 2 (Cons. 5 (Cons. 2 (Cons. 3 nil))))))
       ;  (it "should return the original list if the element wasn't found."
	;     (let [xx (Cons. 10  (Cons. 20 (Cons. 30 (Cons. 44 (Cons. 55 nil)))))]
         ;      (should= true (identical? xx (delete 22 xx)))))
	  (it "should delete one copy of the element if it's in the beginning of the list."
	       (should= (Cons. 2 (Cons. 5 nil)) (delete 1 (Cons. 1 (Cons. 2 (Cons. 5 nil))))))
          (it "should delete one copy of the element if it's in the middle of the list."
	       (should= (Cons. 2 (Cons. 5 nil)) (delete 3 (Cons. 2 (Cons. 3 (Cons. 5 nil))))))
 	  (it "should delete one copy of the element if it's in the end of the list."
	       (should= (Cons. 2 (Cons. 5 nil)) (delete 10 (Cons. 2 (Cons. 5 (Cons. 10 nil)))))))
 

(describe "delete-all"
          (it "should delete every copy of the item in the list."
              (should= (Cons. 5 nil) (delete-all 2 (Cons. 2 (Cons. 2 (Cons. 5 nil))))))
	  (it "should return an empty list if the original list were empty."
            (let [xx nil]
	      (should= xx (delete-all 2 xx)))))


(describe "efficient-delete"
          (it "should return a copy of the element in the list."
	   (should=  (Cons. 5 nil) (efficient-delete 2 (Cons. 2 (Cons. 5 nil))))
          (it "should return a copy of the list if the element is not found."
	   (should=  (Cons. 5 nil) (efficient-delete 2 (Cons. 5 nil)))))
          (it "should return an empty list if the original were empty."
	   (should= nil (efficient-delete 5 nil )))
          (it "Should recycle memory."
	     (let [xx (Cons. 10  (Cons. 20 (Cons. 30 (Cons. 44 (Cons. 55 nil)))))]
		(should= true (identical? xx (efficient-delete 25 xx))))))

  

(run-specs)
