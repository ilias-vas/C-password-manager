# P(assword) MAN(ager)

pman is a simple terminal password manager for managing different accounts.

```C
[pman] Enter password.
> *****
[pman] Welcome (name)
[1] Show Accounts
[2] Add Account
[3] Edit Account
[4] Remove Account
[5] Show Password
[6] Exit
```

```
[pman] Accounts
        ├─important
        │  ├─gmail
        │  │  ├─personal
        │  │  ├─work
        │  │  └─school
        │  └─bank
        ├─social
        │  ├─instagram
        │  ├─reddit
        │  └─github
        └─master password
```

```
[pman] Edit Account
[pman] Enter Account name.
> github
[pman] Enter New Password.
> *****
```

```
[pman] Remove Account
[pman] Enter Account name.
> reddit
[pman] Remove account reddit? (Y\n)
> Y
[pman] Account reddit removed.
```

```
[pman] Show Password
[pman] Enter Account name.
> bank
[pman] sUp3r5tr#nk!123
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
