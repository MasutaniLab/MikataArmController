#include "practice.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Windows.h>
using namespace std;

const double ArmRange = 0.4;//アームが届く距離[m] 
double cylDistance = 0.0;      // 円柱とアームとの距離を格納する
double initialFvForceR;        //PreGrasp終了時の右FVの値を保持する
double initialFvForceL;        //PreGrasp終了時の左FVの値を保持する
double handRatioGrasp;         // [Grasp]円柱が把持ができた時にhandの閉じ具合を保持する
int contactCount;              // 接触する条件が満たされる時にこれで判定する

// 構造体「Cylinder」のオブジェクト(インスタンス) (x,y,z,radius,hue)
Cylinder cylinder;

//状態遷移
enum State { Recognize, PreApproach, Approach, PreGrasp, Grasp, Bring, Carry, Release, Initialize };
State state;
enum Color { Unknown, Red, Yellow, Green, Blue };
Color color;

// 比較関数
bool cmp(const Cylinder &a, const Cylinder &b);

void setup()
{
    state = Recognize;
    color = Unknown;

    initialFvForceR = 0.0;
    initialFvForceL = 0.0;
    handRatioGrasp = 0.3;  // 初0.3[rad]:把持時に少しだけ閉じる(回すごとに-0.03される)
    contactCount = 0;      // 接触する条件が満たされる時にこれで判定する

}

