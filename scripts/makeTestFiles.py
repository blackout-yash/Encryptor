import os
import string
import random

def makeFiles(path):
    
    os.makedirs(path, exist_ok=True)
    os.chdir(path)
    for i in range(1000):
        print(i)
        filename = f"test{i+1}.txt"
        print(filename)
        with open(filename, "w") as file:
            random_string = ''.join(random.choices(string.ascii_uppercase + string.digits, k=1000))
            file.write(random_string)

if __name__ == "__main__":
    makeFiles("./test")