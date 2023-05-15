import subprocess 
import pandas as pd
import sys

# res = subprocess.run(['mpicc','-o', 'hotpotato' , './hotpotato.c'],  stdout=subprocess.PIPE)
# a = subprocess.run(['mpirun','-np', '2', 'hotpotato', '0' ,'512', '32'],  stdout=subprocess.PIPE ,)
# if sys.version_info[0] < 3: 
#     from StringIO import StringIO
# else:
#     from io import StringIO

# b = StringIO(a.stdout.decode('utf-8'))
# print("read")
# df = pd.read_csv("hotpotato_output_in_text.txt", sep="\t")
# df.to_excel("hotpotato_output.xlsx")


# res = subprocess.run(['mpicc','-o', 'hotpotato' , './hotpotato.c'],  stdout=subprocess.PIPE)
# a = subprocess.run(['mpirun','-np', '2', 'hotpotato', '1024' ,'128000', '1024'],  stdout=subprocess.PIPE ,)
# if sys.version_info[0] < 3: 
#     from StringIO import StringIO
# else:
#     from io import StringIO
# print("read")
# b = StringIO(a.stdout.decode('utf-8'))

df = pd.read_csv("hotpotato_output_in_text_large.txt", sep="\t")
df.to_excel("hotpotato_output_2.xlsx")
print("2nd done")