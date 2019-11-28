\ These functions are useful to index arrays.
\ e.g: array-ptr 3 cells + @ is equivelant to array-ptr[3]

: cells ( n1 -- n2 )
    8 *
;

: bytes ( n1 -- n1 ) ;
: chars ( n1 -- n1 ) ;
