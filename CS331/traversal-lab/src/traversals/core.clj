
(ns traversals.core)

;; Given Code

(defrecord BNode [left data right])

(defn add [t elt]
   (cond (nil? t)          (BNode. nil elt nil)
         (= elt (:data t)) t
         (< elt (:data t)) (BNode. (add (:left t) elt) (:data t) (:right t))
         :else             (BNode. (:left t) (:data t) (add (:right t) elt))))

;; A fast way to make trees is to use the code
;; (reduce add nil '(4 2 3 5 9))
;;
;; Use (reduce #(str "(" %1 " " %2 ")") "0" '(1 2 3)) to get an idea what it's doing.

;; # Your Code


(defn show [t]
   (if (nil? t)
     "x"
       (str "(" (show (:left t)) " " (:data t)
           " " (show (:right t)) ")")))

(def t (add (add (add (add (add (add (add nil 6) 3) 8) 2) 5) 7) 9))

(show t)

(declare postorder-aux)

(defn postorder "Outputs a list containing the preorder traversal of the given tree." [t]
    (postorder-aux t))

(defn postorder-aux [t]
  (if (nil? t) '()
    (concat (postorder-aux (:left t))
            (postorder-aux (:right t))
            (list (:data t)))))

(postorder t)

(declare preorder-aux)

(defn preorder "Outputs a list containing the preorder traversal of the given tree." [t]
  (preorder-aux t))

(defn preorder-aux [t]
  (if (nil? t) '()
    (concat (list (:data t))
         (preorder-aux (:left t))
         (preorder-aux (:right t))
         )))

(preorder t)

(declare inorder-aux)

(defn inorder "Outputs a list containing the preorder traversal of the given tree." [t]
  (inorder-aux t))

(defn inorder-aux [t]
  (if (nil? t) '()
    (concat (inorder-aux (:left t))
            (list (:data t))
            (inorder-aux (:right t))
         )))

(inorder t)

(declare get-kids)

(defn levelorder-aux [q xx]
        (if (empty? q) xx
          (let [q (concat q (get-kids (first q)))]
        (levelorder-aux  (rest q) (concat xx (list (:data (first q)))) ))))

(defn levelorder "Outputs a list containing the level-order traversal of the given tree." [t]
  (if (nil? t) '()
    (levelorder-aux (list t) '() ))) ;; first arg is list of trees acting like a queue, and the second is a list

(defn get-kids [t]
  (let [left (:left t)
        right (:right t)]
    (cond (and (nil? left) (nil? right)) '()
          :else (concat (list left) (list right)))))

(levelorder t)


(declare frontier-aux)

(defn frontier "Outputs a list containing the frontier of the given tree." [t]
    (frontier-aux t))

(defn frontier-aux [t]
 (cond (nil? t) '()
          (and (nil? (:left t)) (nil? (:right t))) (list (:data t))
          :else (concat (frontier (:left t)) (frontier (:right t)))))

(frontier t)

(comment
(defn map-tree
  "Create a new tree by applying the given function to all the elements."
  [f t]
  nil)
)














