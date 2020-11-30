/**
 * @file EvolAlgObserver.hpp
 * @author Paweł Kotiuk
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef EVOL_ALG_OBSERVER
#define EVOL_ALG_OBSERVER

/**
 * @brief Interface used for observing EvolAlg
 */
class EvolAlgObserver
{
  protected:
  public:
    virtual void update() = 0;
};

#endif