\ fib finds the next fibonacci number in the sequence stored on the stack
: fib ( a b -- b c )
    variable b
    b ! \ b=b
    drop ( b a )
    + ( c )
    b @ swap ( b c )
;

\ nth_fib finds the nth fibonacci number
: nth_fib ( a -- a n )
    variable n
    \ Store the limit in n
    n ! drop ( a )
    \ The first 2 fibonacci numbers
    1 1 ( a 1 1 )
    \ Push n, and starting at 2 (1 and 1 are already there)
    n @ 2 do ( a 1 1 )
        fib ( fib(n-1) fib(n) )
    loop
    swap drop ( fib(n) )
;

: main
	10 nth_fib . endl 0
;
