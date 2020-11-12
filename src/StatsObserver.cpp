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
