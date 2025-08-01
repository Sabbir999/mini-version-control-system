# Custom Version Control System (VCS) – Git Implementation in C++

## 📌 Overview
This project is a **lightweight Git-like version control system** implemented from scratch in C++. It replicates essential Git functionalities such as repository initialization, file staging, committing changes, viewing history, and reverting to previous versions — all while demonstrating efficient file system operations and custom data structures.

![Demo](https://demo.gif)

---

## ✨ Features

- ✅ **Repository Initialization**: `init` command creates a `.git` folder to track changes.
- ✅ **File Staging**: `add` command supports single file, folder, or wildcard (`.`) staging.
- ✅ **Commit Snapshots**: `commit` captures staged changes along with metadata.
- ✅ **Commit History**: `log` displays version history in reverse chronological order.
- ✅ **Revert Versions**: `revert` restores working directory to any previous commit.
- ✅ **Efficient Storage**: Uses file-based object database with deduplication and hard links.
- ✅ **Cross-Platform Support**: Works on **Windows**, **Linux**, and **macOS**.

---

## 🛠️ Installation

### **Prerequisites**
- C++17-compatible compiler (GCC 9+, Clang 10+, MSVC 19.28+)
- CMake 3.14+ *(optional)*

### **Compilation**

```bash
# Clone repository
git 
cd custom-vcs/src

# Compile (Windows)
g++ -std=c++17 -o mygit.exe main.cpp gitClass.cpp commitNodeList.cpp

# Compile (Linux/macOS)
g++ -std=c++17 -o mygit main.cpp gitClass.cpp commitNodeList.cpp -lstdc++fs


##  Usage
# Initialize a new repository
./mygit init

# Stage all files
./mygit add .

# Stage specific files or directories
./mygit add file.txt src/

# Commit staged files
./mygit commit -m "Initial commit"

# View commit history
./mygit log

# Revert to a specific commit
./mygit revert d70164da

# Revert to HEAD (most recent commit)
./mygit revert HEAD


🧠 Technical Design
🏗️ Architecture Overview
Working Directory
     |
     v
.git/
 ├── commits/
 │   └── <commit-id>/
 │       ├── Data/                # Snapshot of committed files
 │       ├── commitInfo.txt       # Metadata (ID, message, timestamp)
 │       └── nextCommitInfo.txt   # Pointer to next commit
 └── staging_area/               # Files staged before commit



🔄 Core Components
📁 Object Database
Stores commit data in .git/commits/<commit-id>/

Contains:

commitInfo.txt: Commit metadata

nextCommitInfo.txt: Pointer to next commit

Data/: Snapshot of committed files

📦 Commit Structure
class commitNode {
    string commitID;        // Unique 8-character hash
    string commitMsg;       // User-provided message
    string nextCommitID;    // Pointer to next commit
    commitNode* next;       // In-memory pointer to next node
};


Singly linked list of commit nodes

HEAD points to the latest commit

TAIL points to the initial commit

🧠 Storage Optimization
Files are only saved once per unique version

Unchanged files use hard links

Deduplication via content-addressable storage (future scope)