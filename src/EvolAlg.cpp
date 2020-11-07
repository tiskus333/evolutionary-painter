#include "EvolAlg.hpp"

/**
 * @brief Construct a new EvolAlg:: EvolAlg object
 *
 */
EvolAlg::EvolAlg(/* args */) {}

/**
 * @brief Construct a new EvolAlg:: EvolAlg object and load input image from file
 *
 * @param filename - name of input file
 * @param population_size - number of members in population
 * @param genes_count
 * @param max_generations - maximal number of generations if 0 then then number of generations is not limited
 */
EvolAlg::EvolAlg(const std::string &filename, const unsigned int population_size, const unsigned int genes_count, const unsigned int max_generations)
    : input_file_(filename)
    , is_image_loaded_(false)
    , population_size_(population_size)
    , max_genes_(genes_count)
    , generations_(0)
    , max_generations_(max_generations)
    , best_fitness_(0)
    , base_fitness_(0)
    , probability_(0, 1)
{
    loadInputImage(filename);
    init();
}
/**
 * @brief Destroy the Evol Alg:: Evol Alg object
 *
 */
EvolAlg::~EvolAlg() {}

/**
 * @brief Initialize algorithm
 *
 */
void EvolAlg::init()
{
    curr_population.reserve(population_size_);
    best_member_texture_.create(image_x_size_, image_y_size_);
    original_image_pixels_ptr_ = original_image_.getPixelsPtr();

    for (unsigned int i = 0; i < image_y_size_ * image_x_size_ * 4; i += 4)
    {
        base_fitness_ += original_image_pixels_ptr_[i] * original_image_pixels_ptr_[i] +
                         original_image_pixels_ptr_[i + 1] * original_image_pixels_ptr_[i + 1] +
                         original_image_pixels_ptr_[i + 2] * original_image_pixels_ptr_[i + 2];
    }

    for (unsigned int i = 0; i < population_size_; ++i)
    {
        curr_population.emplace_back(max_genes_, image_x_size_, image_y_size_);
        curr_population[i].addGene();
        curr_population[i].calculateFitness(original_image_pixels_ptr_);
    }

    // scale image view to fit max window size. Doesn't affect image size.
    const sf::Vector2f MAX_SIZE(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height);
    float scale = 1.0;
    if (image_x_size_ > MAX_SIZE.x)
        scale = MAX_SIZE.x / image_x_size_;
    if (image_y_size_ * scale > MAX_SIZE.y)
        scale *= MAX_SIZE.y / image_y_size_;
    best_member_sprite_.setScale(sf::Vector2f(scale, scale));
    original_image_sprite_.setScale(sf::Vector2f(scale, scale));

    original_image_texture_.loadFromImage(original_image_);
    original_image_sprite_.setTexture(original_image_texture_);
    window_.create(sf::VideoMode(2 * image_x_size_ * scale, image_y_size_ * scale), "Image overview", sf::Style::Close);
}
/**
 * @brief Start main loop of algorithm
 *
 */
void EvolAlg::run()
{
    while (window_.isOpen() && (max_generations_ == 0 || generations_ < max_generations_))
    {
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                stop();
                return;
            }
        };
        nextGeneration();
        best_fitness_ = curr_population.at(0).getFitness();
        best_member_texture_.update(curr_population.at(0).getTexture());
        best_member_sprite_.setTexture(best_member_texture_);
        best_member_sprite_.setPosition(image_x_size_, 0);
        window_.clear();
        window_.draw(best_member_sprite_);
        window_.draw(original_image_sprite_);
        window_.display();

        notifyObservers();
    }
    stop();
}
/**
 * @brief load target image from file
 *
 * @param filename
 * @return true loaded succesfully
 * @return false if any problem
 */
bool EvolAlg::loadInputImage(const std::string &filename)
{
    is_image_loaded_ = original_image_.loadFromFile(filename);
    if (!is_image_loaded_)
        exit(-1);
    image_x_size_ = original_image_.getSize().x;
    image_y_size_ = original_image_.getSize().y;
    return is_image_loaded_;
}