void loop()
{
    // 状態遷移
    State statePrev = state;
    if (state == Recognize) {
        if (cylinderData.size() > 0) {

            // 円柱が複数ある場合，各円柱の距離を近い順に並び替える．
            sort(cylinderData.begin(), cylinderData.end(), cmp);

            // 以降このcylinderを使う
            cylinder = cylinderData[0];
            //cylinder.disp();

            // 最も近い円柱との距離を代入
            cylDistance = sqrt(pow(cylinderData[0].x, 2) + pow(cylinderData[0].y, 2));

            // 確認用
            for (int i = 0; i < cylinderData.size(); i++)
                cout << "距離参考 昇順 cylinderData[" << i << "] = " << cylinderData[i].x << endl;
            cout << "cylDistance = " << cylDistance << endl;


            // 最も近い円柱色の取得し，colorに対応する色を格納
            if ((285 < cylinder.hue && cylinder.hue <= 360) || (0 <= cylinder.hue && cylinder.hue <= 45)) {
                cout << "円柱の色は 赤色" << endl;
                color = Red;
            }
            else if (45 < cylinder.hue && cylinder.hue <= 75) {
                cout << "円柱の色は 黄色" << endl;
                color = Yellow;
            }
            else if (75 < cylinder.hue && cylinder.hue <= 165) {
                cout << "円柱の色は 緑色" << endl;
                color = Green;
            }
            else if (165 < cylinder.hue && cylinder.hue <= 285) {
                cout << "円柱の色は 青色" << endl;
                color = Blue;
            }
            else {
                cout << "色情報の取得に失敗" << endl;
                cout << cylinder.hue << endl;
                color = Unknown;
            }

            // 円柱認識時の判断
            // 円柱の色情報と半径で判定し，armが届く範囲に円柱があればPreApproach状態へ移行        
            if (color != Unknown) {
                if (0.01 < cylinder.radius && cylinder.radius <= 0.07) {
                    cout << "円柱を見つけました" << endl;
                    // 円柱がアームが届く範囲にあるかを判定
                    if (ArmRange <= cylDistance) {
                        cout << "届きません..." << endl;
                    }
                    else {
                        cout << "把持可能です" << endl;
                        state = PreApproach;
                        cout << "Recognize -> PreApproach" << endl;
                        Beep(440, 200);
                    }
                }
                else {
                    cout << "円柱が大きすぎもしくは小さすぎる．再試行します" << endl;
                    color = Unknown;
                }
            }        
        }
    }
    else if (state == PreApproach) {   
        //Wrapperからの現在のアームの座標等を比較(m_armTip.data[0,1,2,4]→FVの重さでピッチが0.06[rad]以上ずれる)
        //アームが停止中なら状態遷移
        if (statusExecuting == false) {
            state = Approach;
            cout << "PreApproach -> Approach" << endl;
            Beep(440, 200);
        }
    }
    else if (state == Approach) {
        //Wrapperからの現在のアームの座標等を比較(m_armTip.data[0,1,2,4]→FVの重さでピッチが0.06[rad]以上ずれる)
        // zはApproach時に重みで0.03[m]以上ずれる
        //アームが停止中なら状態遷移
        if (statusExecuting == false) {
            state = PreGrasp;
            cout << "Approach -> PreGrasp" << endl;
            Beep(440, 200);
        }

    }
    else if (state == PreGrasp) {
        // //アームが停止中なら状態遷移
        if (statusExecuting == false) {
            state = Grasp;
            cout << "PreGrasp -> Grasp" << endl;
            initialFvForceL = FvForceL;   //力の平均のz成分を格納 left
            initialFvForceR = FvForceR;    //力の平均のz成分を格納 right
            Beep(440, 200);
        }
    }
    else if (state == Grasp) {

        // 事前に記憶した力の平均値と今の値を比較し，力が掛かっている(差が開く)と次の状態遷移へのカウントが始まる
        if (abs(initialFvForceL - FvForceL) > 0.09 && abs(initialFvForceR - FvForceR) > 0.09
            || abs(initialFvForceL - FvForceL) > 0.16 || abs(initialFvForceR - FvForceR) > 0.16) {
            contactCount++;
            //cout << FvForceL << "   " << FvForceR << endl;

            if (contactCount > 16) {
                cout << "把持条件を満たしました" << endl;
                initialFvForceR = 0.0;
                initialFvForceL = 0.0;
                handRatioGrasp = armTip.hand; // 把持条件を満たしたならば，現在のhandの閉じ具合を記憶
            }
        }
        else {
            contactCount = 0;
        }

        // contactCountの値が一定数以上 なら状態遷移
        if (contactCount > 16) {
            contactCount = 0;
            state = Bring;
            cout << "Grasp -> Bring" << endl;
            Beep(440, 200);
        }
    }
    else if (state == Bring) {
        //アームが停止中なら状態遷移
        if (statusExecuting == false) {
            state = Carry;
            cout << "Bring -> Carry" << endl;
            Beep(440, 200);
        }
    }
    else if (state == Carry) {
        //アームが停止中なら状態遷移
        if (statusExecuting == false) {
            state = Release;
            cout << "Carry -> Release" << endl;
            Beep(440, 200);
        }

    }
    else if (state == Release) {
        //アームが停止中なら状態遷移
        if (statusExecuting == false) {
            state = Initialize;
            cout << "Release -> Initialize" << endl;
            Beep(440, 200);
        }
    }
    else if (state == Initialize) {
        //アームが停止中なら状態遷移
        if (statusExecuting == false) {
            state = Recognize;
            cout << "Initialize -> Recognize" << endl;
            handRatioGrasp = 0.3;
            color = Unknown;
            Beep(440, 200);
        }
    }


    bool transited = false;
    if (state != statePrev) transited = true;

    // 状態ごとの処理
    if (state == Recognize) {

    }
    else if (state == PreApproach) {
        //遷移直後に1度だけ実行する処理
        if (transited) {
            ArmTip target;
            target.x = cylinder.x*(cylDistance - 0.08) / cylDistance;
            target.y = cylinder.y*(cylDistance - 0.08) / cylDistance;
            target.z = cylinder.z;
            target.pitch = 0.0;
            target.hand = 1.0;
            target.time = 3.0;
            setArmTipTarget(target);
        }

    }
    else if (state == Approach) {
        //円柱が掴める所までグリッパを移動させる
        if (transited) {
            ArmTip target;
            target.x = cylinder.x;
            target.y = cylinder.y;
            target.z = cylinder.z;
            target.pitch = 0.0;
            target.hand = 1.0;
            target.time = 4.0;
            setArmTipTarget(target);
        }

    }
    else if (state == PreGrasp) {
        //途中までグリッパを閉じる
        if (transited) {
            ArmTip target;
            target.x = cylinder.x;
            target.y = cylinder.y;
            target.z = cylinder.z;
            target.pitch = 0.0;
            target.hand = handRatioGrasp;
            target.time = 3.0;
            setArmTipTarget(target);
        }

    }
    else if (state == Grasp) {
        // グリッパをゆっくり閉じきる
        if (transited) {
            ArmTip target;
            target.x = cylinder.x;
            target.y = cylinder.y;
            target.z = cylinder.z;
            target.pitch = 0.0;
            target.hand = 0.0;
            target.time = 5.0;
            setArmTipTarget(target);
        }
    }
    else if (state == Bring) {
        //円柱を掴んだまま引き寄せる
        if (transited) {
            ArmTip target;
            target.x = cylinder.x*(cylDistance - 0.08) / cylDistance;
            target.y = cylinder.y*(cylDistance - 0.08) / cylDistance;
            target.z = 0.16;
            target.pitch = 0.0;
            target.hand = handRatioGrasp;
            target.time = 3.0;
            setArmTipTarget(target);
        }
    }
    else if (state == Carry) {
        //色によって設置場所を変える
        if (transited) {
            ArmTip target;
            if (color == Red) {
                cout << "赤色 の設置場所に運搬します" << endl;
                //ArmTip target;
                target.x = 0.14;
                target.y = -0.15;
            }
            else if (color == Yellow) {
                cout << "黄色 の設置場所に運搬します" << endl;
                //ArmTip target;
                target.x = -0.14;
                target.y = 0.08;
            }
            else if (color == Green) {
                cout << "緑色 の設置場所に運搬します" << endl;
                //ArmTip target;
                target.x = 0.14;
                target.y = 0.08;
            }
            else if (color == Blue) {
                cout << "青色 の設置場所に運搬します" << endl;
                //ArmTip target;
                target.x = 0.14;
                target.y = 0.15;
            }
            target.z = 0.16;
            target.pitch = 0.0;
            target.hand = handRatioGrasp;
            target.time = 5.0;
            setArmTipTarget(target);

        }
    }
    else if (state == Release) {
        //グリッパを開く
        if (transited) {
            ArmTip target;
            target.x = armTip.x;
            target.y = armTip.y;
            target.z = armTip.z;
            target.pitch = 0.0;
            target.hand = 1.0;
            target.time = 3.0;
            setArmTipTarget(target);
        } 
    }
    else if (state == Initialize) {
        //(Kinectに映らないように)待機の姿勢をとる
        if (transited) {
            ArmTip target;
            target.x = 0.132;
            target.y = 0.0;
            target.z = 0.03;
            target.pitch = -0.55;
            target.hand = 0.0;
            target.time = 3.0;
            setArmTipTarget(target);
        }
    }
}

bool cmp(const Cylinder &a, const Cylinder &b)
{
    float aDistance = sqrt(pow(a.x, 2) + pow(a.y, 2));
    float bDistance = sqrt(pow(b.x, 2) + pow(b.y, 2));
    return aDistance < bDistance;
}
