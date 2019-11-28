\ Assert that two things are equal!
: should= ( result expected -- )
    = if
        ."\t==> Passed\n"
    else
        ."\t==> Failed\n"
    then
;