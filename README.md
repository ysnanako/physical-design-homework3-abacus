# physical-design-homework3-abacus

![GitHub repo size](https://img.shields.io/github/repo-size/ysnanako/physical-design-homework3-abacus)
![GitHub last commit](https://img.shields.io/github/last-commit/ysnanako/physical-design-homework3-abacus)

This project is part of the **National Cheng Kung University (NCKU) - VLSI/CAD Group** course **"Physical Design for Nanometer IC"**, focusing on **Legalization** using the **Abacus algorithm**. The program reads **global placement results** and adjusts **cell positions** to eliminate overlaps while minimizing **cell displacement**.

## 📖 Table of Contents

- [Project Overview](#project-overview)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Project Structure](#project-structure)
- [Parsing & Placement Flow](#parsing--placement-flow)
- [Example Execution](#example-execution)
- [Validation & HPWL Calculation](#validation--hpwl-calculation)
- [Generated Plots](#generated-plots)
- [Contribution Guide](#contribution-guide)
- [Contact Information](#contact-information)

## 📝 Project Overview

Modern placement tools consist of **three main steps**:

1. **Global Placement** - Determines **approximate** cell locations.
2. **Legalization** - Ensures **valid cell placement** while keeping the design compact.
3. **Detailed Placement** - Fine-tunes **wirelength optimization**.

This project implements **legalization** using the **Abacus algorithm**, which:

- **Minimizes cell displacement** from global placement.
- **Removes cell overlaps** by aligning them to legal sites.
- **Preserves placement density**.

The program **parses `.gp.pl` (global placement result)**, processes cell movements, and outputs a **legal placement file**.

## 📄 Input Format

This project follows the **Bookshelf** format and requires the following files:

- **`.aux`** - Main index file pointing to `.nodes`, `.nets`, `.gp.pl`, `.scl`, etc.
- **`.nodes`** - Describes circuit components and their dimensions.
- **`.nets`** - Defines netlist connectivity.
- **`.gp.pl`** - **Global placement** results.
- **`.scl`** - Defines row placement structure.

📄 **Example `.aux` File**

```
RowBasedPlacement : circuit.nodes circuit.nets circuit.gp.pl circuit.scl
```

## 📄 Output Format

After execution, the program generates:

- **`.legal.pl`** - Legalized placement file.
- **`.dat`** files - Stores parsed circuit information.
- **`.plt`** files - Gnuplot scripts for visualization.

## 🧰 Project Structure

```
📂 physical-design-homework3-abacus/
│── 📂 src/ # (main.cpp, parser.cpp, abacus.cpp, and headers)
│── 📂 obj/ # (ignored)
│── 📂 plt/ # (ignored in Git, automatically generated)
│── 📂 dat/ # (ignored in Git due to large size, automatically generated)
│── 📂 legal/ # (ignored in Git due to large size, automatically generated)
│── 🖥️ HW3_StudentID # (ignored)
│── 🔧 Makefile
│── 📜 README.md # This file
│── 📜 .gitignore

../📂 benchmarks/ # (located outside the repo)
```

## 🔹 **Parsing & Placement Flow**

This project implements **legalization** using the **Abacus algorithm**, a dynamic programming approach that efficiently places standard cells while preserving relative positioning from global placement. The steps are as follows:

### **1. Read Input Files**

- Parse the `.aux` file to obtain the `.nodes`, `.nets`, `.gp.pl`, and `.scl` files.
- Extract **cell dimensions**, **net connections**, and **initial placement positions**.

### **2. Build Data Structures**

- Construct **MODULES**, **NETS**, **PADS**, and **ROWS** data structures.
- Store cell information, connectivity, and row details for placement adjustments.

### **3. Apply the Abacus Algorithm**

- **Sort cells by x-coordinates**: Cells are processed from **left to right** to maintain spatial consistency.
- **Row-based placement**: Each cell is assigned to the closest legal row.
- **Sliding Window Method**:
  - Identify **legal placement sites** using row-based constraints.
  - Assign **minimum displacement** positions for each cell while avoiding overlaps.
  - If a cell overlaps, it is pushed to the nearest **available site**.
- **Row Shifting & Densification**:
  - Adjust cell alignment to ensure **compact placement**.
  - Distribute whitespace efficiently to balance density.

### **4. Output Generation**

- **Generate `.legal.pl` file** with the new legalized positions.
- **Store placement results in `.dat` and `.plt` files** for visualization.

### **5. Validation & HPWL Calculation**

- **Check legality** of placement using `iccad2013_check_legality`.
- **Compute HPWL** to measure placement efficiency using `iccad2013_get_hpwl`.

## ⚡ **Example Execution**

```bash
./HW3_StudentID ../benchmarks/superblue1/superblue1.aux
```

```bash
gnuplot superblue1/superblue1.plt
```

```bash
./iccad2013_check_legality superblue1.aux superblue1.legal.pl
```

```bash
./iccad2013_get_hpwl superblue1.aux superblue1.legal.pl
```

## ✅ Validation & HPWL Calculation

To validate legalization correctness and measure **Half-Perimeter Wirelength (HPWL)**, the following **ICCAD 2013 Checkers** are provided. These tools ensure the legality of the placement and compute the wirelength for performance evaluation.

### **Legality Check**

```bash
./iccad2013_check_legality superblue1.aux superblue1.legal.pl
```

- Ensures that all cells are legally placed without overlaps.

### **HPWL Calculation**

```bash
./iccad2013_get_hpwl superblue1.aux superblue1.legal.pl
```

- Computes **Half-Perimeter Wirelength (HPWL)** for placement evaluation.

## 🖼️ Generated Plots

Below are the generated plots from the `gnuplot` output:

**superblue1 row**  
![superblue1_row](https://github.com/user-attachments/assets/0a4e474c-1902-462e-86cb-83aa7ba7b1e9)  
**superblue1 init**  
![superblue1_init](https://github.com/user-attachments/assets/ddd280f1-e584-4ec1-b7e1-fc754b2f3243)  
**superblue1 legal**  
![superblue1](https://github.com/user-attachments/assets/0f6c2a63-cbce-4eee-82ce-d1410df6f052)  

## 🤝 Contribution Guide

1. Fork this repository.
2. Create a new branch (`git checkout -b feature-xyz`).
3. Commit your changes (`git commit -m 'Add new feature'`).
4. Push to the remote branch (`git push origin feature-xyz`).
5. Submit a Pull Request.

## 📬 Contact Information

- 📧 Email: [m16131056@gs.ncku.edu.tw](mailto\:m16131056@gs.ncku.edu.tw)
- 🌎 University: [National Cheng Kung University (NCKU)](https://www.ncku.edu.tw)
- 📖 Course: Physical Design for Nanometer IC, Fall 2024
