# 触覚と3次元視覚を備えたロボットアームの制御用RTコンポーネント

大阪電気通信大学  
澤崎 悠太，升谷 保博  
2019年11月25日

## はじめに

- 触覚と3次元視覚を備えたロボットアームの制御用RTコンポーネントです．ロボットプログラミングの教材として，RTミドルウェアの知識のない受講者が，このコンポーネントの一部のコードを書いて実習します．
- 以下のハードウェアを利用することを想定しています．
  - ロボットアーム： Mikata Arm（Dynamixel XM430 5個利用，4関節＋グリッパ）
  - 触覚： ロボットアームのグリップにFingerVisionを装着
  - 3次元視覚： Microsoft Kinect v2
- RTミドルウェアコンテスト2019応募作品の中心となるRTコンポーネントです．
- 以下の環境で開発，動作確認しています．
  - Windows 10 64bit版
  - Visual Studio 2015
  - OpenRTM-aist 1.2.0 64bit版

## 構成

- 受講者が自由に編集するソースファイルを全体から分離しました．practice.cpp内で#includeするファイルを変更することによってソースファイルを切り替えます．これによって，コンポーネント名は同じままで，様々なコードを試すことができます．
- このドキュメントでは，説明のため#includeする対象のファイルをexampleX.cppと書くことにします．その具体例として，example0.cppとexample5.cppを含めています．
- exampleX.cppには，必ず二つの関数setup()とloop()を含めなければいけません．setup()はコンポーネントのonActtivated()から，loop()はコンポーネントのonExecute()から呼び出されます．
- setup()とloop()は，コンポーネントのクラスのメンバ関数ですが，#defineによってそのように見えないようにしています．
- exampleX.cppでは，他のコンポーネントからの入力情報は，クラスのメンバ変数を介して得ます．
- exampleX.cppから他のコンポーネントへの出力はロボットアームへの指令だけです．これは，メンバ関数を介して送出します．
- loop()は毎回終了しますので，受講者のプログラム内で保持しておきたい情報は，exampleX.cppのグローバル変数にすることを想定しています．

## 処理の概要

- アクティベート時にexampleX.cppのsetup()を呼び出します．
- 他のコンポーネントから以下のような入力を得ます．
  - ハンドの左右のグリッパに取り付けられた触覚センサFingerVisionから得られた画像データは，FingerVisionCoreコンポーネントによってセンサから得られた各マーカーの座標，サイズ，変動後のマーカー座標，サイズと，オブジェクト空間モーメント，中心モーメント等に変換されます．それを受けた二つのFingerVisionFilterコンポーネントが，センサ部分に掛かった力のx,y,z成分と平均，トルクのx,y,z成分の平均や，オブジェクトの位置，向き，中心位置等に変換します．それをこのコンポーネントで利用します．
  - 3次元センサKinect v2から得られたカラー画像と深度画像は，まずKinect v2用のコンポーネント Kinect2ToPC から出力された色付き点群データを出力して，それをPCToColorCylinderを介して受け取ることで，基本座標系に変換された単体または複数の円柱の座標，半径，色相の情報を得られる．このコンポーネントでは単体または複数の円柱情報を利用して，手先位置指令を確定する．
  - ロボットアームを構成するアクチュエータの状態は，MikataArmWrapperからの値で判断しており，値にはビットごとに意味を持たせている．このコンポーネントではアームが動作中に得られる値のみを利用している．
- 得られた入力は，以下のような形でexampleX.cppに提供されます．
  - グリッパの力覚
    - double FvForceL : 力の平均z成分(左)の値を保持する
    - double FvForceR : 力の平均z成分(右)の値を保持する
  - 円柱の情報
    - 構造体 Cylinder のオブジェクト vector<Cylinder>cylinderData : 入力された単体もしくは複数の円柱の情報(x,y,z,radius,hue)を格納します．
  - アームの状態
    - bool statusMoving : Dynamixelが1つでも動作中なら1,停止中は0が格納されています．
    - bool statusExecuting : アームの動作中は1,停止中は0が格納されています．
    - bool CommandError : 上位からの指令が不適切(逆運動学が解けない等)ならば1，それ以外ならば0が格納されています．
    - bool LowerError : 解のDynamixelが一つでもエラーならば1，それ以外ならば0が格納されています．
  - アームの手先位置
    - 構造体 ArmTip のオブジェクト armTip (x,y,z,pitch,hand)
- 新たな入力がある度にexampleX.cppのloop()を呼び出します．
- exampleX.cppでは，ロボットアームへの指令を送出するために以下の関数を利用します．
  - void setArmTipTarget(ArmTip target)  
構造体ArmTipはアームの手先位置の入力にも使用していますが，手先位置指令の出力には座標，pitch，ハンドの開閉に加えて，動作時間[sec]を含めて設定してください．アームの全ての軸が指定した同じ時間で動作するようになっています．
- 手先の位置指令が新たに設定されると，armTipTargetへデータを出力します．

## example0.cpp

- 受講者のファイルのテンプレートです．

## example5.cpp

