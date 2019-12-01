// -*- C++ -*-
/*!
 * @file  MikataArmController.h
 * @brief FVFil to ThisRTC to ArmRTC
 * @date  $Date$
 *
 * $Id$
 */

#ifndef MIKATAARMCONTROLLER_H
#define MIKATAARMCONTROLLER_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">
#include "FingerVisionStub.h"
#include "BasicDataTypeStub.h"

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="port_stub_h">
using namespace FingerVision;
// </rtc-template>

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

#include <vector>
using namespace RTC;

// アーム制御用の手先位置指令と手先位置の現在位置を格納する構造体
struct ArmTip {
    float x;
    float y;
    float z;
    float pitch;
    float hand;
    float time;
 
    /*
    void setData(float x, float y, float z, float pitch, float hand, float time, bool &flag) {
        this->x = x;    // Kinect2ToPC:z -> Arm:x
        this->y = y;    // Kinect2ToPC:x -> Arm:y
        this->z = z;
        this->pitch = pitch;  // pitch
        this->hand = hand;  // hand
        this->time = time;    // time
        flag = true;
    }
    */

};

// 円柱情報を格納する構造体
struct Cylinder {
    float x;
    float y;
    float z;
    float radius;
    float hue;
    void disp() {
        std::cout << "x = " << x << " y = " << y << " z = " << z 
            << " radius = " << radius << " hue = " << hue << std::endl;
    }
};


/*!
 * @class MikataArmController
 * @brief FVFil to ThisRTC to ArmRTC
 *
 */
class MikataArmController
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  MikataArmController(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~MikataArmController();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  FingerVision::Filter1Wrench m_fv_filter0_wrench;
  /*!
   */
  InPort<FingerVision::Filter1Wrench> m_fv_filter0_wrenchIn;
  FingerVision::Filter1ObjInfo m_fv_filter0_objinfo;
  /*!
   */
  InPort<FingerVision::Filter1ObjInfo> m_fv_filter0_objinfoIn;
  FingerVision::Filter1Wrench m_fv_filter1_wrench;
  /*!
   */
  InPort<FingerVision::Filter1Wrench> m_fv_filter1_wrenchIn;
  FingerVision::Filter1ObjInfo m_fv_filter1_objinfo;
  /*!
   */
  InPort<FingerVision::Filter1ObjInfo> m_fv_filter1_objinfoIn;
  RTC::TimedDoubleSeq m_cylinder;
  /*!
   */
  InPort<RTC::TimedDoubleSeq> m_cylinderIn;
  RTC::TimedDoubleSeq m_armTip;
  /*!
   */
  InPort<RTC::TimedDoubleSeq> m_armTipIn;
  RTC::TimedUShort m_armStatus;
  /*!
   */
  InPort<RTC::TimedUShort> m_armStatusIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedDoubleSeq m_armTipTarget;
  /*!
   */
  OutPort<RTC::TimedDoubleSeq> m_armTipTargetOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>
     
     void setup();
     void loop();

     // 構造体「ArmTip」のオブジェクト(x,y,z,pitch,hand,time)
     ArmTip armTipTarget;

     // 構造体「ArmTip」のオブジェクト(x,y,z,pitch,hand)
     ArmTip armTip;

     // 構造体「Cylinder」のベクター
     std::vector<Cylinder>cylinderData; // 構造体「Cylinder」のオブジェクト(インスタンス) (x,y,z,radius,hue)

     // 入力されたm_fv_filter1_wrench.force[1](力の平均z成分 左)の値を保持する
     double FvForceL;
     // 入力されたm_fv_filter0_wrench.force[1](力の平均z成分 右)の値を保持する
     double FvForceR;

     bool sendOnce; // m_armTipTargetOut.Write()を状態遷移時に一度だけ送るためのフラグ

     //停止の判断
     coil::TimeValue m_timeCommand;

     const int StatusBitMoving = 1;
     const int StatusBitExecuting = 2;
     const int StatusBitCommandError = 4;
     const int StatusBitLowerError = 8;
     bool statusMoving;
     bool statusExecuting;
     bool statusCommandError;
     bool statusLowerError;

     // 受け取ったオブジェクトで手先位置指令を設定する
     void  MikataArmController::setArmTipTarget(ArmTip target)
     {
         m_armTipTarget.data[0] = target.x;
         m_armTipTarget.data[1] = target.y;
         m_armTipTarget.data[2] = target.z;
         m_armTipTarget.data[3] = target.pitch;
         m_armTipTarget.data[4] = target.hand;
         m_armTipTarget.data[5] = target.time;
         sendOnce = true;
     }
};


extern "C"
{
  DLL_EXPORT void MikataArmControllerInit(RTC::Manager* manager);
};

#endif // MIKATAARMCONTROLLER_H
