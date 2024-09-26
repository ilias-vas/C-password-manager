# pman

The basic idea of the program is that its a CLI password manager where the user can save a new password by first entering the account name of that password, then the password itself. Example:
```C
---------- [Password Manager Menu] ----------
1. Add password
2. Remove password
3. Display password (requires account name)
4. Display all account names
---------------------------------------------
Enter your choice> 1
Enter account name: google
Enter password: test
Password Saved!
```
This password would then be stored into a file as binary using an [XOR Cipher](https://en.wikipedia.org/wiki/XOR_cipher). So if the user in the above example entered `test`, this is how the cipher would work:

```
"test": 01110100 01100101 01110011 01110100
   key: 11110011 11110011 11110011 11110011
        -----------------------------------
Output: 10000111 10010110 10000000 10000111
```
That output would be saved into a "vault" binary file, which would be compressed with [Run-Length Encoding](https://en.m.wikipedia.org/wiki/Run-length_encoding).

Below i've added a a to-do list of features that make up the project. We can split these evenly between each other so that no one person is doing too much or too little work. We should start off by getting the basic features implemented, then we can slowly tack off the more advanced features as they should be much harder to implement than basic file storage and reading. 


## Feature List
### Basic Features
- [ ] Saving password to file with account name
- [ ] Removing password with account name
- [ ] Opening & Displaying password from file from account name
- [ ] Displaying all account names
### Advanced Features
 - [ ] XOR Encryption / Decryption
 - [ ] Run-Length Encoding for File Compression
 - [ ] Multiple Users via Linked List
 - [ ] Displaying all passwords
