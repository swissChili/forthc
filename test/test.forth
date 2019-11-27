include "should=.forth"

: main
    "1 == 1" 1 1 should=
    "1 < 2" 1 2 < 1 should=
    "2 > 1" 2 1 > 1 should=
    0
;
