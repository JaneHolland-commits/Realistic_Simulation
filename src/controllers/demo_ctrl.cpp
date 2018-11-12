/*
 * KilobotGA
 * Copyright (C) 2017 Marcos Cardinot <mcardinot@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "demo_ctrl.h"

#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <QString>
#include <iostream>

// parameters of our fitness function
#define ALPHA 3 // begning of the long tail
#define MAX_LOCAL_PERFORMANCE 20 // max score received in one interaction

DemoCtrl::DemoCtrl()
: AbstractGACtrl()
, m_iLUTSize(11)
{
}

void DemoCtrl::Init(TConfigurationNode& t_node)
{
  AbstractGACtrl::Init(t_node);

    // parse the configuration file
  GetNodeAttributeOrDefault(t_node, "lut_size", m_iLUTSize, m_iLUTSize);
  if(m_iLUTSize < 3) {
    LOGERR << "[FATAL] Invalid value for lut_size (" << m_iLUTSize
    << "). Should be a integer greater than 2." << std::endl;
  }

  m_lutDistance.reserve(m_iLUTSize);
  m_chromosome.reserve(m_iLUTSize);

  Reset();
}

void DemoCtrl::Reset()
{
  initLUT();
  m_fPerformance = 0.f;
}

void DemoCtrl::ControlStep()
{
    // send an empty message
  m_pcSensorOut->SetMessage(NULL);

    // read messages
  CCI_KilobotCommunicationSensor::TPackets in = m_pcSensorIn->GetPackets();

  uint8_t cluster = 0;
  float distance = m_kMinDistance;

    // if messages received get the distance
  if (in.size()) {

    for (uint32_t i = 1; i <= in.size(); ++i) {

      uint32_t d = in[i].Distance.high_gain;

      distance += d;
    }

        // average distance if more than one message is receives
    distance /= in.size();

	// neighbourhood size + 1 - this is for you Alan
    cluster = in.size()+1;

    m_fPerformance += calcPerformance(getLUTIndex(distance), cluster);
    
  }

  else {
   cluster = 0;
   distance = m_kMaxDistance+1;
 }
 

// update speed
 const MotorSpeed m = m_chromosome[getLUTIndex(distance)].value<MotorSpeed>();

 m_pcMotors->SetLinearVelocity(m.left * SPEED_SCALE, m.right * SPEED_SCALE);

}

QVariant DemoCtrl::randGene() const
{
  const CRange<Real> speedRange(0, 1);
  MotorSpeed m;
  m.left = QString::number(m_pcRNG->Uniform(speedRange),'g', SPEED_PRECISION).toDouble();
  m.right = QString::number(m_pcRNG->Uniform(speedRange), 'g', SPEED_PRECISION).toDouble();
  return QVariant::fromValue(m);
}

bool DemoCtrl::setChromosome(Chromosome chromosome)
{
    // check for lut size. Must be equal to what we have in the .argos script
  if (chromosome.size() != m_iLUTSize) {
    qFatal("\n[FATAL] Acconding to the XML file, the LUT size should be %ld", m_iLUTSize);
    return false;
  }
  m_chromosome = chromosome;
  return true;
}


void DemoCtrl::initLUT()
{
	 m_lutDistance.clear();
	 m_chromosome.clear();

	 const float distInterval = 6.597f;
   //34 is min
	 float distance = m_kMaxDistance+1;

	    // fill the LUT with distance intervals of 7.33 (give or take, it prefers whole numbers, but whatever/
	    // start with 100 and work down to 34. 100 = 0th index, 34 = 9th index.
	 for (uint32_t i = m_iLUTSize; i >0; --i) {
	  m_chromosome.push_back(randGene());
	  m_lutDistance.push_back(distance);
	 // LOGERR << distance << ": " << i << "\n";
	  distance -= distInterval;

	}

}

size_t DemoCtrl::getLUTIndex(float distance) const
{


  // for (size_t idx = m_lutDistance.size(); idx > 0; --idx) {
//	LOGERR << "FIRST " << m_lutDistance[idx-1] << ": " << idx << "\n";
	//}


    // i.e sensing range
	if (distance < m_kMaxDistance) {
		// we're decrementing here, do you follow?
		for (size_t idx = m_lutDistance.size(); idx > 0; --idx) {	
      			if (distance < m_lutDistance[idx-1]) {
       				return idx;
     			}
   		}
 	}
        // 0th index being the worse -> distance of 100.
 	return 1;
}

// the distance taken in here is the returned index number from the method above (getLUTIndex)
int DemoCtrl::calcPerformance(uint8_t cluster, uint8_t distance) const
{

 return distance*cluster;

}

REGISTER_CONTROLLER(DemoCtrl, "kilobot_demo_controller")
