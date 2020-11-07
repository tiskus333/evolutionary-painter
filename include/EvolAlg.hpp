#ifndef EVOL_ALG
#define EVOL_ALG

#include <chrono>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

#include "EvolAlgObserver.hpp"
#include "Member.hpp"

/**
 * @brief Class containing and managing EvolAlg of Members
 *
 */
class EvolAlg
{
  private:
    std::vector<Member> curr_population;
    sf::Image original_image_;
    const sf::Uint8 *original_image_pixels_ptr_;
    unsigned int image_x_size_;
    unsigned int image_y_size_;
    sf::Texture best_member_texture_;
    sf::Sprite best_member_sprite_;
    sf::Texture original_image_texture_;
    sf::Sprite original_image_sprite_;
    std::string input_file_;

    std::list<EvolAlgObserver *> observers_list_;
    bool is_image_loaded_;
    unsigned int population_size_;
    unsigned int max_genes_;
    unsigned long long generations_;
    unsigned long long max_generations_;
    unsigned long long best_fitness_;
    unsigned long long base_fitness_;

    void notifyObservers();

    // variables and functions used for generating new population
    std::default_random_engine generator_;
    std::uniform_real_distribution<float> probability_;

    const float NEW_GENE = 0.05;
    const float DELETE_GENE = 0.01;
    const float MODIFY_GENE = 0.5;
    const float MOVE_GENE = 0.1;
    const float CHANGE_SHAPE = 0.1;
    const float CHANGE_COLOR = 0.1;
    const unsigned int SUPER_MEMBER_COUNT = 1;
    std::vector<Member> tmp_population_;

    void reproduce();
    void crossover();
    void mutate();
    void deleteRectangle(int pos, int index);
    void succesion();

    // GUI
    sf::RenderWindow window_;

  public:
    EvolAlg(/* args */);
    EvolAlg(const std::string &filename, const unsigned int size, const unsigned int genes_count, const unsigned int max_generations = 0);
    ~EvolAlg();

    bool loadInputImage(const std::string &filename);
    void run();
    void init();
    void stop();
    void addObserver(EvolAlgObserver *observer) { observers_list_.push_back(observer); }

    const sf::Image &getInputImage() const { return original_image_; }
    const sf::Texture &getBestMemberTexture() const { return best_member_texture_; }
    unsigned long long getBestFitness() const { return best_fitness_; }
    unsigned long long getGeneration() const { return generations_; }

    bool isRunning() { return window_.isOpen(); }
    bool isImageLoaded() { return is_image_loaded_; }
    double getPercentFitness();
    unsigned long long getbestFitness() { return best_fitness_; }

    void nextGeneration();
};

#endif