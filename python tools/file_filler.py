PATH2WRITEFILE = "./out.txt"
PATH2READFILE = "./input.txt"

# opening first file in append mode and second file in read mode
output_file = open(firstfile, 'a+')
input_file = open(secondfile, 'r')

# appending the contents of the second file to the first file
f1.write(f2.read())

# relocating the cursor of the files at the beginning
f1.seek(0)
f2.seek(0)

# closing the files
f1.close()
f2.close()


