# DFA Predecessor

## 注意事項
AIにも書かせているので間違いがある可能性があります。

## 動作環境
推奨する動作環境は、Ubuntu（秋元研究室の一番大きい黒いデスクトップパソコン）です。

## 概要
主に単体の更新過程（べき分布）のDFAをシミュレーションしたり、計算値と合わせたりグラフを作成するコードです。
superpositionのコードもprojectsの中にいくつかあります。

## 重要なファイル
シミュレーションの追試で重要なのは以下のファイルです：

### dfa_fa6_equ.cpp
- コンパイル方法: `g++ dfa_fa6_equ.cpp -o dfa_fa6_equ.exe`
- 注意点: unsigned intの最大値が10^9程度なので、それを超えないように注意が必要です。

### power_law_tool_equ.hpp
- 特徴: 0~1の一様分布の生成に特殊な方法（Xorgshift）を使用。コード作成者の私も理解してないですが、ubuntuでは高速に動き、細かい一様分布が生成できる(0が出る確率がめちゃくちゃ低い)ので使っています。

- 動作環境の制限: 以下の環境でのみ高速動作が期待できます
  - Ubuntu
  - WSL
  - Cygwin
  - Mac
- Windows の g++ では処理が遅くなる可能性があります

## Git Cloneの使い方
ここにあるファイル、フォルダを全部コピーするときは以下のコマンドを実行してください。
```
git clone https://github.com/Jika-1366/DFA_Predecessor.git
```




## Superposition関連
superpositionの解析を行う場合は、以下のディレクトリを参照してください：

- `split_superposition`: superpositionの解析用
- `superposition_distribution`: べき分布の更新過程の100個のsuperpositionがポアソン過程になっているか確認するためのコード
