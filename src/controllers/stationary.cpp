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

#include "stationary.h"

#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <QString>

// parameters of our fitness function
#define ALPHA 3 // begning of the long tail
#define MAX_LOCAL_PERFORMANCE 20 // max score received in one interaction

stationary::stationary()
    : AbstractGACtrl()
{
}

void stationary::Init(TConfigurationNode& t_node)
{
    AbstractGACtrl::Init(t_node);
    Reset();
}

void DemoCtrl::Reset()
{
}

void DemoCtrl::ControlStep()
{
    m_pcSensorOut->SetMessage(10);


}
REGISTER_CONTROLLER(DemoCtrl, "stationary_controller")
