\ Is a number zero?
\ ( a -- a b )
: is_zero 0 = swap drop ;

\ Greet the user
: say_hi "Hello, World!\n" puts ;

\ The entry function
: main say_hi
	0 is_zero if
		0
	else
		1
	then
;
