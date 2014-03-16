
(ns bst.core)

;; # Introduction
;;
;; In this lab you get to write a BST like the one we did in class, only
;; this time it is a dictionary structure and not a set.
;; As such, the "data" element from before will have a key and value instead.

(defrecord BST [root size])
(defrecord BNode [left key value right])

(defn make-node
  ([key value]  (make-node nil key value nil))
  ([left key value right] (BNode. left key value right))
  )

(defn make-tree []
  (BST. nil 0))

;; # Size
;;
;; A warmup function.

(defn size "Return the size of the tree." [t]
(-> t :size))

;; # Add
;;
;; The nodes will be entered into the tree on the basis of their key.
;; If someone tries to add a key that is already there, we replace the value
;; with the new entry.

(declare add-aux)

(defn add [bst nu-key nu-val]
(BST. (add-aux (:root bst) nu-key nu-val) (-> bst :size inc)))

(defn add-aux [node nu-key nu-val]
(cond (nil? node) (make-node nu-key nu-val)
(= nu-key (:key node))
(make-node (:left node) nu-key nu-val (:right node))
(pos? (compare nu-key (:key node)))
(make-node
(:left node) (:key node) (:value node) (add-aux (:right node) nu-key nu-val))
:else
(make-node
(add-aux (:left node) nu-key nu-val) (:key node) (:value node) (:right node))
))


;; FIND THE VALUE GIVEN THE KEY
;; O (lg n) time

(declare find-value-aux)

(defn find [bst key]
   (find-value-aux (:root bst) key))

(defn find-value-aux [node key]
(cond (nil? node) nil
	(= key (:key node)) (:value node)
	(pos? (compare key (:key node)))
		(find-value-aux (:right node) key)
	:else
		(find-value-aux (:left node) key)
))

(declare find-if-tree-aux)

(defn find-if-tree [bst]
  (find-if-tree-aux (:root bst)))

(defn find-if-tree-aux [node]
  (cond (nil? node) true
        ((> (:key (:right node)) (:key node))) (find-if-tree-aux (:right node))
        :else 
            false
        ((< (:key (:left node)) (:key node))) (find-if-tree-aux (:left node))
        :else
            false))
 

;; FIND THE KEY GIVEN THE VALUE
;; O (n) time

(declare find-key-aux)

(defn find-key [bst value]
   (find-key-aux (:root bst) value))


(defn find-key-aux [node value]
  (cond (nil? node) nil
	(= value (:value node)) (:key node)
:else
  (let [found (find-key-aux (:left node) value)]
	  (cond (nil? found)
     (find-key-aux (:right node) value)
     :else found))))



;; FUNCTION TO DELETE THE NODE GIVEN THE KEY O (lg n)
(comment
(defn go-right [t]
 (if (nil? (:right t)) t
    (go-right t)))
)

(defn go-right [t]
      (if (nil? (:right t)) t
          (go-right (:right t))))

(defn get-pred [t]
 (go-right (:left t)))

(declare delete-aux)

(defn delete [bst victim]
  (cond (nil? (find bst victim)) (make-tree)
   :else
  (BST. (delete-aux (:root bst) victim) (-> bst :size dec)))

)

(defn delete-aux [node look-key]
  (cond (nil? node) nil
    (= look-key (:key node))
	(cond (and (nil? (:left node)) (nil? (:right node))) nil
	      (or (nil? (:left node)) (nil? (:right node)))
                 (or (:left node) (:right node))
        :else (let [pred (get-pred node)]
		     (make-node (delete-aux (:left node) (:key pred)) (:key pred) (:value pred) (:right node))))
	(neg? (compare look-key (:key node)))
		(make-node (delete-aux (:left node) look-key)  (:key node) (:value node) (:right node))
	(pos? (compare look-key (:key node)))
    (make-node (:left node) (:key node) (:value node) (delete-aux (:right node) look-key)
)))

;; FUNCTION TO DELETE THE NODE GIVEN THE VALUE
;; O(n) time

(comment
(defn show' [t]
   (if (nil? t)
     "x"
       (str "(" (show' (:left t)) " " (:value t)
           " " (show' (:right t)) ")")))
(defn show [t]
   (if (nil? t)
     "x"
       (str "(" (show (:left t)) " " (:key t)
           " " (show (:right t)) ")")))

(def t (delete (add (add (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") 6 "y") 5 "u") 8))

(def t2 (add (add (add (add (add (add (add (add (add (add (make-tree) 4 "slamz") 8 "3") 7 "yeah" )3 "bleh") 2 "gree") 0 "uargh") 16 "d") 22 "cephalotripsy") 6 "y") 5 "u") )

(show (:root t2))

(show (:root t))
)

(defn delete-value [node victim-value]
  (let [u (find-key node victim-value)]
    (if (nil? u) (make-tree)
    (delete node u))))



;; # Map Tree
;;
;; This function takes a tree t and maps a function f over it.
;; If your tree is ((x 3 x) 5 ((x 7 x) 6 x)), then (map-tree t inc)
;; will return ((x 4 x) 6 ((x 8 x) 7 x))


(declare map-tree-aux)

(defn map-tree [t f]
  (BST. (map-tree-aux (:root t) f) (:size t)))

(defn map-tree-aux [t f]
(when-not (nil? t)
    (let [nuvalue (f (:value t))]
     (make-node (map-tree-aux (:left t) f)
                (:key t) nuvalue
                (map-tree-aux (:right t) f)))))










