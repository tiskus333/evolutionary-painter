/**
 * @file StatsObserver.hpp
 * @author Paweł Kotiuk
 * @brief File containg header describing class StatsObserver
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <chrono>
#include <fstream>

#include "EvolAlg.hpp"
#include "EvolAlgObserver.hpp"
#include "mutex"

using std::list;
typedef std::lock_guard<std::mutex> lock;
using namespace std::chrono;

class ResultsContainer
{
  private:
    list<double> fitness_list_;
    list<uint> generation_list_;

  public:
    ResultsContainer(list<double> fitness_list, list<uint> generation_list)
        : fitness_list_(fitness_list)
        , generation_list_(generation_list)
    {
    }
    ResultsContainer(){};

    list<uint> getGenerations() { return generation_list_; }
    list<double> getFitnesses() { return fitness_list_; }
    void exportToCsv(std::string filename);
};

class StatsObserver : public EvolAlgObserver
{
  private:
    EvolAlg *observed_EvolAlg_ = nullptr;

    list<double> fitness_list_;
    list<uint> generation_list_;
    std::mutex lists_mutex_;

    bool is_timer_initialized_ = false;
    std::chrono::_V2::steady_clock::time_point start_time_;
    uint duration_time_;

  public:
    uint max_gene_count;
    uint population_size;

    StatsObserver();
    StatsObserver(uint timeout);
    ~StatsObserver();
    virtual void update();
    void setObservedEvolAlg(EvolAlg &p);

    list<double> getFitnesses();
    list<uint> getGenerations();
    ResultsContainer getGenerationsAndFitness();
};