/**
 * @brief Notify every registered observer
 */
void EvolAlg::notifyObservers()
{
    for (auto i : observers_list_)
        i->update();
}

/**
 * @brief Stop main loop of algorithm
 *
 */
void EvolAlg::stop()
{
    std::stringstream output_file;
    std::string name;
    size_t path_len;
#if (unix)
    path_len = input_file_.find_last_of("/");
#elif _WIN32
    path_len = input_file_.find_last_of("\\");
#endif
    output_file << input_file_.substr(0, path_len + 1) << "generated_";                            // path to file location
    output_file << input_file_.substr(path_len + 1, input_file_.find_last_of(".") - path_len - 1); // name of file (without type)
    output_file << "_" << generations_;                                                            // number of generation
    output_file << "_" << getPercentFitness() * 100.0 << "percent.jpg";                            // fitness in percent and filetype

    curr_population.at(0).getTexture().copyToImage().saveToFile(output_file.str());
    std::cout << std::endl << "File saved as: \"" << output_file.str() << "\"" << std::endl;
    if (window_.isOpen())
        window_.close();
}
/**
 * @brief Agregate other functions used for generating new population
 *
 */
void EvolAlg::nextGeneration()
{
    reproduce();
    mutate();
    succesion();
    for (auto &member : curr_population)
        member.calculateFitness(original_image_pixels_ptr_);
    std::sort(curr_population.begin(), curr_population.end());
    ++generations_;
}

/**
 * @brief Create new population using two player tournament
 *
 */
void EvolAlg::reproduce()
{
    unsigned int index_a, index_b;
    tmp_population_.clear();
    tmp_population_.reserve(population_size_);
    // population_size - supermember times we choose new member for next generations
    for (unsigned int i = 0; i < population_size_ - SUPER_MEMBER_COUNT; ++i)
    {
        index_a = probability_(generator_) * population_size_;
        index_b = probability_(generator_) * population_size_;
        if (curr_population.at(index_a) < curr_population.at(index_b))
            tmp_population_.push_back(curr_population.at(index_a));
        else
            tmp_population_.push_back(curr_population.at(index_b));
    }
}

/**
 * @brief Mutate tmp_population including: color, radius, position of at most 1 rectangle for each member. Based on constants in EvolAlg.hpp
 *
 */
void EvolAlg::mutate()
{
    for (unsigned int pos = 0; pos < tmp_population_.size(); ++pos)
    {
        unsigned int gene_index = static_cast<unsigned int>(probability_(generator_) * (tmp_population_.at(pos).relative_size())) * 4;
        if (probability_(generator_) <= NEW_GENE)
        {
            tmp_population_.at(pos).addGene();
        } else if (probability_(generator_) <= DELETE_GENE)
        {
            tmp_population_.at(pos).deleteGene(gene_index);
        } else if (probability_(generator_) <= MODIFY_GENE)
        {
            if (tmp_population_.at(pos).rectangles_.size() <= 0)
                continue;
            if (probability_(generator_) <= MOVE_GENE)
                tmp_population_.at(pos).mutatePosition(gene_index);
            else if (probability_(generator_) <= CHANGE_RADIUS)
                tmp_population_.at(pos).mutateShape(gene_index);
            else if (probability_(generator_) <= CHANGE_COLOR)
                tmp_population_.at(pos).mutateColor(gene_index);
        }
    }
}
/**
 * @brief old generation becomes the current one
 *
 */
void EvolAlg::succesion()
{
    for (unsigned int i = 0; i < SUPER_MEMBER_COUNT; ++i)
    {
        tmp_population_.push_back(curr_population.at(i));
    }
    curr_population.swap(tmp_population_);
}
/**
 * @brief Return percentage match to the original image
 *
 * @return double
 */
double EvolAlg::getPercentFitness() { return 1.0 - sqrt((best_fitness_ / static_cast<double>(base_fitness_))); }