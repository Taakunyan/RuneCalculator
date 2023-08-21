# RuneCalculator
メメントモリの戦闘力を調整するための計算ツールです。

## 使用方法
1. [ここ](https://github.com/Taakunyan/RuneCalculator/releases/)から __RuneCalcurator.exe__ と __rune.csv__ をダウンロードし、同じフォルダに保存。
2. __RuneCalculator.exe__ をダブルクリック。
3. Current Power ... 現在の戦闘力をコンマ（,）なしで入力。
4. Target Power ... 目標の戦闘力をコンマ（,）なしで入力。
5. Number of runes ... 使用するルーンの個数を入力。

### コマンドラインから実行する場合
引数は [Current Power] [Target Power] (Number of runes) です。<br>
※ [ ] は必須、( )はオプションです。 デフォルトでは Number of runes は 6 に設定されています。

## 表示
![img](cmd.png)<br>
① ルーンを装着することで上がる戦闘力<br>
② 装着するルーンのレベル<br>
③ 装着するルーンの名前 （攻撃系（左側）のルーンは赤色、防御系（右側）のルーンは緑色で表示されます。）<br>
図の場合、7500, 4350, 500  または、 9100, 3010, 180, 60  のルーンを装着すると目標の戦闘力になります。

## 注意事項
Number of runes が増えると計算量が指数関数的に増加します。小さい数から順番に実行することをお勧めします。<br>
__rune.csv__ は Shift-JIS で保存してください。 他の文字コードで保存すると文字化けします。<br>
キャラクターや、武器の強化レベルによって戦闘力が変わることがあります。

## ChangeLog
### 1.0.0 [2023/08/21]