- この教材のハードウェアを全て使ったプログラムの例です．
- 机の上に置かれた円柱物体をロボットアームで把持し，その色に応じて指定の場所に移動させます．
- 状態遷移を利用して動作を切り替えています．
- 各状態の概要：
  - Recognize: 受け取った円柱の座標と色情報から手先位置指令を設定して，円柱が複数ある場合は一番近い円柱情報を使用する．円柱が届かない場所にある場合は設定しない．
  - PreApproach: 円柱がある手前までアームを移動させる．
  - Approach: 円柱までアームを移動させる．
  - PreGrasp: グリッパを途中まで閉じさせる．次の状態に遷移する前にFingerVisionのセンサ部分に掛かっている力を記憶する．
  - Grasp: グリッパを完全に閉じさせる．閉じている最中にFingerVisionのセンサ部分に掛かっている力を見て，PreGrasp時に記憶しているFingerVisionに掛かる力の値と比較して，一定以上の値に差が開くとその時のグリッパの閉じ具合を保持したまま次の状態に遷移する．
  - Bring: 円柱を掴んだまま手前に引き寄せる．
  - Carry: 円柱を指定の場所に運搬する．円柱の色によって置く場所を変える．
  - Release: グリッパを開いて円柱を離す．
  - Initialize: 待機の姿勢をとる． 

## 仕様

### 入力ポート
- fv_filter0_wrench
  - 型: FingerVision::Filter1Wrench
  - 概要： 右(もしくは左)グリッパのFingerVisionからの全ブロブ(マーカー)のx,y,z座標，力のx,y,z成分と力のx,y,z成分の平均，トルクのx,y,z成分の平均
- fv_filter0_objinfo
  - 型: FingerVision::Filter1ObjInfo
  - 概要： 右(もしくは左)グリッパのFingerVisionからのオブジェクト中心x,y,zやオブジェクトの傾き等
- fv_filter1_wrench
  - 型: FingerVision::Filter1Wrench
  - 概要： 左(もしくは右)グリッパのFingerVisionからの全ブロブ(マーカー)のx,y,z座標，力のx,y,z成分と力のx,y,z成分の平均，トルクのx,y,z成分の平均
- fv_filter1_objinfo
  - 型: FingerVision::Filter1ObjInfo
  - 概要： 左(もしくは右)グリッパのFingerVisionからのオブジェクト中心x,y,zやオブジェクトの傾き等
- cylinder
  - 型: RTC::TimedDoubleSeq
  - 概要： 1から3個の円柱情報の値．中心のx,y,z座標[m], 半径[m], 色相(0～360)(要素数5)
- armTip
  - 型: RTC::TimedDoubleSeq
  - 概要： 現在のアームの手先位置の値(x[m], y[m], z[m], ピッチ角[rad], ハンドの開閉(閉:0.0 ～ 開:1.0))の値(要素数5)
- armStatus
  - 型: RTC::TimedDoubleSeq
  - 概要： アームの状態を表す値．ビットごとに意味を持たせています．
    - bit0: 下位のDynamixelが一つでもmovingならば1
    - bit1: アームが動作中ならば1(動き始めの時間差を考慮)
    - bit2: 上位からの指令が不適切(逆運動学が解けない等)ならば1
    - bit3: 解のDynamixelが一つでもエラーならば1

### 出力ポート

- armTipTarget
  - 型: RTC::TimedDoubleSeq
  - 概要： ロボットアームの手先位置指令．(x[m], y[m], z[m], ピッチ角[rad], ハンドの開閉(閉:0.0 ～ 開:1.0),動作時間[sec])

### コンフィギュレーション

なし

## インストール

- [OpenRTM-aist 1.2.0](https://www.openrtm.org/openrtm/ja/download/openrtm-aist-cpp/openrtm-aist-cpp_1_2_0_release)をインストール．
- [GitHubのMikataArmControllerのリポジトリ](https://github.com/MasutaniLab/MikataArmController)をクローンかダウンロードする．
- CMake
  - ビルドディレクトリはトップ直下の`build`
  - ConfigureはVisual Studio 64bit
- `build\MikataArmController.sln`をVisual Studioで開く．
- ビルド

## 使い方

- Naming Serviceを起動しておきます．
- トップディレクトリの[MikataArmController.bat](MikataArmController.bat)を実行．

## 既知の問題・TODO

- 入力のarmStatusの値は，実際のアームの状態と比べるとラグがあります．このコンポーネントでは手先位置指令を出力後に1[sec]未満はbit1が強制的に真になるように考慮しています．
- 部屋の明るさや温度差でFingerVisionのキャリブレーションがずれる場合があります．FingerVisionを用いた把持に影響が出る可能性があるので，使用前はキャリブレーションの実行を推奨します．
- 現時点ではFingerVisionのfv_filter0_wrenchとfv_filter1_wrenchの一部の情報のみ使用しています．追加の課題としては，円柱運搬中に円柱が滑って落ちそうになった場合にfv_filter0_objinfoの情報を活用して落とさないようにする機能などを追加する等が考えられます．