import subprocess
import csv
from itertools import product

cpp_program = "./dfa_fa2"
python_path = "C:/Users/user/anaconda3/envs/openinterpreter/python.exe"
python_script = "c:/cygwin64/home/user/akimotoken/Predecessor/graph_alpha_slope_all.py"

def run_analysis(tau_0, sample_amount, number_i, t_first_l):
    args = [
        "--tau_0", str(tau_0),
        "--sample_amount", str(sample_amount),
        "--number_i", str(number_i),
        "--t_first_l", str(t_first_l)
    ]

    subprocess.run([cpp_program] + args, check=True)

    try:
        result = subprocess.run(
            [python_path, python_script],
            capture_output=True, text=True, check=True, timeout=60
        )
        last_line = result.stdout.strip().split('\n')[-1]
        print(f"graph_alpha_slope_all.pyの最後の出力: {last_line}")
        return last_line
    except subprocess.TimeoutExpired:
        print("graph_alpha_slope_all.pyの実行がタイムアウトしました。")
        return "Timeout"
    except subprocess.CalledProcessError as e:
        print(f"エラーが発生しました: {e}")
        print(f"標準エラー出力:\n{e.stderr}")
        return f"Error: {e}"

def main():
    # パラメータの範囲を定義
    tau_0_range = [0.5, 1.0, 1.5]
    sample_amount_range = [10**4]
    number_i_range = [3]
    t_first_l_range = [16]

    results = []

    for tau_0, sample_amount, number_i, t_first_l in product(tau_0_range, sample_amount_range, number_i_range, t_first_l_range):
        print(f"Running with parameters: tau_0={tau_0}, sample_amount={sample_amount}, number_i={number_i}, t_first_l={t_first_l}")
        
        slope = run_analysis(tau_0, sample_amount, number_i, t_first_l)
        
 
        
        result = {
            "tau_0": tau_0,
            "sample_amount": sample_amount,
            "number_i": number_i,
            "t_first_l": t_first_l,
            "slope": slope
        }
        results.append(result)

    # 結果をCSVファイルに保存
    csv_filename = "dfa_results.csv"
    with open(csv_filename, 'w', newline='') as csvfile:
        fieldnames = ["tau_0", "sample_amount", "number_i", "t_first_l", "slope"]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for result in results:
            writer.writerow(result)

    print(f"Results saved to {csv_filename}")

if __name__ == "__main__":
    main()