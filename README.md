# InOS Package Manager (inp)

In Package Manager (inp) is a simple package manager for InOS. It can create, 
remove, install, and even recreate package. InP bundle the project binary, 
include, and other app requirements in one tarball and Inp will copy the content
to root directory. It will be danger sometimes but we keep the simplicity to
our application.

Since InOS is based on LFS, we recommends you to compile the application from
source or using default online repository for security purpose.

Usage (Root):

1. To install package from repository:
```bash
inp install gcc
```

2. To install package from file (.inp):
```bash
inp install ./gcc.inp
```

3. To install package with minimal GUI interface (ncurses):
```
inpg install gcc # Online
inpg install ./gcc.inp # Local
```

Packages are built using this structure:

| Parts               | Description |
| -----------         | ----------- |
| package.tar.gz      | Package contents (in FHS) |
| preinstall.sh       | Pre-install script. Run before copying files |
| postinstall.sh      | Post-install script. Run after copying files |
| meta-data.json      | Meta information for package like project name, author, license details, etc. |
| LICENSE             | License content        |
| CONTENTS            | Package contents. Needed for uninstall process. Created automatically after package creation |