/**
 * @file TerminalObserver.cpp
 * @author Paweł Kotiuk
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "TerminalObserver.hpp"
#include <iomanip>

TerminalObserver::TerminalObserver() { std::cout << "\e[?25l"; }

TerminalObserver::~TerminalObserver() { std::cout << "\e[?25h"; }

/**
 * @brief method inherited from EvolAlgObserver
 */
void TerminalObserver::update()
{
    if (!started_)
    {
        clock.restart();
        view_timer_.restart();
        first_generation_ = observed_EvolAlg_->getGeneration();
        first_fitness_ = observed_EvolAlg_->getBestFitness();
        if (first_fitness_ != 0)
        {
            started_ = true;
        }
        return;
    }
    if (view_timer_.getElapsedTime().asMilliseconds() > refresh_time_)
    {
        view_timer_.restart();
        auto speed = (observed_EvolAlg_->getGeneration() - first_generation_) / clock.getElapsedTime().asSeconds();
        std::cout << "\rGeneration: " << std::setw(7) << std::left << observed_EvolAlg_->getGeneration();
        std::cout << " Gen/s: " << std::left << std::setw(6) << std::setprecision(5) << speed;
        std::cout << "  Fitness: " << std::setw(6) << std::left << observed_EvolAlg_->getPercentFitness() * 100.0 << "% " << std::flush;
        std::cout << " Time: " << std::setw(5) << std::left << clock.getElapsedTime().asSeconds() << "s  ";
    }
}

/**
 * @brief Register this observer in EvolAlg's list and save pointer to this EvolAlg in TerminalObserver
 *
 * @param p -pointer to observed EvolAlg
 */
void TerminalObserver::setObservedEvolAlg(EvolAlg *p)
{
    observed_EvolAlg_ = p;
    p->addObserver(this);
}
