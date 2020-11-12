

#include "EvolAlg.hpp"
#include "EvolAlgObserver.hpp"
#include "mutex"

using std::list;
typedef std::lock_guard<std::mutex> lock;

class ResultsContainer
{
  private:
    list<unsigned long long> fitness_list_;
    list<uint> generation_list_;

  public:
    ResultsContainer(list<unsigned long long> fitness_list, list<uint> generation_list)
        : fitness_list_(fitness_list)
        , generation_list_(generation_list)
    {
    }
    ResultsContainer(){};

    list<uint> getGenerations() { return generation_list_; }
    list<unsigned long long> getFitnesses() { return fitness_list_; }
};

class StatsObserver : public EvolAlgObserver
{
  private:
    EvolAlg *observed_EvolAlg_ = nullptr;

    list<unsigned long long> fitness_list_;
    list<uint> generation_list_;
    std::mutex lists_mutex_;

  public:
    uint max_gene_count;
    uint population_size;

    StatsObserver();
    ~StatsObserver();
    virtual void update();
    void setObservedEvolAlg(EvolAlg *p);

    list<unsigned long long> getFitnesses();
    list<uint> getGenerations();
    ResultsContainer getGenerationsAndFitness();
};