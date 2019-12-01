// -*- C++ -*-
/*!
 * @file  MikataArmController.cpp
 * @brief FVFil to ThisRTC to ArmRTC
 * @date $Date$
 *
 * $Id$
 */

#include "MikataArmController.h"

#include <iomanip>
#include <vector>
using namespace std;


// Module specification
// <rtc-template block="module_spec">
static const char* mikataarmcontroller_spec[] =
  {
    "implementation_id", "MikataArmController",
    "type_name",         "MikataArmController",
    "description",       "FVFil to ThisRTC to ArmRTC",
    "version",           "1.0.0",
    "vendor",            "MasutaniLab",
    "category",          "ArmController",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
MikataArmController::MikataArmController(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_fv_filter0_wrenchIn("fv_filter0_wrench", m_fv_filter0_wrench),
    m_fv_filter0_objinfoIn("fv_filter0_objinfo", m_fv_filter0_objinfo),
    m_fv_filter1_wrenchIn("fv_filter1_wrench", m_fv_filter1_wrench),
    m_fv_filter1_objinfoIn("fv_filter1_objinfo", m_fv_filter1_objinfo),
    m_cylinderIn("cylinder", m_cylinder),
    m_armTipIn("armTip", m_armTip),
    m_armStatusIn("armStatus", m_armStatus),
    m_armTipTargetOut("armTipTarget", m_armTipTarget)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
MikataArmController::~MikataArmController()
{
}



RTC::ReturnCode_t MikataArmController::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("fv_filter0_wrench", m_fv_filter0_wrenchIn);
  addInPort("fv_filter0_objinfo", m_fv_filter0_objinfoIn);
  addInPort("fv_filter1_wrench", m_fv_filter1_wrenchIn);
  addInPort("fv_filter1_objinfo", m_fv_filter1_objinfoIn);
  addInPort("cylinder", m_cylinderIn);
  addInPort("armTip", m_armTipIn);
  addInPort("armStatus", m_armStatusIn);
  
  // Set OutPort buffer
  addOutPort("armTipTarget", m_armTipTargetOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>


  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t MikataArmController::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmController::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmController::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t MikataArmController::onActivated(RTC::UniqueId ec_id)
{
    RTC_INFO(("onActivated()"));

    m_armTipTarget.data.length(6);
    
    setup();

    for (int i = 0; i < m_cylinder.data.length(); i++)
        m_cylinder.data[i] = 0.0;

    for (int i = 0; i < m_armTip.data.length(); i++)
        m_armTip.data[i] = 0.0;

    armTip.x = 0.0;
    armTip.y = 0.0;
    armTip.z = 0.0;
    armTip.pitch = 0.0;
    armTip.hand = 0.0;

    FvForceL = 0.0;
    FvForceR = 0.0;

    m_armStatus.data = 0;
    m_timeCommand = coil::gettimeofday();

  return RTC::RTC_OK;
}


RTC::ReturnCode_t MikataArmController::onDeactivated(RTC::UniqueId ec_id)
{
    RTC_INFO(("onDeactivated()"));

    cylinderData.clear();

    return RTC::RTC_OK;
}


RTC::ReturnCode_t MikataArmController::onExecute(RTC::UniqueId ec_id)
{
    bool newinfo = false;
    if (m_fv_filter0_wrenchIn.isNew()) {
        m_fv_filter0_wrenchIn.read();

        FvForceR = m_fv_filter0_wrench.force[1];

        newinfo = true;
    }
    if (m_fv_filter1_wrenchIn.isNew()) {
        m_fv_filter1_wrenchIn.read();

        FvForceL = m_fv_filter1_wrench.force[1];

        newinfo = true;
    }
    if (m_fv_filter0_objinfoIn.isNew()) {
        m_fv_filter0_objinfoIn.read();
        newinfo = true;
    }
    if (m_fv_filter1_objinfoIn.isNew()) {
        m_fv_filter1_objinfoIn.read();
        newinfo = true;
    }
    if (m_cylinderIn.isNew()) {
        m_cylinderIn.read();
        
        cylinderData.clear();

        // 円柱の情報(x,y,z,radius,hue)を取得,Cylinder構造体のcylinderDataに格納する
        for (int i=0; i<m_cylinder.data.length()/5; i++)
        {
            Cylinder a;
            a.x = (float)m_cylinder.data[0+i*5];
            a.y = (float)m_cylinder.data[1+i*5];
            a.z = (float)m_cylinder.data[2+i*5];
            a.radius = (float)m_cylinder.data[3+i*5];
            a.hue = (float)m_cylinder.data[4+i*5];

            cylinderData.push_back(a);
        }

        newinfo = true;
    }
    if (m_armTipIn.isNew()) {
        m_armTipIn.read();

        armTip.x = (float)m_armTip.data[0];
        armTip.y = (float)m_armTip.data[1];
        armTip.z = (float)m_armTip.data[2];
        armTip.pitch = (float)m_armTip.data[3];
        armTip.hand = (float)m_armTip.data[4];
        newinfo = true;
    }
    if (m_armStatusIn.isNew()) {
        m_armStatusIn.read();

        statusMoving = ((m_armStatus.data&StatusBitMoving) == StatusBitMoving);
        statusExecuting = ((m_armStatus.data&StatusBitExecuting) == StatusBitExecuting);
        statusCommandError = ((m_armStatus.data&StatusBitCommandError) == StatusBitCommandError);
        statusLowerError = ((m_armStatus.data&StatusBitLowerError) == StatusBitLowerError);
        double elapsedTime = coil::gettimeofday() - m_timeCommand;
        if (elapsedTime < 1.0) {
            statusExecuting = true;
        }
        //cout << statusMoving << statusExecuting << statusCommandError << statusLowerError << endl;

        newinfo = true;
    }

    
    if (newinfo) {
        sendOnce = false;
        loop();
        if (sendOnce == true) {
            m_armTipTargetOut.write();

            std::cout << "send Once" << std::endl;
            sendOnce = false;
            m_timeCommand = coil::gettimeofday(); //
        }
    }

    return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t MikataArmController::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmController::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmController::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmController::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmController::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void MikataArmControllerInit(RTC::Manager* manager)
  {
    coil::Properties profile(mikataarmcontroller_spec);
    manager->registerFactory(profile,
                             RTC::Create<MikataArmController>,
                             RTC::Delete<MikataArmController>);
  }
  
};


