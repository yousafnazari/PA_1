

@mainpage ELEC 331 Programming Assignment 1

@author Jayden Sahl (jaydensahl)
@author Yousaf Nazari (yousafnazari)

Status of the project: 
- Can send any file type over UDP from sender to receiver
- at the time of submission the rdt FSM was not succesfully implemented
- no error detection

Limitations:
- bytesToTransfer not implemented, just transfers the entire file
- works well running on localhost, had issues testing on cloudlab

How it was tested:
- Two terminals open in WSL VS Code
- on one run 'make receiver'
- on the other run 'make sender'
- for testing sake, the input parameters, were left in the makefile, but these can  be easily removed and run from the command line, 
-- e.g run 'src/sender.c -o src/send' then run 'src/send 127.0.0.1 4000 "BelieveItorNot.mp4" 2048'
- tested files: BelieveItorNot.mp4, shrek.txt, forest.jpg

