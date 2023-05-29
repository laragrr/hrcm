CC = g++
CFLAG = -O3 -march=native -Wall -std=c++17
objects = combined.cpp

lngc:
	$(CC) combined.cpp -o compress $(CFLAG)
	$(CC) decompression.cpp -o decompress $(CFLAG)


clean:
	del *.o

# compress.exe -r e_coli_103_1.fa -f e_coli_103_2.fa e_coli_103_3.fa e_coli_103_4.fa
# 7z e output.7a -y
# decompress.exe e_coli_103_1.fa output.fa