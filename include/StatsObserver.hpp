

#include "EvolAlg.hpp"
#include "EvolAlgObserver.hpp"
#include "mutex"

using std::list;

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
};