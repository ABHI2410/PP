import subprocess 
import plotly.express as px
import pandas as pd
import sys

res = subprocess.run(['mpicc','-o', 'pingpong' , './pingpong.c'],  stdout=subprocess.PIPE)
a = subprocess.run(['mpirun','-np', '2', 'pingpong', '0' ,'512', '32'],  stdout=subprocess.PIPE ,)
if sys.version_info[0] < 3: 
    from StringIO import StringIO
else:
    from io import StringIO

b = StringIO(a.stdout.decode('utf-8'))

df = pd.read_csv(b, sep="\t")
df.to_excel("pingpong_output.xlsx")


res = subprocess.run(['mpicc','-o', 'pingpong' , './pingpong.c'],  stdout=subprocess.PIPE)
a = subprocess.run(['mpirun','-np', '2', 'pingpong', '1024' ,'128000', '1024'],  stdout=subprocess.PIPE ,)
if sys.version_info[0] < 3: 
    from StringIO import StringIO
else:
    from io import StringIO

b = StringIO(a.stdout.decode('utf-8'))

df = pd.read_csv(b, sep="\t")
df.to_excel("pingpong_output_2.xlsx")
