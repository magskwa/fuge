/**
 * @file randomgenerator.cpp
 * @author Yvan Da Silva <yvan.m.silva AT gmail.com>
 * @author IICT Institute for Information and Communication<www.iict.ch>
 * @author HEIG-VD (Haute école d'inénierie et de gestion) <www.heig-vd.ch>
 * @date 02.2012
 * @section LICENSE
 *
 * This application is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @class RandomGenerator
 * @brief A random generator
 *
 * RandomGenerator is a random generator, thread safe.
 * It uses the Qt function qsrand and qrand to be multi-platform
 * compatible.
 * RangomGenerator works as a singleton and by so
 * no pointer of it should be kept on the user code.
 */

#include "randomgenerator.h"
#include <cstdlib>

RandomGenerator* RandomGenerator::randomGenerator = NULL;

RandomGenerator::RandomGenerator() :
    QThread()
{}

RandomGenerator *RandomGenerator::getGeneratorInstance(){

    if(randomGenerator == NULL)
        randomGenerator = new RandomGenerator();

    return randomGenerator;
}

qint32 RandomGenerator::random(qint32 min, qint32 max){
    if (min > max){
        qSwap(min,max);
    }
    return QRandomGenerator::global()->generate()%(max-min+1) + min;
}

qreal RandomGenerator::randomReal(qreal min, qreal max){
    if (min > max) {
        qSwap(min, max);
    }
    return QRandomGenerator::global()->generateDouble()*(max-min) + min;
}
