// -*- C++ -*-
/*!
 * @file  LEDController.cpp
 * @brief LED Controller (arduino) RT component
 * @date $Date$
 *
 * $Id$
 */

#include "LEDController.h"

// Module specification
// <rtc-template block="module_spec">
static const char* ledcontroller_spec[] =
  {
    "implementation_id", "LEDController",
    "type_name",         "LEDController",
    "description",       "LED Controller (arduino) RT component",
    "version",           "1.0.0",
    "vendor",            "ssr",
    "category",          "Experimental",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "1",
    //    "conf.default.port", "/dev/ttyUSB0",
    "conf.default.port", "/dev/tty.wchusbserial14530",
    "conf.default.baudrate", "9600", 
    // Widget
    "conf.__widget__.debug", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
LEDController::LEDController(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_digitalOutIn("digitalOut", m_digitalOut)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
LEDController::~LEDController()
{
}



RTC::ReturnCode_t LEDController::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("digitalOut", m_digitalOutIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "1");
  bindParameter("port", m_port, "/dev/ttyUSB0");
  bindParameter("baudrate", m_baudrate, "9600");
  // </rtc-template>



  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t LEDController::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LEDController::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LEDController::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t LEDController::onActivated(RTC::UniqueId ec_id)
{
  try {
    m_pSerialPort = new ssr::SerialPort(m_port.c_str(), m_baudrate);
    //m_pSerialPort = new ssr::SerialPort("/dev/cu.wchusbserial14530", m_baudrate);
  } catch (ssr::ComException& ex) {
    return RTC::RTC_ERROR;
  }
  return RTC::RTC_OK;
}


RTC::ReturnCode_t LEDController::onDeactivated(RTC::UniqueId ec_id)
{
  delete m_pSerialPort;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t LEDController::onExecute(RTC::UniqueId ec_id)
{
  try {
    if (m_digitalOutIn.isNew()) {
      m_digitalOutIn.read();
      if (m_digitalOut.data.length() == 3) {
	double r = m_digitalOut.data[0];
	double g = m_digitalOut.data[1];
	double b = m_digitalOut.data[2];

	r = r > 1.0 ? 1.0 : (r < 0 ? 0 : r);
	g = g > 1.0 ? 1.0 : (g < 0 ? 0 : g);
	b = b > 1.0 ? 1.0 : (b < 0 ? 0 : b);

	uint8_t rr = r * 255;
	uint8_t rg = g * 255;
	uint8_t rb = b * 255;

	std::cout << "INPUT: " << r << ", " << g << ", " << b << std::endl;
	uint8_t h = 0xF1;
	m_pSerialPort->Write(&h, 1);
	coil::usleep(30*1000);
	m_pSerialPort->Write(&h, 1);
	coil::usleep(30*1000);
	m_pSerialPort->Write(&rr, 1);
	coil::usleep(30*1000);
	m_pSerialPort->Write(&rg, 1);
	coil::usleep(30*1000);
	m_pSerialPort->Write(&rb, 1);
	coil::usleep(30*1000);
      }
    }

  } catch (ssr::ComException& ex) {
    return RTC::RTC_ERROR;
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t LEDController::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LEDController::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LEDController::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LEDController::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LEDController::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void LEDControllerInit(RTC::Manager* manager)
  {
    coil::Properties profile(ledcontroller_spec);
    manager->registerFactory(profile,
                             RTC::Create<LEDController>,
                             RTC::Delete<LEDController>);
  }
  
};


