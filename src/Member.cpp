#include "Member.hpp"

/**
 * @brief Construct a new Member:: Member object
 *
 * @param max_genes - maximal number of Rectangles in picture
 * @param x - width of image
 * @param y -height of image
 */
Member::Member(unsigned int max_genes, unsigned int x, unsigned int y)
    : fitness_(0)
    , image_x_size_(x)
    , image_y_size_(y)
    , max_rectangles_(max_genes * 4)
    , distribution_(0, 3)
{
    rectangles_.reserve(max_genes);
}

/**
 * @brief Calculate fitness of Member's image. Squared difference between pixels value. Lower fitness is better.
 *
 * @param original_pixels_ptr -pointer to imput image
 */
void Member::calculateFitness(const sf::Uint8 *const original_pixels_ptr)
{
    if (modified_) // skip if nothing changed
    {
        unsigned long fitness_local = 0;
        sf::RenderTexture generated_texture;
        generated_texture.create(image_x_size_, image_y_size_);
        generated_texture.clear();
        generated_texture.draw(&rectangles_[0], rectangles_.size(), sf::Quads);
        generated_texture.display();

        const auto image = generated_texture.getTexture().copyToImage();
        const sf::Uint8 *generated_pixels_ptr = image.getPixelsPtr();
        if (!generated_pixels_ptr || !original_pixels_ptr)
            std::cout << "blad wskaznikow" << std::endl;

        for (unsigned int i = 0; i < image_x_size_ * image_y_size_ * 4; i += 4)
        {
            fitness_local += (original_pixels_ptr[i + 0] - generated_pixels_ptr[i + 0]) * (original_pixels_ptr[i + 0] - generated_pixels_ptr[i + 0]) +
                             (original_pixels_ptr[i + 1] - generated_pixels_ptr[i + 1]) * (original_pixels_ptr[i + 1] - generated_pixels_ptr[i + 1]) +
                             (original_pixels_ptr[i + 2] - generated_pixels_ptr[i + 2]) * (original_pixels_ptr[i + 2] - generated_pixels_ptr[i + 2]) +
                             (original_pixels_ptr[i + 3] - generated_pixels_ptr[i + 3]) * (original_pixels_ptr[i + 3] - generated_pixels_ptr[i + 3]);
        }
        fitness_ = fitness_local;
        modified_ = false;
    }
}

/**
 * @brief compare fitness of two Members
 *
 * @param os
 * @return true
 * @return false
 */
bool Member::operator<(const Member &os) const { return fitness_ < os.fitness_; }

/**
 * @brief Returns sf::Texture for given EvolAlg member
 *
 * @return sf::Texture
 */
sf::Texture Member::getTexture()
{
    sf::RenderTexture generated_texture;
    generated_texture.create(image_x_size_, image_y_size_);
    generated_texture.clear();

    generated_texture.draw(&rectangles_[0], rectangles_.size(), sf::Quads);

    generated_texture.display();
    return generated_texture.getTexture();
}

/**
 * @brief Add new Rectangle to model describing Member
 *
 */
void Member::addGene()
{
    if (rectangles_.size() < max_rectangles_)
    {
        sf::Vector2f v1(rand() % image_x_size_, rand() % image_x_size_), v2(rand() % image_x_size_, rand() % image_y_size_);
        sf::Color color(rand() % 256, rand() % 256, rand() % 256, 128);
        rectangles_.emplace_back(v1, color);
        rectangles_.emplace_back(sf::Vector2f(v2.x, v1.y), color);
        rectangles_.emplace_back(v2, color);
        rectangles_.emplace_back(sf::Vector2f(v1.x, v2.y), color);
        modified_ = true;
    }
}
/**
 * @brief Calculate if the Rectangle moved out of the picture and is no longer visible
 *
 * @param gene_index
 * @return true
 * @return false
 */
bool Member::isOutOfBounds(const unsigned int gene_index)
{

    return (rectangles_.at(gene_index).position.x >= image_x_size_ || rectangles_.at(gene_index + 2).position.x <= 0 ||
            rectangles_.at(gene_index).position.y >= image_y_size_ || rectangles_.at(gene_index + 2).position.y <= 0);
}
/**
 * @brief Delete Rectangle at given index
 *
 * @param gene_index
 */
void Member::deleteGene(const unsigned int gene_index)
{
    if (rectangles_.size() > 0 && gene_index < rectangles_.size())
    {
        for (int i = 3; i >= 0; --i)
        {
            std::swap(rectangles_.at(gene_index + i), rectangles_.at(rectangles_.size() - 1));
            rectangles_.pop_back();
        }
        modified_ = true;
    }
}
/**
 * @brief Randomly change color of Rectangle at given index
 *
 * @param gene_index
 */
void Member::mutateColor(const unsigned int gene_index)
{
    sf::Color tmp_color = rectangles_.at(gene_index).color;
    tmp_color.r += distribution_(generator_);
    tmp_color.g += distribution_(generator_);
    tmp_color.b += distribution_(generator_);
    for (int i = 0; i < 4; ++i)
        rectangles_.at(gene_index + i).color = tmp_color;
    modified_ = true;
}
/**
 * @brief Randomly change position of Rectangle at given index. Rectangle is deleted when moves out of view
 *
 * @param gene_index
 */
void Member::mutatePosition(const unsigned int gene_index)
{
    sf::Vector2f move_(distribution_(generator_), distribution_(generator_));
    for (int i = 0; i < 4; ++i)
    {
        rectangles_.at(gene_index + i).position.x += move_.x;
        rectangles_.at(gene_index + i).position.y += move_.y;
    }
    modified_ = true;
    if (isOutOfBounds(gene_index))
        deleteGene(gene_index);
}
/**
 * @brief randomly change radius of Rectangle at given index. Rectangle is deleted when it becomes too small
 *
 * @param gene_index
 */
void Member::mutateShape(const unsigned int gene_index)
{
    int vert = rand() % 4;
    float rand_scale = 1.0 + distribution_(generator_);

    if (rand() % 2)
        for (int i = 0; i < 4; ++i)
        {
            rectangles_.at(gene_index + i).position.x =
                rectangles_.at(gene_index + i).position.x * rand_scale + rectangles_.at(gene_index + vert).position.x * (1.0 - rand_scale);
        }
    else
        for (int i = 0; i < 4; ++i)
        {
            rectangles_.at(gene_index + i).position.y =
                rectangles_.at(gene_index + i).position.y * rand_scale + rectangles_.at(gene_index + vert).position.y * (1.0 - rand_scale);
        }
    modified_ = true;
    if (isOutOfBounds(gene_index))
        deleteGene(gene_index);
}


bool Member::isModified() { return modified_; }
void Member::setModified(bool val) { modified_ = val; }
unsigned long long Member::getFitness() { return fitness_; }
unsigned int Member::relative_size() { return rectangles_.size() / 4; }