\ Assert that two things are equal!
: should= ( description result expected -- )
    = if
        puts "\t\t==> Passed" puts endl
    else
        puts "\t\t==> Failed" puts endl
    then
;