# assignment4

Problem Statement & Persistence Design:

In order to keep the flight log data after ending the program, I used fwrite() to save the data to a file called flight_data.bin. Then, I simply use fread() to read the flight_data.bin file, and then load it at the start of the program to keep the saved data. I also make sure to validate the file when reading it to make sure the data isn't corrupted or has data that is invalid. Since all the code for the menus and actions are the same as the previous assignment, I wont talk about the layout/structure for them here.

File Validation & Error Recovery Analysis:

At the start of my program, I attempt to load_data() to keep any saved data and if there isnt any then it starts with all the data empty. If there is data to load, it then checks the file size using fseek() and ftell(). This detects truncated files by making sure the data contains all 48 seats and not less like 10. It also checks for over-sized files with extra bytes on the end. After making sure the file is the correct size, the program uses fread() to load the saved outbound and inbound data. This data is also checked for unexpected EOF, and if the expected number of records cannot be read, the file is tossed and the flight data starts fresh. After loading, the program uses validate_flight() to make sure the seat numbers are from 1-24 and that they are either assigned or not assigned(0 or 1). It also checks that the names dont have any unprintable characters. this checks both garbage data/non-printable characters as well as any out of bound seat numbers and corrupted assignment statuses. Like the other checks, if this fails then the file is tossed and the flight log starts fresh.

Pros & Cons of Solution: 

The main pros of binary files is that saving and loading files is super easy. Only needing to use fwrite() and fread() to save the data to a file and then load it again later on. Not only is it simpler, but its also faster in saving and loading the data overall. As for its cons, it isnt easy for humans to read and similarly also not easy to edit incase you need to go and fix or change a specific piece of data. Its also less flexible overall as binary struct files can work differently from one compiler to the next, as well as create problems when adding new fields to the structure.

AI Fuzzing Reflection:

The prompt I used for the fuzzer.c file, was simply asking it to create a fuzzer.c file that simulates truncated files, over-sized files, non-printable characters inside string fields and out-of-bound seat numbers. At the start, the only one my code was able to catch was truncated files and loaded the rest of the corrupted files without realizing they were wrong. In order to fix this, I created the function validate_flight() and also updated the code to check for over-sized files as well by comparing the file size to the expected file size. Then the code goes through the process mentioned in "File Validation & Error Recovery Analysis" to tackle these issues.
