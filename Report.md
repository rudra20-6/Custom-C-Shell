# Mini Project 1
Submitted by  
Rudra Choudhary  
2024121012  

### Instructions to Compile and Run
- Run the make command which will compile the code and make the executable shell
- Run the created shell executable

  
  ![image](https://github.com/user-attachments/assets/a322d1b0-1d69-422e-9aaa-23ecc00616a0)


**All Assumptions are mentioned within the respective spec descriptions.**  
## Detailed Descriptions, Assumptions and Sample Output screesnshots for each specification

## [Spec 1] Display Requirements

_This spec is implemented in parser.c_

**Important  :** In all the screenshots in the readme the Username is displayed as None, this is because the code was run and made in WSL, the username is NOT hardcoded, infact it works on linux system (example screenshot below).


![image](https://github.com/user-attachments/assets/e7ff35ae-e5ce-48aa-a9ea-c3dcfc407716)

### Features :
- The shell prompt displays the username, system name, and current directory in the format <Username@SystemName:CurrentPath>.
- The username and system name are automatically retrieved from the system environment.
- The directory where the shell is invoked is treated as the home directory and represented by ~.
- Paths within the home directory are shown as relative, while paths outside are shown as absolute.
- The prompt dynamically updates to reflect changes in the current working directory after every directory change.


 ### Sample Outputs
  

![image](https://github.com/user-attachments/assets/55c88fc9-fbba-420b-9703-6b028ef45eb7)


## [Spec 2] Input Requirements

_This spec is implemented in parser.c_

### Features:  
- The shell supports both ; and & to separate multiple commands in a single input line.
- Commands separated by ; are executed sequentially, similar to how it's done in Bash.
- The & operator runs the preceding command in the background and prints the process ID of the newly created background process.
- Random spaces and tabs in the input are handled, ensuring commands are parsed correctly.
- If a command is invalid, the shell prints an error message specifying that the command is not valid.

### Sample Outputs

![image](https://github.com/user-attachments/assets/27ce7554-d115-409d-a2c3-f940d1d1b2dd)


![image](https://github.com/user-attachments/assets/f16652a6-9d9b-439d-a9fd-25e952f403f2)



## [Spec 3]  `hop` Command

_This spec is implemented in hop.c_  

### Features:  
- The command accepts both absolute and relative paths, including paths starting from the home directory (~).
- Special flags supported:
  - `.`: Represents the current directory.
  - `..`: Moves up one directory.
  - `~`: Represents the home directory of the shell.
  - `-`: Switches to the previously visited directory.
- If no argument is provided, the shell moves to the home directory by default.
- Multiple arguments are supported, with hop changing directories sequentially for each argument, from left to right.

### Sample Outputs  

![image](https://github.com/user-attachments/assets/e61d64d0-c62c-4999-b550-7da885a6a577)  



## [Spec 4]  `reveal` Command  

_This spec is implemented in reveal.c_ 

### **ASSUMPTIONS**  

`reveal filename` is an invalid command, reveal only works for directories.

### Features:  
- Special flags supported:
-   `-a`: Displays all files, including hidden ones.
-   `-l`: Displays extra information about each file (similar to the ls -l command in Bash).
-   Flags can be combined in various forms (e.g., -a -l, -la, -lala).
- The command supports the . (current directory), .. (parent directory), ~ (home directory), and - (previous directory) symbols.  
- Both relative and absolute paths are supported.
- If no path is provided, the reveal command lists files and directories in the current working directory.
The command always expects the format: reveal <flags> <path/name>. Multiple arguments are not allowed.

### Sample Outputs  


![image](https://github.com/user-attachments/assets/32e8c3d0-2803-4265-b4ca-398b18066c9b)  

![image](https://github.com/user-attachments/assets/596758e7-6ec1-4f6e-a19d-08707c055dfb)

![image](https://github.com/user-attachments/assets/50926cf9-123a-41bc-b128-a773866f7f65)  


## [Spec 5]  `log` Command  

_This spec is implemented in log.c_   
_Log entries are stored in logFile.txt_

### **ASSUMPTIONS**

- Erroneous commands ARE stored in logFile
- Log is indexed 1 indexed starting from most recent to oldest


### Features
- The log does not store duplicate consecutive commands. Commands must differ from the previous entry to be stored.
- The log persists across all shell sessions.

### Sample Outputs  

![image](https://github.com/user-attachments/assets/bc3d8c73-0529-4fc2-8d20-5b46a31ca4b1)

![image](https://github.com/user-attachments/assets/eb0066a3-ff14-4ef5-9885-a23ab8be0ad7)  


## [Spec 6]  System commands   

_This spec is implemented in systemCommands.c_   

### **Assumptions**
IMP : I have implemnted the functionality of background process according to how it happens in bash,
    - If a process has finished executing in the background the message displaying it would be printed whenever user types in the terminal next time.  
    - Also like in bash the bg process doesnt have the terminal access for input but it still may print to the terminal.
    - Terminal interactive processes won't be run in the background as the need constant terminal input.  
    ![image](https://github.com/user-attachments/assets/0f85f5f3-94e7-4b8c-b635-187e94c9d29c)


### Features

- Execution time for foreground process is displayed
- Background processes prompts for normal and abnormal exits

### Sample Output  

![image](https://github.com/user-attachments/assets/dd7b3cc7-05c6-4a33-8f00-4b71704d3a03)  

![image](https://github.com/user-attachments/assets/f5f809e3-e0fa-4f80-b2d5-e04836b0d4e4)


## [Spec 7] `proclore` commands  

_This spec is implemented in proclore.c_   

I have used /proc to retrieve data for proc

### Sample Outputs  


![image](https://github.com/user-attachments/assets/9ab9c013-db2f-4a6a-aa49-76142ce00e26)   

![image](https://github.com/user-attachments/assets/0011ec85-031c-42cd-bbe3-ef2c4e8e4764)  

## [Spec 8] `seek` commands  

_This spec is implemented in seek.c_   

### Features  

- seek searches for a file or directory in the target directory or current working directory if no target is specified.
- Results are relative paths, with files displayed in green and directories in blue, each on a new line.
- -d flag restricts the search to directories.
- -f flag restricts the search to files.
- -e flag, when used, changes the working directory if exactly one matching directory is found or prints the content of the file if exactly one file is found. Otherwise, it has no effect.
- If permissions are missing on the file/directory, print "Missing permissions for task!".
- seek supports the symbols ".", "..", "~", and "-" for paths as in the reveal command.
- Multiple arguments are supported: the first argument is the target name to search, and the second is the directory path (optional).
- If no match is found, print "No match found!".
- If both -d and -f flags are used together, return "Invalid flags!".

### Sample Outputs

![image](https://github.com/user-attachments/assets/d825cfab-2b59-4f6d-916d-661a357961d8)  


![image](https://github.com/user-attachments/assets/64adf56d-16ae-4534-8f4d-f3d151245361)


## [Spec 10] I/O Redirection  

_This spec is implemented in redirection.c and systemCommands.c_   

### **Assumptions**

- & will NOT be inputed in middle of the I/O redirection command
  `echo hello & > a.txt` is an **INVALID** command  
  `echo niceCommand & > a.txt` is a **VALID** command

### Features
- Can handle Input, Output redirection and also both at the same time
- Can run in background also
  
### Sample Outputs
![image](https://github.com/user-attachments/assets/36c6791b-fdb6-4d41-8f31-6ed9c0727ff9)  

![image](https://github.com/user-attachments/assets/00745f6a-8229-4fee-b0a4-9e51d412d5f2)  

![image](https://github.com/user-attachments/assets/ca041ba5-bf61-4cac-b762-ee5d6dd61c1f)  


## [Spec 11] Pipe Commands

_This spec is implemented in piping.c and systemCommands.c_   

### **Assumptions**
- & will NOT be inputed in middle of the piped command.
    - `echo hello & | wc` is an **INVALID**  command.
    - `echo hello | wc &` is a **VALID** command.
 
### Features
- Implemented piping for any number of commands.

### Sample Outputs  

![image](https://github.com/user-attachments/assets/585ec690-ec06-4ea3-8b78-1204f2ac658b)  


![image](https://github.com/user-attachments/assets/5630c289-0276-4f90-9098-0c17a2bedd3c)


## [Spec 12] Redirection along with pipes  

_This spec is implemented in piping.c, redirection.c and systemCommands.c_   

### **Assumptions**
- Following the assumptions for piping and redirection, 
    - `echo hello & | wc & > a.txt ` is an **INVALID**  command.
    - `echo hello | wc > a.txt &` is a **VALID** command.
 
### Features
- Piping and redirection work together
- can be run in background

### Sample Outputs

![image](https://github.com/user-attachments/assets/e6e71589-e9e0-4e4e-b33d-a26961848747)  


## [Spec 13] `activities` command 

_This spec is implemented in activities.c_   


### Sample Outputs

![image](https://github.com/user-attachments/assets/adb32d21-3e5c-4b21-93ce-14d0790dabc5)


## [Spec 14] signals 

_This spec is implemented in ping.c and systemCommands.c_


### Sample Outputs

![image](https://github.com/user-attachments/assets/86ab8976-3fee-4e73-acb1-629b57b05af6)

![image](https://github.com/user-attachments/assets/d0e47626-5082-489d-80de-291f31a5ac55)  

![image](https://github.com/user-attachments/assets/63caa37c-d9fe-4d96-a5f8-77bc2a9c8ac0)  


emacs terminated after ctrl c  
![image](https://github.com/user-attachments/assets/a47584c0-8f75-472d-892d-1817f66f44c0)


ctrl D

![image](https://github.com/user-attachments/assets/d0cb8b10-8d8b-4728-b153-0ada5297e6f9)

## [Spec 15] `fg` and `bg` commands 

_This spec is implemented in fgbg.c_  

### **Assumptions**  

- As interactive process are not supposed to be run in backgrounf BG would NOT be used for interactive processes
- emacs and gedit are NOT TERMINAL interactive

### Sample Outputs  

# Mini Project 1
Submitted by  
Rudra Choudhary  
2024121012  

### Instructions to Compile and Run
- Run the make command which will compile the code and make the executable shell
- Run the created shell executable

  
  ![image](https://github.com/user-attachments/assets/a322d1b0-1d69-422e-9aaa-23ecc00616a0)

## Detailed Descriptions, Assumptions and Sample Output screesnshots for each specification

## [Spec 1] Display Requirements

_This spec is implemented in parser.c_

**Important  :** In all the screenshots in the readme the Username is displayed as None, this is because the code was run and made in WSL, the username is NOT hardcoded, infact it works on linux system (example screenshot below).


![image](https://github.com/user-attachments/assets/e7ff35ae-e5ce-48aa-a9ea-c3dcfc407716)

### Features :
- The shell prompt displays the username, system name, and current directory in the format <Username@SystemName:CurrentPath>.
- The username and system name are automatically retrieved from the system environment.
- The directory where the shell is invoked is treated as the home directory and represented by ~.
- Paths within the home directory are shown as relative, while paths outside are shown as absolute.
- The prompt dynamically updates to reflect changes in the current working directory after every directory change.


 ### Sample Outputs
  

![image](https://github.com/user-attachments/assets/55c88fc9-fbba-420b-9703-6b028ef45eb7)


## [Spec 2] Input Requirements

_This spec is implemented in parser.c_

### Features:  
- The shell supports both ; and & to separate multiple commands in a single input line.
- Commands separated by ; are executed sequentially, similar to how it's done in Bash.
- The & operator runs the preceding command in the background and prints the process ID of the newly created background process.
- Random spaces and tabs in the input are handled, ensuring commands are parsed correctly.
- If a command is invalid, the shell prints an error message specifying that the command is not valid.

### Sample Outputs

![image](https://github.com/user-attachments/assets/27ce7554-d115-409d-a2c3-f940d1d1b2dd)


![image](https://github.com/user-attachments/assets/f16652a6-9d9b-439d-a9fd-25e952f403f2)



## [Spec 3]  `hop` Command

_This spec is implemented in hop.c_  

### Features:  
- The command accepts both absolute and relative paths, including paths starting from the home directory (~).
- Special flags supported:
  - `.`: Represents the current directory.
  - `..`: Moves up one directory.
  - `~`: Represents the home directory of the shell.
  - `-`: Switches to the previously visited directory.
- If no argument is provided, the shell moves to the home directory by default.
- Multiple arguments are supported, with hop changing directories sequentially for each argument, from left to right.

### Sample Outputs  

![image](https://github.com/user-attachments/assets/e61d64d0-c62c-4999-b550-7da885a6a577)  



## [Spec 4]  `reveal` Command  

_This spec is implemented in reveal.c_ 


### Features:  
- Special flags supported:
-   `-a`: Displays all files, including hidden ones.
-   `-l`: Displays extra information about each file (similar to the ls -l command in Bash).
-   Flags can be combined in various forms (e.g., -a -l, -la, -lala).
- The command supports the . (current directory), .. (parent directory), ~ (home directory), and - (previous directory) symbols.  
- Both relative and absolute paths are supported.
- If no path is provided, the reveal command lists files and directories in the current working directory.
The command always expects the format: reveal <flags> <path/name>. Multiple arguments are not allowed.

### Sample Outputs  


![image](https://github.com/user-attachments/assets/32e8c3d0-2803-4265-b4ca-398b18066c9b)  

![image](https://github.com/user-attachments/assets/596758e7-6ec1-4f6e-a19d-08707c055dfb)

![image](https://github.com/user-attachments/assets/50926cf9-123a-41bc-b128-a773866f7f65)  


## [Spec 5]  `log` Command  

_This spec is implemented in log.c_   
_Log entries are stored in logFile.txt_

### **ASSUMPTIONS**

- Erroneous commands ARE stored in logFile
- Log is indexed 1 indexed starting from most recent to oldest


### Features
- The log does not store duplicate consecutive commands. Commands must differ from the previous entry to be stored.
- The log persists across all shell sessions.

### Sample Outputs  

![image](https://github.com/user-attachments/assets/bc3d8c73-0529-4fc2-8d20-5b46a31ca4b1)

![image](https://github.com/user-attachments/assets/eb0066a3-ff14-4ef5-9885-a23ab8be0ad7)  


## [Spec 6]  System commands   

_This spec is implemented in systemCommands.c_   

### **Assumptions**
IMP : I have implemnted the functionality of background process according to how it happens in bash,
    - If a process has finished executing in the background the message displaying it would be printed whenever user types in the terminal next time.  
    - Also like in bash the bg process doesnt have the terminal access for input but it still may print to the terminal.
    - Terminal interactive processes won't be run in the background as the need constant terminal input.  
    ![image](https://github.com/user-attachments/assets/0f85f5f3-94e7-4b8c-b635-187e94c9d29c)


### Features

- Execution time for foreground process is displayed
- Background processes prompts for normal and abnormal exits

### Sample Output  

![image](https://github.com/user-attachments/assets/dd7b3cc7-05c6-4a33-8f00-4b71704d3a03)  

![image](https://github.com/user-attachments/assets/f5f809e3-e0fa-4f80-b2d5-e04836b0d4e4)


## [Spec 7] `proclore` commands  

_This spec is implemented in proclore.c_   

I have used /proc to retrieve data for proc

### Sample Outputs  


![image](https://github.com/user-attachments/assets/9ab9c013-db2f-4a6a-aa49-76142ce00e26)   

![image](https://github.com/user-attachments/assets/0011ec85-031c-42cd-bbe3-ef2c4e8e4764)  

## [Spec 8] `seek` commands  

_This spec is implemented in seek.c_   

### Features  

- seek searches for a file or directory in the target directory or current working directory if no target is specified.
- Results are relative paths, with files displayed in green and directories in blue, each on a new line.
- -d flag restricts the search to directories.
- -f flag restricts the search to files.
- -e flag, when used, changes the working directory if exactly one matching directory is found or prints the content of the file if exactly one file is found. Otherwise, it has no effect.
- If permissions are missing on the file/directory, print "Missing permissions for task!".
- seek supports the symbols ".", "..", "~", and "-" for paths as in the reveal command.
- Multiple arguments are supported: the first argument is the target name to search, and the second is the directory path (optional).
- If no match is found, print "No match found!".
- If both -d and -f flags are used together, return "Invalid flags!".

### Sample Outputs

![image](https://github.com/user-attachments/assets/d825cfab-2b59-4f6d-916d-661a357961d8)  


![image](https://github.com/user-attachments/assets/64adf56d-16ae-4534-8f4d-f3d151245361)


## [Spec 10] I/O Redirection  

_This spec is implemented in redirection.c and systemCommands.c_   

### **Assumptions**

- & will NOT be inputed in middle of the I/O redirection command
  `echo hello & > a.txt` is an **INVALID** command  
  `echo niceCommand & > a.txt` is a **VALID** command

### Features
- Can handle Input, Output redirection and also both at the same time
- Can run in background also
  
### Sample Outputs
![image](https://github.com/user-attachments/assets/36c6791b-fdb6-4d41-8f31-6ed9c0727ff9)  

![image](https://github.com/user-attachments/assets/00745f6a-8229-4fee-b0a4-9e51d412d5f2)  

![image](https://github.com/user-attachments/assets/ca041ba5-bf61-4cac-b762-ee5d6dd61c1f)  


## [Spec 11] Pipe Commands

_This spec is implemented in piping.c and systemCommands.c_   

### **Assumptions**
- & will NOT be inputed in middle of the piped command.
    - `echo hello & | wc` is an **INVALID**  command.
    - `echo hello | wc &` is a **VALID** command.
 
### Features
- Implemented piping for any number of commands.

### Sample Outputs  

![image](https://github.com/user-attachments/assets/585ec690-ec06-4ea3-8b78-1204f2ac658b)  


![image](https://github.com/user-attachments/assets/5630c289-0276-4f90-9098-0c17a2bedd3c)


## [Spec 12] Redirection along with pipes  

_This spec is implemented in piping.c, redirection.c and systemCommands.c_   

### **Assumptions**
- Following the assumptions for piping and redirection, 
    - `echo hello & | wc & > a.txt ` is an **INVALID**  command.
    - `echo hello | wc > a.txt &` is a **VALID** command.
 
### Features
- Piping and redirection work together
- can be run in background

### Sample Outputs

![image](https://github.com/user-attachments/assets/e6e71589-e9e0-4e4e-b33d-a26961848747)  


## [Spec 13] `activities` command 

_This spec is implemented in activities.c_   


### Sample Outputs

![image](https://github.com/user-attachments/assets/adb32d21-3e5c-4b21-93ce-14d0790dabc5)


## [Spec 14] signals 

_This spec is implemented in ping.c and systemCommands.c_


### Sample Outputs

![image](https://github.com/user-attachments/assets/86ab8976-3fee-4e73-acb1-629b57b05af6)

![image](https://github.com/user-attachments/assets/d0e47626-5082-489d-80de-291f31a5ac55)  

![image](https://github.com/user-attachments/assets/63caa37c-d9fe-4d96-a5f8-77bc2a9c8ac0)  


emacs terminated after ctrl c  
![image](https://github.com/user-attachments/assets/a47584c0-8f75-472d-892d-1817f66f44c0)


ctrl D

![image](https://github.com/user-attachments/assets/d0cb8b10-8d8b-4728-b153-0ada5297e6f9)

## [Spec 15] `fg` and `bg` commands 

_This spec is implemented in fgbg.c_  

### **Assumptions**  

- As interactive process are not supposed to be run in backgrounf BG would NOT be used for interactive processes
- emacs and gedit are NOT TERMINAL interactive

### Sample Outputs  

![image](https://github.com/user-attachments/assets/7295c8bc-eddb-4c32-839a-bfed9906b942)

![image](https://github.com/user-attachments/assets/25d5b03b-70eb-4b67-a51d-3a348a5c7930)

## [Spec 16] `Neonate` command  

_This spec is implemented in neonate.c_  

### Sample Outputs  


![image](https://github.com/user-attachments/assets/050a9f33-71da-4ff7-a85a-2f2c5e63662a)  


## [Spec 16] `Neonate` command  

_This spec is implemented in iMan.c_  

### Sample Outputs  

![image](https://github.com/user-attachments/assets/fc47b646-fa16-4717-a1f6-b7355261defe)
