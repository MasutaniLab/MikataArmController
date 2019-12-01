// -*- C++ -*-
/*!
 * @file  MikataArmControllerTest.cpp
 * @brief FVFil to ThisRTC to ArmRTC
 * @date $Date$
 *
 * $Id$
 */

#include "MikataArmControllerTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* mikataarmcontroller_spec[] =
  {
    "implementation_id", "MikataArmControllerTest",
    "type_name",         "MikataArmControllerTest",
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
MikataArmControllerTest::MikataArmControllerTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_fv_filter0_wrenchIn("fv_filter0_wrench", m_fv_filter0_wrench),
    m_fv_filter0_objinfoIn("fv_filter0_objinfo", m_fv_filter0_objinfo),
    m_fv_filter1_wrenchIn("fv_filter1_wrench", m_fv_filter1_wrench),
    m_fv_filter1_objinfoIn("fv_filter1_objinfo", m_fv_filter1_objinfo),
    m_cylinderIn("cylinder", m_cylinder),
    m_armStatusIn("armStatus", m_armStatus),
    m_armTipTargetOut("armTipTarget", m_armTipTarget)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
MikataArmControllerTest::~MikataArmControllerTest()
{
}



RTC::ReturnCode_t MikataArmControllerTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("armTipTarget", m_armTipTargetIn);
  
  // Set OutPort buffer
  addOutPort("fv_filter0_wrench", m_fv_filter0_wrenchOut);
  addOutPort("fv_filter0_objinfo", m_fv_filter0_objinfoOut);
  addOutPort("fv_filter1_wrench", m_fv_filter1_wrenchOut);
  addOutPort("fv_filter1_objinfo", m_fv_filter1_objinfoOut);
  addOutPort("cylinder", m_cylinderOut);
  addOutPort("armStatus", m_armStatusOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t MikataArmControllerTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmControllerTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmControllerTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t MikataArmControllerTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t MikataArmControllerTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t MikataArmControllerTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t MikataArmControllerTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmControllerTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmControllerTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmControllerTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t MikataArmControllerTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void MikataArmControllerTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(mikataarmcontroller_spec);
    manager->registerFactory(profile,
                             RTC::Create<MikataArmControllerTest>,
                             RTC::Delete<MikataArmControllerTest>);
  }
  
};


