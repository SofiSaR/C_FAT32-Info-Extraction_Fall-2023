# Project 3-Group 25

[Our shell gives the ability to view a user's shell info, execute simple commands,
check if errors exist in commands, and run commands concurrently in the background. ]

## Group Members
- **Roderick Shaw**: rks21b@fsu.edu
- **Fernando Parra**: fap20g@fsu.edu
- **Sofia Sanchez**: srs20h@fsu.edu
## Division of Labor

## BEFORE
### Part 1: Mount the Image File
- **Responsibilities**: [Prompt the user with the image name and its path, parse the image file, then create the info and exit commands.]
- **Assigned to**: Roderick Shaw

### Part 2: Navigation
- **Responsibilities**: [Pluck out all the individual entries, find the path, then create the cd and ls commands.]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 3: Create
- **Responsibilities**: N/A
- **Assigned to**: N/A

### Part 4: Read
- **Responsibilities**: [Create the open, close, lsof, lseek, and read commands.]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 5: Update
- **Responsibilities**: N/A
- **Assigned to**: N/A

### Part 6: Delete
- **Responsibilities**: N/A
- **Assigned to**: N/A

## AFTER
### Part 1: Prompt
- **Responsibilities**: [Prompt the user with the image name and its path, parse the image file, then create the info and exit commands.]
- **Assigned to**: Roderick Shaw

### Part 2: Environment Variables
- **Responsibilities**: [Pluck out all the individual entries, find the path, then create the cd and ls commands.]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 4: $PATH Search
- **Responsibilities**: [Create the open, close, lsof, lseek, and read commands.]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez


## File Listing
```
README.md
starter/
│
├── src/
│ |── main.c
│ |── bpb_reader.c
│ |── dir.c
│ |── cd.c
│ |── lexer.c
│ |── open.c
│ └── prompt.c
│
├── include/
│ |── lexer.h
│ |── dir.h
│ |── open.h
| └── bpb_reader.h
│
├── fat32.img
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: gcc -std=c99 nameOfFile -o whatYouWantTheExecutableToBeNamed
- **Dependencies**: None needed to be downloaded

### Compilation
```bash
make
```
This makes the whole executable for our Project 3.
### Execution
```bash
./bin/shell fat32.img
```
This will run the shell program and give you access to every command we provide for a FAT32 image file.
You cannot do "make run" because you won't be able to insert the image file.
