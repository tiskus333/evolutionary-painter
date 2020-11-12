#include "StatsObserver.hpp"

StatsObserver::StatsObserver() {}

StatsObserver::~StatsObserver() {}

void StatsObserver::update()
{
    std::lock_guard(lists_mutex_);
    fitness_list_.push_back(observed_EvolAlg_->getGeneration());
    generation_list_.push_back(observed_EvolAlg_->getbestFitness());
}

void StatsObserver::setObservedEvolAlg(EvolAlg *p)
{
    observed_EvolAlg_ = p;
    p->addObserver(this);
    max_gene_count = p->getMaxGeneCount();
    population_size = p->getPopulationSize();
}

list<unsigned long long> StatsObserver::getFitnesses()
{
    std::lock_guard(lists_mutex_);
    return fitness_list_;
}

list<uint> StatsObserver::getGenerations()
{
    std::lock_guard(lists_mutex_);
    return generation_list_;
}

ResultsContainer StatsObserver::getGenerationsAndFitness()
{
    std::lock_guard(lists_mutex_);
    return ResultsContainer(fitness_list_,generation_list_);
}