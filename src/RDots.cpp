// -*- C++ -*-
/*!
 * @file  RDots.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "RDots.h"

// Module specification
// <rtc-template block="module_spec">
static const char* rdots_spec[] =
  {
    "implementation_id", "RDots",
    "type_name",         "RDots",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "Urabe",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.range_x", "39",
    "conf.default.range_y", "39",
    "conf.default.range_t", "50",
    "conf.default.direction", "0",
    "conf.default.speed", "1",
    "conf.default.density", "0.15",
    // Widget
    "conf.__widget__.range_x", "text",
    "conf.__widget__.range_y", "text",
    "conf.__widget__.range_t", "text",
    "conf.__widget__.direction", "text",
    "conf.__widget__.speed", "text",
    "conf.__widget__.density", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RDots::RDots(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outputOut("output", m_output),
    m_output2Out("output2", m_output2)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RDots::~RDots()
{
}



RTC::ReturnCode_t RDots::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("output", m_outputOut);
  addOutPort("output2", m_output2Out);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("range_x", m_range_x, "39");
  bindParameter("range_y", m_range_y, "39");
  bindParameter("range_t", m_range_t, "50");
  bindParameter("direction", m_direction, "0");
  bindParameter("speed", m_speed, "1");
  bindParameter("density", m_density, "0.15");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RDots::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RDots::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RDots::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RDots::onActivated(RTC::UniqueId ec_id)
{
	if(!libdotsInitialize()){
		std::cerr << "ライブラリを初期化できません" << std::endl;
		return RTC::RTC_ERROR;
	}

	mwArray input(1, 4, mxCELL_CLASS);//1*4の入れ物をつくる.そこに色々いれていく

	mwArray range(1, 3, mxDOUBLE_CLASS);//作る画像のサイズ
	range(1,1) = m_range_x;//39
	range(1,2) = m_range_y;//39
	range(1,3) = m_range_t;//50
	
	mwArray dir(1, 1, mxDOUBLE_CLASS);//刺激の傾き
	dir(1,1) = m_direction;//0

	mwArray speed(1, 1, mxDOUBLE_CLASS);//刺激のスピード
	speed(1,1) = m_speed;//1

	mwArray density(1,1,mxDOUBLE_CLASS);//ドットの濃さ
	density(1,1) = m_density;//0.15

	//入力
	input.Get(1,1).Set(range);
	input.Get(1,2).Set(dir);
	input.Get(1,3).Set(speed);
	input.Get(1,4).Set(density);

	mwArray output;
	mkDots(1, output, input);//ドット刺激の作成（引数_1:返り値の数,s:返り値の値,input:可変長引数）

	//インデックスの受け渡し
	m_output2.width = m_range_x;
	m_output2.length = m_range_y;
	m_output2.time = m_range_t;

	int num = 0;
	double a = 0;
	m_output2.s_data.length(m_range_x * m_range_y * m_range_t);//s_dataの配列の確保_76050

	double *data_copy = new double [m_range_x * m_range_y * m_range_t];
	output.GetData(data_copy,m_range_x * m_range_y * m_range_t);

	for (num = 0; num < m_range_x * m_range_y * m_range_t ; num++) {
		m_output2.s_data[num] = data_copy[num];
	}

	double ddd = dir(1,1);
	double eee = speed(1,1);
	double fff = density(1,1);

	std::cerr << "x = " << m_output2.width << std::endl;
	std::cerr << "y = " << m_output2.length << std::endl;
	std::cerr << "t = " << m_output2.time << std::endl;
	std::cerr << "total:" << num << std::endl;//x*y*t（全要素数）の表示
	std::cerr << "direction = " << ddd << std::endl;
	std::cerr << "speed = " << eee << std::endl;
	std::cerr << "density = " << fff << std::endl;
	
	std::cerr << "V1野への書き出し" << std::endl;
	m_output2Out.write();	//書き出し

	delete[] data_copy;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t RDots::onDeactivated(RTC::UniqueId ec_id)
{
	libdotsTerminate();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RDots::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RDots::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RDots::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RDots::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RDots::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RDots::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RDotsInit(RTC::Manager* manager)
  {
    coil::Properties profile(rdots_spec);
    manager->registerFactory(profile,
                             RTC::Create<RDots>,
                             RTC::Delete<RDots>);
  }
  
};


