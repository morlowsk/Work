(ns dlist-atom.core-spec
;  (:refer-clojure :exclude [])
  (:require [speclj.core :refer :all]
            [dlist-atom.core :refer :all])
;  (:import [dlist-atom.core ])
  )

;; # The Tests
;;
;; We are going to use [spelj](https://github.com/slagyr/speclj) for our tests.

(describe "insert-front"
 (it "should work with empty dlists."
     (let [xx (dlist)]
         (do (insert-front xx 5)
      (should= 5 (-> xx d-sentinel d-next d-data)))))
 (it "should work with lists with data in them."
     (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
      (should= 7 (-> xx d-sentinel d-next d-data)))))
 (it "should increase size by the amount I insert"
    (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
      (should= 2 (-> xx d-size)))))
 (it "should make the previous of the node point to first element inserted"
   (let [xx (dlist)]
	(do (insert-front xx 5)
	    (insert-front xx 6)
	    (insert-front xx 7)
       (should= 5 (-> xx d-sentinel d-prev d-data)))))
(it "multiple times will convert to a list properly."
   (let [xx (dlist)]
	(insert-front xx 30)
	(insert-front xx 20)
        (insert-front xx 10)
    (should= (show-dlist xx) '(10 20 30))))

(it "in show-dlist-reverse do the opposite."
   (let [xx (dlist)]
	(insert-front xx 30)
	(insert-front xx 20)
        (insert-front xx 10)
    (should= (show-dlist-reverse xx) '(30 20 10))))

)

(describe "insert-last"
 (it "should work with empty dlists."
    (let [xx (dlist)]
	(do (insert-last xx 6)
    (should= 6 (-> xx d-sentinel d-prev d-data)))))
 (it "should work with lists with data in them."
    (let [xx (dlist)]
	 (do (insert-last xx 6)
             (insert-last xx 7)
	     (insert-last xx 8)
       (should= 8 (-> xx d-sentinel d-prev d-data)))))
 (it "should increase the size by the amount I insert."
    (let [xx (dlist)]
	 (do (insert-last xx 6)
             (insert-last xx 7)
	     (insert-last xx 8)
       (should= 3 (-> xx d-size)))))
 (it "should make the next of the node point to first element inserted"
   (let [xx (dlist)]
	(do (insert-last xx 5)
	    (insert-last xx 6)
	    (insert-last xx 7)
       (should= 5 (-> xx d-sentinel d-next d-data)))))

(it "multiple times will convert to a list properly."
   (let [xx (dlist)]
	(insert-last xx 30)
	(insert-last xx 20)
        (insert-last xx 10)
    (should= (show-dlist xx) '(30 20 10))))

(it "in show-dlist-reverse do the opposite."
   (let [xx (dlist)]
	(insert-last xx 30)
	(insert-last xx 20)
        (insert-last xx 10)
    (should= (show-dlist-reverse xx) '(10 20 30))))


)

(describe "insert-sorted"
 (it "should put it first if first elt is bigger than your elt."
    (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
	    (insert-sorted xx 9)
      (should= 9 (-> xx d-sentinel d-next d-data)))))

(it "should work with empty dlists."
    (let [xx (dlist)]
     (do (insert-sorted xx 5)
      (should= 5 (-> xx d-sentinel d-next d-data)))))

(it "should have a back link."
    (let [xx (dlist)]
     (do (insert-sorted xx 5)
      (should= 5 (-> xx d-sentinel d-prev d-data)))))

 (it "should increase the size by one in a list with data."
    (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
	    (insert-sorted xx 9)
      (should= 6 (-> xx d-size)))))

(it "should increase the size by one in empty case."
    (let [xx (dlist)]
	(do (insert-sorted xx 5)
        (should= 1 (-> xx d-size)))))

(it "multiple times will convert to a list properly."
   (let [xx (dlist)]
	(insert-front xx 30)
	(insert-front xx 20)
        (insert-front xx 10)
        (insert-sorted xx 5)
        (insert-sorted xx 15)
    (should= (show-dlist xx) '(5 10 15 20 30))))

(it "should work with show-dlist-reverse."
   (let [xx (dlist)]
	(insert-front xx 30)
	(insert-front xx 20)
        (insert-front xx 10)
        (insert-sorted xx 5)
        (insert-sorted xx 15)
    (should= (show-dlist-reverse xx) '(30 20 15 10 5))))


);; look at later

(describe "index forward"
  (it "should return 0 at the first position"
     (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
        (should= 0 (index-forward xx 10)))))

(it "should return nil if there's nothing in the list"
     (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
        (should= nil (index-forward xx 89)))))


 (it "should return (n-1) at the nth position"
     (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
        (should= 4 (index-forward xx 5)))))

)

(describe "index-backward"
  (it "should return -1 at the last position (back)"
     (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
        (should= -1 (index-backward xx 5)))))


 (it "should return -n at the nth position (front)"
     (let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
        (should= -5 (index-backward xx 10)))))

)

(describe "list-to-dlist"
   (it "should have a sentinel."
	(let [xx '(10 8 7 6 5)
	      yy (list-to-dlist xx)]
      (should= 'sentinel (-> yy d-sentinel d-data))))
   (it "should have a size."
	(let [xx '(10 8 7 6 5)
	      yy (list-to-dlist xx)]
      (should= 5 (-> yy d-size))))

 ;   (it "should not mess up pointers."
;	(let [xx '(10 8 7 6 5)
;	      yy (list-to-dlist xx)]
 ;     (should= 10 (-> yy d-sentinel d-next))))

(it "will convert to a list properly."
   (let [xx '(1 2 3 4 5 6)
         yy (list-to-dlist xx)]
    (should= (show-dlist yy) '(1 2 3 4 5 6))))


)

(describe "delete"
 (it "should delete if the element is in the middle"
	(let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
	    (delete xx 7)
       (should= 6 (-> xx d-sentinel d-next d-next d-next d-data)))))

(it "should not assume that the list is sorted."
	(let [xx (dlist)]
	(do (insert-front xx 34)
            (insert-front xx 53)
	    (insert-front xx 7)
	    (insert-front xx 89)
	    (insert-front xx 10)
	    (delete xx 7)
       (should= 53 (-> xx d-sentinel d-next d-next d-next d-data)))))

(it "should delete if the element is in the middle"
	(let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
	    (delete xx 7)
       (should= 8 (-> xx d-sentinel d-next d-next d-next d-prev d-data)))))

(it "should decrement the size"
	(let [xx (dlist)]
	(do (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 10)
	    (delete xx 7)
       (should= 4 (-> xx d-size)))))

(it "multiple times will convert to a list properly."
   (let [xx (dlist)]
	(insert-front xx 30)
	(insert-front xx 20)
        (insert-front xx 10)
        (insert-front xx 5)
        (delete xx 20)
    (should= (show-dlist xx) '(5 10 30))))

(it "multiple times will convert to a list properly."
   (let [xx (dlist)]
	(insert-front xx 30)
	(insert-front xx 20)
        (insert-front xx 10)
        (insert-front xx 5)
        (delete xx 20)
    (should= (show-dlist-reverse xx) '(30 10 5))))

)

(describe "reverse"
 (it "should reverse the list in its entirety."
     (let [xx (dlist)]
       (do  (insert-front xx 5)
            (insert-front xx 6)
	    (insert-front xx 7)
	    (insert-front xx 8)
	    (insert-front xx 9)
	    (insert-front xx 10)
            (reverse xx)
       (should= '(5 6 7 8 9 10) (show-dlist xx) ))))

)





(run-specs)

