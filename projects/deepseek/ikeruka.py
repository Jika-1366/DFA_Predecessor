import sympy
from sympy import gamma

def calculate_term(k, t, d, alpha):
    return  (t**(2+k-alpha) * d**(1-k))/(gamma(3+k-alpha) * gamma(2-k))

t_val = 10**3 +1212
d_val = 10**3 +52627
alpha_val = 1

terms = []
for k in range(101):
    term_k = calculate_term(k, t_val, d_val, alpha_val)
    terms.append(term_k)
    print(f"k={k}, term={term_k}")

print("\n項が少しずつ小さくなっているか確認してください。")
print("k=2以降の項はガンマ関数Gamma(2-k)が特異点を持つため、無限大または不定となります。")
print("sympy は複素無限大 (zoo, -zoo) で表します。")
print("したがって、級数は発散する可能性が高いです。")
print("k=0, 1の項は有限の値ですが、k=2以降の項は無限大に発散するため、")
print("項数を増やしても級数の値は収束しません。")
print("もし項が小さくなっている部分があるか確認したい場合は、")
print("上記の出力を確認して、項の値の変化を見てください。")