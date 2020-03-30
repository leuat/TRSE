/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "nodeforloop.h"
#include "nodenumber.h"

NodeForLoop::NodeForLoop(QSharedPointer<Node> a, QSharedPointer<Node> b, QSharedPointer<Node> block, QSharedPointer<Node> step, bool unroll, int forcePage, int loopCounter, bool inclusive) :Node(){
    m_a = a;
    m_b = b;
    m_block = block;
    m_inclusive = false; // to change
    m_forcePage = forcePage;
    m_step = step;
    m_loopCounter = loopCounter;
    m_inclusive = inclusive;
    //        m_op = op;
}



