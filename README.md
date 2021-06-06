# b4ex_NaI
2021年度B4実験 テーマ:NaI

## Usage
- cloneする
```
[usr@local ~]$ git clone https://github.com/hmdyt/b4ex_NaI
```

- data 以下に実験データを置く
```
[usr@local b4ex_NaI]$ cp -r /path/to/data .
```

- ./ (cloneしたフォルダの一番上)でrootを実行
```
[usr@local b4ex_NaI] root -l
```

- 任意の関数を実行
```
root [0] Na_AB_th2()
```

## data
<pre>
data
 |-- day1
 |-- day2
 |-- day3
 ...
</pre>
実験データを格納する。

## src
<pre>
src
 |-- calibration
 |-- main_mesurement
</pre>
ROOTのマクロを格納する。

### calibration
- キャリブレーションに用いるマクロを格納する
- ADC-Energy 直線のパラメータを返すところまでをキャリブレーションに用いる関数とする
- github導入前に書いたキメラがある

### main_mesurement
- 本測定の解析に用いるマクロが入っている
 
## rootlogon.C
- root起動時に読み込むマクロを列挙している
- 新しいマクロを追加するときに登録することを忘れるな

