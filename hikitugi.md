# B4実験NaI

[TOC]

# エネルギーキャリブレーション

## 測定器A (プラしん)

### Na

- AとCでcoincidenceを取った
  - 15分ぐらいで12000 events
- error functionでfittingした
- Cのgaussianのfitting結果をerror functionの初期値に使った
- data
  - Na_run01, 02, 03

![A_Na](.img/A_Na.svg)

### Cs

- コインし取ってない
- 部屋を暗くして測定した
- 数分で10000 events
- Error functionでfittingした　きれい
- data
  - A_Cs_1900V_24mV_dark

![A_Cs](.img/A_Cs.svg)

### Co

- AとCでコインし取った
- この検出器は2つのcompton edgeを分離できない程度の分解能
- Coの崩壊の原理より, 光電ピークによって2つのcompton edgeを分離した
- data
  - Co_run01, 02
  - 27時間ぐらい測定した

<img src=".img/A_Co_CE.svg" alt="A_Co_CE" style="zoom:67%;" /> <img src=".img/A_Co_PE.svg" alt="A_Co_PE" style="zoom:67%;" />

<img src=".img/A_Co_CE_left.svg" alt="A_Co_CE_left" style="zoom:67%;" /> <img src=".img/A_Co_CE_right.svg" alt="A_Co_CE_right" style="zoom:67%;" />

### 直線

- 切片が大きい
- 手作りであることが原因と考えられる
- 波形データの積分をいじるとよくなるかも

![A_clb](.img/A_clb.svg)

## 測定器B (NaI)

- どの線源に関してもcoincidenceを取ってないので測定時間は数分

### Na

![](.img/B_Na.svg)



### Cs

![](.img/B_Cs.svg)

### Co

![](.img/B_Co.svg)

### 直線

![B_calb_liner]()

## 測定器C (Nal)

どの線源に関してもcoincidenceを取ってないので測定時間は数分

### Na

![](.img/C_Na.svg)

### Cs

![](.img/C_Cs.svg)

### Co

![](.img/C_Co.svg)

### 直線

![B_calb_liner]()

# 本測定

- NaをA,B,Cのcoincidenceで測定

## A 1次元hist

## B 1次元hist

## C 1次元hist

## AB 2次元hist

## BC 2次元hist

## CA 2次元hist

## ABC 3次元hist





# TODO

- 本測定の結果

- Cで511kevのところだけ抜き出す
