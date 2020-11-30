/**
 * @file StatsObserver.cpp
 * @author Paweł Kotiuk
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "StatsObserver.hpp"

StatsObserver::StatsObserver()
    : duration_time_(0)
{
}

StatsObserver::StatsObserver(uint timeout)
    : duration_time_(timeout)
{
}

StatsObserver::~StatsObserver() {}

void StatsObserver::update()
{
    lock guard(lists_mutex_);
    generation_list_.push_back(observed_EvolAlg_->getGeneration());
    fitness_list_.push_back(observed_EvolAlg_->getPercentFitness());
    if (duration_time_ != 0)
    {
        if (!is_timer_initialized_)
        {
            start_time_ = std::chrono::steady_clock::now();
            is_timer_initialized_ = !is_timer_initialized_;
        } else
        {
            std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - start_time_;
            if (elapsed_seconds.count() > duration_time_)
            {
                observed_EvolAlg_->stop();
            }
        }
    }
}

void StatsObserver::setObservedEvolAlg(EvolAlg &p)
{
    observed_EvolAlg_ = &p;
    p.addObserver(this);
    max_gene_count = p.getMaxGeneCount();
    population_size = p.getPopulationSize();
}

list<double> StatsObserver::getFitnesses()
{
    lock guard(lists_mutex_);
    return fitness_list_;
}

list<uint> StatsObserver::getGenerations()
{
    lock guard(lists_mutex_);
    return generation_list_;
}

ResultsContainer StatsObserver::getGenerationsAndFitness()
{
    lock guard(lists_mutex_);
    return ResultsContainer(fitness_list_, generation_list_);
}

void ResultsContainer::exportToCsv(std::string filename)
{

    filename = filename + ".csv";

    std::ofstream file(filename, std::ofstream::out);
    auto iter_gen = generation_list_.begin();
    auto iter_fit = fitness_list_.begin();
    while (iter_gen != generation_list_.end())
    {
        file << *iter_gen << ";" << *iter_fit << std::endl;
        iter_gen++;
        iter_fit++;
    }
    file.close();
}