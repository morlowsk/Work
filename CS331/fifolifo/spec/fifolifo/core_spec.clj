(ns fifolifo.core-spec
  (:refer-clojure :exclude [pop peek])
  (:require [speclj.core :refer :all]
            [fifolifo.core :refer :all])
  (:import [fifolifo.core Stack Queue]))

;; # The Tests
;;
;; We are going to use [spelj](https://github.com/slagyr/speclj) for our tests.

(describe "initial testing for debug"
          (it "top test"
              (let [stk (push (push (make-stack) 5) 10)]
                (should= nil (top (pop (pop stk))))))
)


(describe "The stack declaration"

          (it "should create something."
              (should (make-stack)))

          (it "should have empty components."
              (should= (Stack. nil 0) (make-stack)))
          
          (it "should have a size of zero."
              (should= 0 (stack-size (make-stack))))
          )


(describe "The queue declaration"

          (it "should create something."
              (should (make-queue)))

          (it "should have empty components."
              (should= (Queue. nil nil 0) (make-queue)))
          
          (it "should have a size of zero."
              (should= 0 (stack-size (make-stack))))
          )

(describe "stack-size test"

          (it "should return 0 for empty stack."
              (should= 0 (stack-size (Stack. nil 0))))
          (it "should return a number of a stack if it has data"
              (let [stk (Stack. nil 0)]
               (should= 3 (stack-size (push (push (push stk 1) 2) 3)))))
)

(describe "push"
          
          (it "should add the elt in the firt place"
              (let [stk (make-stack)]
               (should= 1 (top (push (push stk 2) 1)))))
          (it "should increase the size by one when add  non nil elt"
              (let [stk (make-stack)]
               (should= 2 (stack-size (push (push stk 2) 1)))))
          (it "sould not change the previous data"
              (let [stk (push (push (push (make-stack) 2) 3) 1)]
                (should= 3 (top (pop stk)))))
 )
          
(describe "pop"
          
          (it "should work with an empty stack"
              (let [stk (make-stack)]
               (should= stk (pop (pop (pop (pop stk)))))))
          (it "should only delete the first one of the stack"
              (let [stk (push (push (push (push (make-stack) 2) 3) 1) 0)]
                (should= (push (push (make-stack) 2) 3) (pop (pop stk)))))
          (it "should decrease the size when pop a stack"
              (let [stk (push (push (push (make-stack) 2) 3) 1)]
                (should= 1 (stack-size (pop (pop stk))))))
)

(describe "top test"
          
          (it "should return nil if the stack is empty"
              (let [stk (make-stack)]
                (should= nil (top stk))))
          (it "should return a right top if there is a stack"
              (let [stk (push (push (push (make-stack) 4) 5) 6)]
                (should= 6 (top stk))))
)

(describe "queue-size test"
          
          (it "should return the right size if the back is not nil"
              (let [queue (enqueue (enqueue (make-queue) 3) 5)]
                (should= 2 (queue-size queue))))
)

(describe "enqueue test"
          
          (it "should add to the back if the elt is not nil"
              (let [queue (make-queue)]
                (should= 5 (peek (enqueue (enqueue queue 5) 3)))))
          (it "should increase the size in the right amount"
              (let [queue (make-queue)]
                (should= 2 (queue-size (enqueue (enqueue queue 5) 3)))))
          (it "should not delete the previous elt"
              (let [queue (enqueue (make-queue) 5)]
                (should= 3 (peek (dequeue (enqueue queue 3))))))
          (it "should not chnge the front if elt is not nil"
              (let [queue (dequeue (enqueue (enqueue (make-queue) 5) 3))]
                (should= 3 (peek (enqueue queue 10)))))
          (it "should do nothing if elt is nil"
              (let [queue (enqueue (make-queue) 5)]
                (should= 5 (peek (enqueue queue nil)))))
	 )

(describe "dequeue test"
          
          (it "should delete the peek value"
              (let [queue (enqueue (enqueue (enqueue (make-queue) 5) 10) 15)]
                (should= 15 (peek (dequeue (dequeue queue))))))
          (it "should decrease the size by one"
              (let [queue (enqueue (enqueue (enqueue (enqueue (enqueue (make-queue) 5) 10) 15) 20) 25)]
                (should= 3 (queue-size (dequeue (dequeue queue))))))
          (it "should not do anything if the queue is empty"
              (let [queue (make-queue)]
                (should= nil (peek (dequeue (dequeue queue))))))
          (it "dequeue an empty queue, the size remain 0"
              (let [queue (make-queue)]
                (should= 0 (queue-size (dequeue (dequeue queue))))))
)          

(describe "peek test"

         (it "should work with empy queue"
             (let [queue (make-queue)]
               (should= nil (peek queue))))
         (it "should return the value that it will be deleted when dequeue is called"
             (let [queue (enqueue (enqueue (enqueue (make-queue) 5) 10) 15)]
               (should= 5 (peek queue))))
         (it "is the first additional test for above"
             (let [queue (enqueue (enqueue (enqueue (make-queue) 5) 10) 15)]
               (should= 10 (peek (dequeue queue)))))
         (it "is the second additional test for above"
             (let [queue (enqueue (enqueue (enqueue (make-queue) 5) 10) 15)]
               (should= 10 (peek (enqueue (dequeue queue) 20)))))
) 
 
(run-specs)
