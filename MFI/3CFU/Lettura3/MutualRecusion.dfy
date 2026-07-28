/**
    Lexicographic order and mutual recursion
*/

// returns the number of hours needed for completing course c
method RequiredStudyTime(c: nat) returns (hours: nat)

// courses are numbered n, n - 1, ... , 1
// h refers to the remaining hours for completeing course n
method Study(n: nat, h: nat)
{
    if h != 0 {
        // study 1 hour
        Study(n, h - 1);
    } else if n == 0 {
        // end of all courses
    } else {
        var h2 := RequiredStudyTime(n - 1);
        Study(n - 1, h2);
    }
}

method StudyPlan(n: nat)
    requires n <= 40
    decreases 40 - n
{
    if  n == 40 {
        // done
    } else {
        var h := RequiredStudyTime(n);
        Learn(n, h);
    }
}

method Learn(n: nat, h: nat)
    requires n < 40
    decreases 40 - n, h
{
    if h == 0 {
        // course n is done, go ahead to course n + 1
        StudyPlan(n + 1);
    } else {
        // there are still hours of work
        Learn(n, h - 1);
    }
}