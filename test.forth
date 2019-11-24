\ Is a number zero?
\ ( a -- a b )
\ : is_zero 0 = swap drop ;

\ Greet the user
: say_hi "Hello, World!\n" puts ;

\ The entry function
: main
    10 0 do say_hi loop 0
;
