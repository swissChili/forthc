\ Assert that two things are equal!
: should= ( description result expected -- )
    = if
        puts "\t\t==> Passed" puts endl
    else
        puts "\t\t==> Failed" puts endl
    then
;

: main
    "1 == 1" 1 1 should=
    "1 < 2" 1 2 < 1 should=
    "2 > 1" 2 1 > 1 should=
    0
;
