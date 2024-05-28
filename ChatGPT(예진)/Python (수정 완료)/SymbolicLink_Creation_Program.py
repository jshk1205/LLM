import os
import shutil


def create_symbolic_link():
    source_file = input("Enter the path of the file you want to copy: ")
    if not os.path.exists(source_file):
        print("The specified file does not exist.")
        return

    target_directory = input("Enter the directory where you want to copy the file: ")
    if not os.path.isdir(target_directory):
        print("The specified directory does not exist.")
        return

    try:
        # Copy files
        filename = os.path.basename(source_file)
        shutil.copy(source_file, os.path.join(target_directory, filename))
        print(f"File copied to {os.path.join(target_directory, filename)}")

        # Creating a Symbolic Link
        link_name = input("Enter the name for the symbolic link: ")
        os.symlink(os.path.join(target_directory, filename), link_name)
        print(f"Symbolic link created from {link_name} to {os.path.join(target_directory, filename)}")
    except OSError as e:
        print(f"Failed to create symbolic link: {e}")


# Examples of Use
create_symbolic_link()
