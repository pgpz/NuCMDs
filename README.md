<h1 align="center"> NuCMDs - Linux-Style Command Line for PowerShell written in C</h1>

<p align="center">
  <img src="https://img.shields.io/badge/Status-Alpha-red" alt="status">
  <img src="https://img.shields.io/badge/Platform-Windows-blue" alt="platform">
  <img src="https://img.shields.io/badge/License-MIT-green" alt="license">
  <img src="https://img.shields.io/github/languages/top/pgpz/NuCMDs" alt="languages">
</p>

<p align="center">
  <b>fully configurable Linux-style command-line for PowerShell</b><br>
  50+ custom commands • embedded text editors • weekly updates • runs system wide
</p>

---

## 🔥 Features

-  **50+ Custom Commands** and growing. . .
-  **Two terminal-based text editors:**
  - `locust` (inspired by **Vim**)
  - `cicada` (inspired by **Nano**)
-  **Configuration:** Refer to config.c
-  **Support:** Works anywhere on Windows 10 & 11.
-  **Open Source:** Free to use & distribute with credit to `pgp` or `IPN`.

---

##  Developers

<p align="center">
  <a href="https://github.com/pgpz" target="_blank">
    <img src="https://avatars.githubusercontent.com/u/00000000?v=4" width="80" alt="pgp" style="border-radius: 50%;" />
    <br><sub><b>pgp</b></sub>
  </a>

> you are free to contribute this!!! 

---

##  Installation

> Click installer.exe and choose your path with the .exes you downloaded. This script will make your system path recognize NuCMDs commands.


## GCC Method (very time consuming lol..)
> grab the config.c file and compile each command using the format `gcc config.c -o (commandname).exe -D(CMDNAME)` (please note that certain commands require a flag after this.)

###  Python Installer (Untested in VMs)

```bash
python installer.py
