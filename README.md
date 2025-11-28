# 📁 File System Explorer (C + Linux)

A mini Linux file manager built in C using system calls like `opendir()`, `readdir()`, `stat()`, and directory traversal.  
This project simulates the behavior of tools like **ls**, **tree**, **du**, and **find**.

---

## ⭐ Features
✔ List files in a directory (like `ls`)  
✔ Display file size, permissions, and type  
✔ Recursively show directory structure (like `tree`)  
✔ Compute folder size (like `du`)  
✔ Search files by name (like `find`)  
✔ Works on Linux & WSL  

---

## 🛠️ Technologies Used
- **Language:** C  
- **Platform:** Linux / WSL  
- **System Calls:** `opendir`, `readdir`, `stat`, recursive traversal  

---

## 🏗️ Build Instructions

```bash
gcc file_explorer.c -o file_explorer
