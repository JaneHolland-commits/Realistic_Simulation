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

#ifndef STATIONARY_H
#define STATIONARY_H

#include "abstractga_ctrl.h"

/**
 * @brief The DemoCtrl class
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
class stationary : public AbstractGACtrl
{

public:
    stationary();
    virtual ~stationary() {}

    // CCI_Controller stuff
    virtual void Init(TConfigurationNode& t_node);
    virtual void ControlStep();
    virtual void Reset();
};

#endif // STATIONARY_H
