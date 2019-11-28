\ Assert that two things are equal!
: should= ( result expected -- )
    = if
        "\t==> Passed" puts endl
    else
        "\t==> Failed" puts endl
    then
;