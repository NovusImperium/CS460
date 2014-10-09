(define insert-first
  (lambda (l a) 
    (if (list? l)
      (cons a l)
      ("Can only prepend to a list"))))

(define remove-first
  (lambda (l)
    (if (and (list? l) (not (null? l))) 
      (car l)
      ("Can only get first element from a non-empty list"))))

(define list-copy
  (lambda (l)
    (if (and (list? l) (not (null? l)))
      (cons (car l) (list-copy (cdr l)))
      '())))

(define even-copy
  (lambda (l)
    (letrec ((skip 
              (lambda (l skip?)
                (if (and (list? l) (not (null? l)))
                  (if skip? 
                    (skip (cdr l) (not skip?))
                    (cons (car l) (skip (cdr l) (not skip?))))
                  '()))))
            (skip l #t))))

(define odd-copy
  (lambda (l)
    (letrec ((skip 
              (lambda (l skip?)
                (if (and (list? l) (not (null? l)))
                  (if skip? 
                    (skip (cdr l) (not skip?))
                    (cons (car l) (skip (cdr l) (not skip?))))
                  '()))))
             (skip l #f))))

(define insert-last
  (lambda (l a)
    (if (and (list? l) (not (null? l)))
      (if (null? (cdr l))
        (cons (car l) 'a)
        (cons (car l) (insert-last (cdr l) a)))
      '())))

(define remove-last 
  (lambda (l)
    (if (and (list? l) (not (null? l)))
      (if (null? (cdr l))
        '()
        (cons (cdr l) (remove-last (cdr l))))
      '())))

(define list-reverse
  (lambda (l)
    (if (and (list? l) (not (null? l)))
      (append (list-reverse (cdr l)) (list (car l)))
      '())))
