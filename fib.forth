\ A fib function (bruh)
: fib
	dup \ ( a b b )
	third \ ( a b b a )
	+ \ ( a b a+b )
;

: main
	1 1 fib fib fib fib fib fib fib fib . endl 0
;
