(define merge 
  (lambda (ls rs)
    (cond ((and (not (null? ls)) (null? rs)) ls)
          ((and (null? ls) (not (null? rs))) rs)
          ((and (not (null? ls)) (not (null? rs)))
           (if (< (car ls) (car rs))
             (cons (car ls) (merge (cdr ls) rs))
             (cons (car rs) (merge ls (cdr rs)))))
          (else '()))))

(define even-split
  (lambda (l)
    (letrec ((skip 
              (lambda (l skip?)
                (if (and (list? l) (not (null? l)))
                  (if skip? 
                    (skip (cdr l) (not skip?))
                    (cons (car l) (skip (cdr l) (not skip?))))
                  '()))))
            (skip l #t))))

(define odd-split
  (lambda (l)
    (letrec ((skip 
              (lambda (l skip?)
                (if (and (list? l) (not (null? l)))
                  (if skip? 
                    (skip (cdr l) (not skip?))
                    (cons (car l) (skip (cdr l) (not skip?))))
                  '()))))
             (skip l #f))))

(define sort
  (lambda (as)
      (if (and (list? as) (not (null? as)))
        (merge (sort (even-split as)) (sort (odd-split as)))
        '())))

(define mergesort 
  (lambda (as)
    (merge (sort (even-split as)) (sort (odd-split as)))))
