(define (domain blocks)
  (:requirements :strips :typing:equality
                 :universal-preconditions
                 :conditional-effects 
                 :negative-preconditions)
  (:types physob)
  (:predicates   
       (ontable ?x - physob)
       (clear ?x - physob) 
       (on ?x ?y - physob))
  (:action move
            :parameters (?x ?y - physob)
            :precondition (and(clear ?x)(clear ?y)(not(= ?x ?y)))
            :effect (and(on ?x ?y)(clear ?x)(not(clear ?y))
                    (forall (?z - physob)
                      (when (on ?x ?z)
                        (and(not(on ?x ?z))(clear ?z))
                      )
                    )
                    )
            )
  (:action moveToTable
             :parameters (?x - physob)
             :precondition (and (not(ontable ?x)) (clear ?x))
             :effect (and(ontable ?x)(clear ?x)
                    (forall (?z - physob)
                      (when (on ?x ?z)
                        (and(not(on ?x ?z))(clear ?z))
                      )
                    )
                    )
  )
)
