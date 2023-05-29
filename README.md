# HRCM Bioinformatics 1 project

This is a project from the course of [Bioinformatics 1](https://www.fer.unizg.hr/predmet/bio1) in the Faculty of Electrical Engineering and Computing ([FER](www.fer.unizg.hr)).<br />
The goal of this project is to implement the Hybrid Referential Compression Method based on the [original article](https://www.hindawi.com/journals/bmri/2019/3108950/). The repository for the original article can be found [here](https://github.com/haicy/HRCM). <br />
Testing results of our implementation can be found in the [documentation file](https://github.com/laragrr/hrcm/blob/main/documentation.pdf).

# Authors:
[Paulo Erak](https://github.com/Sleepwalker2001)<br />
[Lara Grgurić](https://github.com/laragrr)

# Compile
You can find and download the source code [here](https://github.com/laragrr/hrcm).

# System requirements:
[GNU Make](https://www.gnu.org/software/make/)<br />
[7zip](https://www.7-zip.org/)

# Compiling:
Run 'make' in the source directory

# Usage:
Compression:<br />
compress.exe -r {ref_file_path} -f {tbc_file_path}* <br />
   * -r is the reference, {ref_file_path} is the path to the reference file, required <br />
   * -f is the to-be-compressed file, multiple {tbc_file_path} paths can be set, required at least one to-be-compressed file <br />

Decompression: <br />
7z e output.7a -y <br />
decompress.exe {ref_file_path} output.fa <br />
   * {ref_file_path} is the path to the reference file, required <br />

# Example:
Compression of files e_coli_103_2.fa, e_coli_103_3.fa and e_coli_103_4.fa with e_coli_103_1.fa as reference file: <br />
`compress.exe -r e_coli_103_1.fa -f e_coli_103_2.fa e_coli_103_3.fa e_coli_103_4.fa`<br />

Decompression of files e_coli_103_2.fa, e_coli_103_3.fa and e_coli_103_4.fa with e_coli_103_1.fa as reference file: <br />
`7z e output.7a -y`<br />
`decompress.exe e_coli_103_1.fa output.fa`
