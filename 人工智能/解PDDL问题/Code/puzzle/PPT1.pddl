(define (problem PPT1)
  (:domain puzzle)
  (:objects L1 L2 L3 L4 L5 L6 L7 L8 L9 L10 L11 L12 L13 L14 L15 L16 - loc
            B1 B2 B3 B4 B5 B6 B7 B8 B9 B10 B11 B12 B13 B14 B15 empty - num)
  (:init (is-at B1 L7)(is-at B2 L9)(is-at B3 L10)(is-at B4 L5)(is-at B5 L11)
         (is-at B6 L3)(is-at B7 L15)(is-at B8 L8)(is-at B9 L6) (is-at B10 L2)(is-at B11 L12)
         (is-at B12 L13)(is-at B13 L14)(is-at B14 L1)(is-at B15 L16)(is-at empty L4)(is-empty L4)

         (adjacent L1 L2)(adjacent L2 L3)(adjacent L3 L4)(adjacent L4 L8)(adjacent L1 L5)(adjacent L2 L6)
         (adjacent L3 L7)(adjacent L5 L6)(adjacent L5 L9)(adjacent L6 L7)(adjacent L6 L10)(adjacent L7 L8)
         (adjacent L7 L11)(adjacent L8 L12)(adjacent L9 L10)(adjacent L13 L9)(adjacent L10 L11)(adjacent L10 L14)
         (adjacent L11 L12)(adjacent L11 L15)(adjacent L12 L16)(adjacent L13 L14)(adjacent L14 L15)(adjacent L16 L15)

         (adjacent L2 L1)(adjacent L3 L2)(adjacent L4 L3)(adjacent L8 L4)(adjacent L5 L1)(adjacent L6 L2)
         (adjacent L7 L3)(adjacent L6 L5)(adjacent L9 L5)(adjacent L7 L6)(adjacent L10 L6)(adjacent L8 L7)
         (adjacent L11 L7)(adjacent L12 L8)(adjacent L10 L9)(adjacent L9 L13)(adjacent L11 L10)(adjacent L14 L10)
         (adjacent L12 L11)(adjacent L15 L11)(adjacent L16 L12)(adjacent L14 L13)(adjacent L15 L14)(adjacent L15 L16))
  (:goal (and (is-at B1 L1)(is-at B2 L2)(is-at B3 L3)(is-at B4 L4)(is-at B5 L5)
              (is-at B6 L6)(is-at B7 L7)(is-at B8 L8)(is-at B9 L9) (is-at B10 L10)
              (is-at B11 L11)(is-at B12 L12)(is-at B13 L13)(is-at B14 L14)(is-at B15 L15)
              (is-at empty L16)(is-empty L16)))
)

