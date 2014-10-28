;##############################################################################;
;                                                                              ;
;              CS460  - Programming Languages                                  ;
;              Author - Amandeep Gill                                          ;
;              HW 5   - Quicksort and Mergesort                                ;
;                                                                              ;
;##############################################################################;

;; Top-level function definition to call quick sort
;; input:   a list
;; output:  a sorted list of the numbers in the input list
(define (quicksort as)
  (define (sort as)
    (if (null? as)
        '()
        (append (sort (filter (lambda (a) (>= (car as) a)) (cdr as)))
          (list (car as))
          (sort (filter (lambda (a) (< (car as) a)) (cdr as))))))
  (if (list? as)
    (sort (filter (lambda (a) (number? a)) as))
    as))

;; Top-level function definition to call merge sort
;; input:   a list
;; output:  a sorted list of the numbers in the input list
(define (mergesort as)
  (define (sort as)
    (letrec ((merge (lambda (ls rs)
                       (cond ((null? ls) rs)
                             ((null? rs) ls)
                             ((< (car ls) (car rs)) (cons (car ls) (merge (cdr ls) rs)))
                             (else (cons (car rs) (merge ls (cdr rs))))))))
      (if (or (null? as) (< (length as) 2))
        as
        (let ((half (quotient (length as) 2)))
          (merge (sort (take as half)) (sort (drop as half)))))))
  (if (list? as)
    (sort (filter number? as))
    as))

;; takes up to the first n elements of the list
;; input:   a list
;; output:  the first n elements of the list
(define (take as n)
  (if (or (null? as) (<= n 0)) 
    '() 
    (cons (car as) (take (cdr as) (- n 1)))))

;; drops up to the first n elements from the list
;; input:   a list
;; output:  the last (length - n) elements of the list, or a null list if n > length
(define (drop as n)
  (if (or (null? as) (<= n 0)) 
    as 
    (drop (cdr as) (- n 1))))

