# 🧠 C Assembler – Two-Pass Assembler Written in C

A complete two-pass assembler written entirely in C by **Hillel Suied**, designed to process `.as` source files, expand macros, resolve labels, encode instructions into binary, and generate output in Base64.

---

## 🛠 Features

- ✅ Macro expansion system (`mcro ... endmcro`)
- ✅ Two-pass assembly: label handling and resolution
- ✅ Instruction & data separation using IC / DC counters
- ✅ 16-bit binary encoding of instructions and operands
- ✅ Base64 output for `.ob` object files
- ✅ Generation of `.am`, `.ob`, `.ent`, `.ext` files
- ✅ Clean modular structure

---

## 🔄 How It Works

1. **Macro Expansion**  
   - Input: `.as` file  
   - Output: `.am` file  
   - All macros are expanded before parsing instructions.

2. **First Pass**  
   - Scans `.am` file line by line  
   - Builds symbol table and collects labels  
   - Creates instruction/data memory (IC/DC)

3. **Second Pass**  
   - Resolves labels and deferred references  
   - Encodes each line into binary using shifting and bit masks

4. **Output**  
   - Base64 encoding of 16-bit binary values  
   - Writes to `.ob`, `.ent`, and `.ext` files

---

## 📁 File Structure

| File               | Description |
|--------------------|-------------|
| `main.c`           | Entry point |
| `macro.c/h`        | Macro handling |
| `assembler.c/h`    | Two-pass logic |
| `binary.c/h`       | Instruction encoding |
| `insert.c/h`       | IC / DC list management |
| `Label.c/h`        | Label structures |
| `maybeLabel.c/h`   | Deferred label resolution |
| `printToFile.c/h`  | Output writer to Base64 |
| `base64.c/h`       | Binary → Base64 |
| `wordChecks.c/h`   | Syntax validation |
| `lineFix.c/h`      | Input line formatting |
| `Free.c/h`         | Memory deallocation |
| `makefile`         | Automated compilation |
| `test1.as`         | Sample input file |

---

## 📸 Example

**Input (`test1.as`):**
```asm
mcro M1
mov r1, r2
endmcro
M1
stop
```

**Output (`test1.ob`):**
```
104 2
QG
e/
```

---

## 🚀 Build & Run

```bash
make
./assembler test1
```

### Output Files:
- `test1.am` – After macro expansion
- `test1.ob` – Base64-encoded object file
- `test1.ent` / `test1.ext` – If entry/external labels used

---

## 🗺️ Visual Diagram

> You can include `flowchart.png` to illustrate the assembler's architecture from input to output.

---

## 👨‍💻 Author

Built from scratch by **Hillel Suied**  
Focus on memory management, bitwise operations, file handling, and clean modular architecture in C.

---

## 📜 License

MIT – Free to use and modify with attribution.
