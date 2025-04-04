(define (domain puzzle)
  (:requirements :strips :equality :typing)
  (:types num loc)
  (:predicates 
    (is-at ?n - num ?l - loc)
    (adjacent ?l1 ?l2 - loc)
    (is-empty ?l - loc))
  (:action slide
    :parameters (?n - num ?l1 ?l2 - loc)
    :precondition (and (is-at ?n ?l1)(is-empty ?l2)(adjacent ?l1 ?l2)(not(= ?l1 ?l2)))
    :effect (and (is-at ?n ?l2)(is-empty ?l1)(not (is-empty ?l2))))
)
