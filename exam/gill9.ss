;###########################################################################################################;
;                                                                                                           ;
;              CS460  - Programming Languages                                                               ;
;              Author - Amandeep Gill                                                                       ;
;              Exam 1 - Problem 9 fix
;                                                                                                           ;
;###########################################################################################################;

;; drops up to the first n elements from the list
;; input:   a list
;; output:  the last (length - n) elements of the list, or a null list if n > length
(define (drop as n)
  (if (or (null? as) (<= n 0)) 
    as 
    (drop (cdr as) (- n 1))))

;; returns the last two-thirds of a list
;; input:   a list
;; output:  a list with the first 1/3rd of the elemenents removed
(define (two-thirds as) (drop as (quotient (length as) 3)))

