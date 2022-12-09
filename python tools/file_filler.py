"""Algorithm to deal with file handling"""


PATH2WRITEFILE = "./out.txt"
PATH2READFILE = "./input.txt"


def clear_file_content(filename: str) -> None:
    """Function to clear content of a specified file
    Parameters:
    ------------
        filename : str
            path to file to be cleaned
    Returns:
    ------------
        None
    """
    with open(file=filename, mode="w", encoding="utf-8") as file:
        file.close()


def populate_file_from_file(input_file: str, output_file: str) -> None:
    """Function to write content of input file to output file
    Parameters:
    -----------
        input_file : str
            path to input file where content is locate
        output_file : str
            path to output file where to write content of input file
            Initially empy
    Returns:
    -----------
        None
    """
    # opening first file in append mode and second file in read mode
    with open(file=output_file, mode="a+", encoding="utf-8") as f_out:
        with open(file=input_file, mode="r", encoding="utf-8") as f_in:
            # appending the contents of the second file to the first file
            f_out.write(f_in.read())

            # relocating the cursor of the files at the beginning
            f_out.seek(0)
            f_in.seek(0)

            # closing the files
            f_out.close()
            f_in.close()


if __name__ == "__main__":
    answ = input("Do you want a new fresh output file?")
    if answ == "yes":
        clear_file_content(filename=PATH2WRITEFILE)

    populate_file_from_file(
        input_file=PATH2READFILE, output_file=PATH2WRITEFILE
    )
