# Description

# Assumptions

# BUGS

- echo "Lorem Ipsum" prints `"Lorem Ipsum"` instead of `Lorem Ipsum`

# EDGE CASES TAKEN CARE OF

* Print the errors given by the commands even if they are in background
```
<JohnDoe@SYS:~>slepe 5 &
pid_of_process
<JohnDoe@SYS:~>
ERROR : 'slepe' is not a valid command
slepe exited normally (pid_of_process) # this line is displayed on pressing enter
```

```
(base) vineeth@vineeth:~$ ls &
191249
(base) vineeth@vineeth:~$ 
ceres-bin
ceres-solver-2.1.0 
```

* The command having more than 2 seconds continues to remain in the prompt after pressing enter/any other command

* warp ../../../../

* 







# Program Flow

- [Main](#main)

TO BE FIXED : & in input (example : `sleep 2; sleep 10 & echo hi`)
