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

## Feature List
- [ ] Saving password to file with account name
- [ ] Removing password with account name
- [ ] Opening & Displaying password from file from account name
- [ ] Displaying all account names
- [ ] XOR Encryption / Decryption
