\ Prints n spaces to stdout
: spaces ( n -- )
    0 do
        " " puts
    loop
    drop drop
;