(ns linked_lists.core)

(defrecord Cons [car cdr])

(defn insert-at-beginning [elt xx] (Cons. elt xx))

;; Insert at End

(defn insert-at-end [elt xx] 
 (cond (empty? xx) (Cons. elt nil) 
       :else (Cons. (:car xx) (insert-at-end elt (:cdr xx)))))

;; Sorted Insert

(defn sorted-insert [item xx]
  (cond (empty? xx) (Cons. item nil) 
        (< item (:car xx)) (Cons. item xx)
        :else (Cons. (:car xx) (sorted-insert item (:cdr xx)))))

;; Search

(defn search [elt xx]
  (cond (empty? xx) false
        (= (:car xx) elt) true
          :else (search elt (:cdr xx))))

;; Delete

(defn delete [elt xx]
  (cond (empty? xx) xx 
      (= elt (:car xx)) (:cdr xx)
        :else (Cons. (:car xx) (delete elt (:cdr xx)))))

;; Delete-all
;; DELETES EVERY COPY OF THE ELEMENT IN THE LIST

(defn delete-all [elt xx]
  (cond (empty? xx) xx 
      (= (:car xx) elt) (delete-all elt (:cdr xx))
        :else (Cons. (:car xx) (delete-all elt (:cdr xx)))))

;;  Efficient-Delete (Memory Efficient Delete) 
;;  "Delete a copy of elt from xx, but if elt is not in xx, return the
;;  *original* xx instead of a copy.
  
(defn efficient-delete [elt xx]
    (cond (empty? xx) nil
    (search elt xx) (delete elt xx)
      :else xx)) 



