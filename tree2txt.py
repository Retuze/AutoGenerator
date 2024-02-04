import os
if os.path.exists("tree.txt"):
    os.remove("tree.txt")
os.system("tree /f >>tree.txt")