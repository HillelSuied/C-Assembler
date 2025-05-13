# 🧠 C Assembler – Two-Pass Assembler Project in C

This project is a **complete two-pass assembler** written in **pure C**, developed from scratch by **Hillel Suied** as a major academic and technical systems project.

It supports:
- ✅ Macro expansion (like a C preprocessor)
- ✅ Label definition and resolution
- ✅ Instruction and data section separation
- ✅ Binary encoding of assembly instructions
- ✅ Base64 output formatting
- ✅ Output to `.ob`, `.ent`, and `.ext` files

---

## 🔍 Overview

The assembler works in multiple stages:

1. **Macro Expansion**  
   Reads `.as` files, detects and expands macros (`mcro ... endmcro`), and outputs `.am` files.

2. **First Pass**  
   - Identifies labels and macro usage
   - Builds instruction (`IC`) and data (`DC`) tables
   - Collects symbol table and deferred label references

3. **Second Pass**  
   - Resolves symbol addresses
   - Encodes instructions into 16-bit binary format

4. **Output Generation**  
   - Converts binaries to Base64 format
   - Writes `.ob` file (object code)
   - Outputs `.ent` and `.ext` if labels are present

---

## 🧠 Project Structure

| File              | Description |
|-------------------|-------------|
| `main.c`          | Entry point, manages flow |
| `macro.c/h`       | Macro parsing and expansion |
| `assembler.c/h`   | Two-pass assembler logic |
| `binary.c/h`      | Instruction-to-binary encoding |
| `insert.c/h`      | Handles IC and DC lists |
| `Label.c/h`       | Manages label structures |
| `maybeLabel.c/h`  | Deferred label resolution |
| `printToFile.c/h` | Output of object files in base64 |
| `base64.c/h`      | Binary → Base64 conversion |
| `wordChecks.c/h`  | Syntax validation and checks |
| `lineFix.c/h`     | Preprocessing & line adjustments |
| `Free.c/h`        | Memory deallocation |
| `makefile`        | Automated build system |
| `test1.as`        | Example input file |

---

## 📂 Sample Input (`test1.as`)

```asm
mcro M1
mov r1, r2
endmcro
M1
stop
```

### 🔄 Compiled With:

```bash
make
./assembler test1
```

### 📤 Output (`test1.ob`)
```
104 2
QG
e/
```

---

## 🗺️ Flowchart

> See `flowchart.png` for a full visual of how the assembler works from macros to final output.

---

## 🚀 How to Run

```bash
make
./assembler <filename-without-extension>
```

Generates:
- `<file>.am` – After macro expansion
- `<file>.ob` – Base64-encoded object file
- `<file>.ent` / `<file>.ext` – If entry/external labels exist

---

## 👨‍💻 Author

Built by **Hillel Suied**  
A full implementation from scratch of a multi-phase, low-level assembler in C.  
Includes memory management, binary encoding, file output logic, and modular design.

---

## 📜 License

MIT – Free to use and modify with attribution.
