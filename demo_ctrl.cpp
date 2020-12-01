#include "demo_ctrl.h"

#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <QString>

#include <argos3/core/simulator/visualization/default_visualization.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>

#include <iostream>
#include <cstring>
#include <string>
#include <argos3/core/utility/math/vector3.h> 

#include <sys/time.h>
#include <unistd.h>
#include <time.h>

// parameters of our fitness function
#define ALPHA 1 // begning of the long tail
#define MAX_LOCAL_PERFORMANCE 10 // max score received in one interaction

DemoCtrl::DemoCtrl()
: AbstractGACtrl()
, m_pcRNG(CRandom::CreateRNG("kilobotga"))
, m_iLUTSize(10)
{
}

void DemoCtrl::Init(TConfigurationNode& t_node)
{
    AbstractGACtrl::Init(t_node);

    // parse the configuration file
    GetNodeAttributeOrDefault(t_node, "lut_size", m_iLUTSize, m_iLUTSize);
    if(m_iLUTSize < 2) {
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

    //Handling signals received
    //if received more than 1 message, take the average distance
    //otherwise, use the max+1 distance (no-signal)

    uint32_t distance = 0;

    // send an empty message
    m_pcSensorOut->SetMessage(NULL);

    // read messages
    CCI_KilobotCommunicationSensor::TPackets in = m_pcSensorIn->GetPackets();

    if (in.size()) {

 	// if Kilobot senses another robot
	distance =  1;

	m_fPerformance += calcPerformance(getLUTIndex(distance));
   
   }

   else {

	distance = 0;
    }


// update speed
const MotorSpeed m = m_chromosome[getLUTIndex(distance)].value<MotorSpeed>();

   // LOGERR << "motor" << getLUTIndex(distance) << "\n";

m_pcMotors->SetLinearVelocity(m.left * SPEED_SCALE, m.right * SPEED_SCALE);


	/*else if (in.size() == 2) {
	LOGERR << "RED" << m.left * SPEED_SCALE << "\n";
	}

        else if (in.size() == 3) {
        LOGERR << "YELLOW" << m.left * SPEED_SCALE << "\n";
	}

	else if (in.size() == 4) {
        LOGERR << "ORANGE" << m.left * SPEED_SCALE << "\n";
	}

	else {
	 LOGERR << "ZERO" << m.left * SPEED_SCALE << "\n";
	} */

	/*std::ofstream motorFile;
	motorFile.open("motor.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	motorFile << m.left << "\t" << m.right<< std::endl;
	motorFile.close();*/
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

    // first and last elements must hold the decision for MIN and MAX distance
    // i.e., [34, ... , no-signal]   round((m_kMaxDistance - m_kMinDistance) / (double)(m_iLUTSize - 2.0));
    //const float distInterval = round((m_kMaxDistance - m_kMinDistance)/(double)(m_iLUTSize - 2.0));
    //int distance = m_kMinDistance;

      const float distInterval = 1;
      int distance = 0;

    for (uint32_t i = 0; i < m_iLUTSize; ++i) {
        m_chromosome.push_back(randGene());
        m_lutDistance.push_back(distance);
        distance += distInterval;
    }
}

size_t DemoCtrl::getLUTIndex(uint8_t distance) const
{
	if (distance > 0) {
		return 1;
	}

	return 0;
}


float DemoCtrl::calcPerformance(uint8_t distance) const
{

   return distance;

}

REGISTER_CONTROLLER(DemoCtrl, "kilobot_demo_controller")
