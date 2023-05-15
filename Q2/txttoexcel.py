import pandas as pd 
df = pd.read_csv("simpsons_out_1k.txt", sep="\t", header=None)
print (df.iloc[1])
df.to_excel("simpsons_out1.xlsx")

df = pd.read_csv("simpsons_out_10k.txt", sep="\t", header=None)
df.to_excel("simpsons_out2.xlsx")

df = pd.read_csv("simpsons_out_100k.txt", sep="\t", header=None)
df.to_excel("simpsons_out3.xlsx")

df = pd.read_csv("simpsons_out_1M.txt", sep="\t", header=None)
df.to_excel("simpsons_out4.xlsx")

df = pd.read_csv("simpsons_out_10M.txt", sep="\t", header=None)
df.to_excel("simpsons_out5.xlsx")

df = pd.read_csv("simpsons_out_100M.txt", sep="\t", header=None)
df.to_excel("simpsons_out6.xlsx")
