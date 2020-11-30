/**
 * @file Member.hpp
 * @author Mateusz Chruściel
 * @brief File containing class Member
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef MEMBER_HPP_
#define MEMBER_HPP_
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

/**
 * @brief Member class represents one member of EvolAlg, holds vector for rectangles and fitness score
 *
 */
class Member
{
  private:
    unsigned long long fitness_;
    unsigned int image_x_size_;
    unsigned int image_y_size_;
    bool modified_ = false;
    unsigned int max_rectangles_;
    std::normal_distribution<float> distribution_;

  public:
    // std::vector<sf::rectangleShape> rectangles_;
    std::vector<sf::Vertex> rectangles_;
    std::default_random_engine generator_;

    Member(unsigned int max_genes, const unsigned int x, const unsigned int y);
    void calculateFitness(const sf::Uint8 *original_pixels_ptr);
    bool operator<(const Member &os) const;
    sf::Texture getTexture();
    void addGene();
    void deleteGene(const unsigned int gene_index);
    bool isOutOfBounds(const unsigned int gene_index);
    void mutateColor(const unsigned int gene_index);
    void mutatePosition(const unsigned int gene_index);
    void mutateShape(const unsigned int gene_index);
    void setModified(bool val);
    bool isModified();
    unsigned int relative_size();
    unsigned long long getFitness();
};
#endif // !MEMBER_HPP_
