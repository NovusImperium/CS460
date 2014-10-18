#!##############################################################################

              CS460  - Programming Languages
              Author - Amandeep Gill
              HW 5   - Quicksort and Mergesort

##############################################################################!#

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
  (sort (filter (lambda (a) (number? a)) as)))

;; Top-level function definition to call merge sort
;; input:   a list
;; output:  a sorted list of the numbers in the input list
(define (mergesort as)
  (define (sort as) ; splits the list in two and calls sort with the sublists
    (define (split as) ; splits the list into two parts
      (if (and (list? as) (not (null? as)))
        (let ((half (quotient (length as) 2)))
              (cons (take as half) (drop as half)))))
    (if (and (list? as) (not (null? as)))
      (if (null? (cdr as))
          as
          (let ((s (split as)))
            (merge (sort (car s)) (sort (cdr s)))))
      '()))
  (define (merge ls rs) ; merges the two lists into a single sorted list
    (cond ((and (not (null? ls)) (not (null? rs)))
           (if (< (car ls) (car rs))
             (cons (car ls) (merge (cdr ls) rs))
             (cons (car rs) (merge ls (cdr rs)))))
          ((and (not (null? ls)) (null? rs)) ls)
          ((and (null? ls) (not (null? rs))) rs)
          (else '())))
  (sort (filter number? as)))

;; takes up to the first i elements of the list
(define (take as i)
  (if (and (< 0 i) (not (null? as)))
    (cons (car as) (take (cdr as) (- i 1)))
    '()))

;; drops up to the first i elements from the list
(define (drop as i)
  (if (not (null? as))
    (if (< 0 i)
      (drop (cdr as) (- i 1))
      as)
    '()))

