include "should=.forth"

: array-test
    variable array 4 cells allot

    3 array ( str 3 array )
    2 cells + !
    \ get the item in [2]
    array 2 cells + @
;

: main
    ." 1 == 1  "  1         1 should=
    ." 1 < 2   "  1 2 <     1 should=
    ." 2 > 1   "  2 1 >     1 should=
    ." array[2]" array-test 3 should=
    0
;
