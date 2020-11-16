#include "StatsObserver.hpp"

StatsObserver::StatsObserver() {}

StatsObserver::~StatsObserver() {}

void StatsObserver::update()
{
    lock guard(lists_mutex_);
    generation_list_.push_back(observed_EvolAlg_->getGeneration());
    fitness_list_.push_back(observed_EvolAlg_->getbestFitness());
}

void StatsObserver::setObservedEvolAlg(EvolAlg &p)
{
    observed_EvolAlg_ = &p;
    p.addObserver(this);
    max_gene_count = p.getMaxGeneCount();
    population_size = p.getPopulationSize();
}

list<unsigned long long> StatsObserver::getFitnesses()
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