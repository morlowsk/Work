(ns fifolifo.core
  (:refer-clojure :exclude [pop peek])
)

;; # Introduction
;;
;; We are going to implement stacks and double ended queues in
;; using Clojure lists and our own record types.
;;
;; The student will be given the record definition and a few sample
;; functions, and be asked to implement the given stub functions and
;; the tests.

;; # The Stack
;;
;; Stacks are extremely easy to implement using lists.  This parts
;; should be very simple.  We use a record as a wrapper, though.

(defrecord Stack [top size])

(defn make-stack "Create an empty stack." []
  (Stack. nil 0))

(defn stack-size "Return the size of the stack." [stack]
  (:size stack))

(defn push [stack elt] "Add an element to the top of the stack."
  (Stack. (cons elt (:top stack)) (-> stack :size inc)))

(defn pop "Remove an element from the top of the stack. Return the resulting stack." [stack] 
  (cond (empty? (:top stack)) (make-stack)
	:else (Stack. (rest (:top stack)) (-> stack :size dec))))

(defn top "Return the top of the stack." [stack]
  (first (:top stack)))


;; # Doubly-ended queues
;;
;; Queues are a little bit more tricky to implement with persistent lists.
;; The trick is to provide two lists, one representing the front, and one
;; representing the back.  You enqueue elements by placing them in the back
;; list, and dequeue elements by taking them from the front list.
;;
;; If the front list is empty, you reverse the back list and use that as the
;; front list.

(defrecord Queue [back front size])

(defn make-queue "Create an empty queue." []
  (Queue. nil nil 0))

(defn queue-size [queue]
  (:size queue))

(defn enqueue "Add an element to the back of a queue." [queue elt]
  (Queue. (cons elt (:back queue)) (:front queue) (-> queue :size inc)))

(defn rev [x y] (if (empty? x) y
  (rev (rest x) (cons (first x) y))))

(defn dequeue [queue] "Reverse if front of queue is empty and take away from front if full." 
   		(cond (empty? (:front queue))
		(cond (empty? (:back queue)) queue
		:else (Queue. nil (rest (rev (:back queue) nil)) (-> queue :size dec)))
                :else (Queue. (:back queue) (rest (:front queue)) (-> queue :size dec))))

			


(defn peek  "Peek would return the item that dequeue would get rid of." [queue]
		(cond (empty? (:front queue)) (first (rev (:back queue) nil))
	:else (first (:front queue)))) 
 
 

