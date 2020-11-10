#include "EvolAlg.hpp"
#include "Member.hpp"
#include "StatsObserver.hpp"
#include <iostream>
#include <string.h>
#include <unistd.h>

using std::cout;

void printHelp()
{
    cout << "Usage:    evolving-rectangles [OPTIONS]" << std::endl;
    cout << "          -i  input file (if not described program will show selection window)" << std::endl;
    cout << "          -p  population size (default 30)" << std::endl;
    cout << "          -g  gene pool, maximum number of rectangles on picture (default 1000)" << std::endl;
    cout << "          -c  end simulation after given amount of generations (empty or 0, no restriction)" << std::endl;
    cout << "          -t  run in terminal mode only (without drawing window)" << std::endl;
    cout << "          -h  print help" << std::endl;
}

/**
 * @brief Opens File select dialog for current system
 *
 * @return std::string containing a path to selected file
 */
std::string openFileSelectDialog()
{
    std::string filename = "";
    FILE *f = popen("zenity --file-selection", "r");
    char c;
    if (!f)
    {
        pclose(f);
        return "";
    }
    c = fgetc(f);
    if (c == -1)
        return "";
    while (c != '\n')
    {
        filename += c;
        c = fgetc(f);
    }
    return filename;
}

int main(int argc, char *argv[])
{
    int option;
    int population_size = 30;
    int gene_count = 1000;
    int max_generations = 0;
    int render_window = true;
    std::string filename = "";

    try
    {
        while ((option = getopt(argc, argv, ":i:p:g:thc:")) != -1)
        { // get option from the getopt() method
            switch (option)
            {
            case 'i':
                filename = optarg;
                break;
            case 'h':
                printHelp();
                return 0;
                break;

            case 'p':
                population_size = std::stoi(optarg);
                if (population_size <= 0)
                    throw("Population size must be greater than 0");
                break;
            case 't':
                render_window = false;
                break;
            case 'g':
                gene_count = std::stoi(optarg);
                if (gene_count <= 0)
                    throw("Gene count must be greater than 0");
                break;
            case 'c':
                max_generations = std::stoi(optarg);
                if (max_generations < 0)
                    throw("Generation count cannot be lower than 0");
                break;
            case ':':
                printf("option needs a value\n");
                return 1;
                break;
            case '?': // used for some unknown options
                printf("unknown option: %c\n", optopt);
                printHelp();
                return 1;
                break;
            }
        }
    } catch (const std::exception &e)
    {
        if (!strcmp(e.what(), "stoul"))
        {
            cout << "Wrong value of parameter -" << char(option) << std::endl;
        } else
        {
            cout << "Unknown exception occured: " << e.what() << std::endl;
        }
        printHelp();
        return -1;
    } catch (const char *msg)
    {
        std::cout << msg << std::endl;
        return -1;
    }

    if (filename == "")
    {
        filename = openFileSelectDialog();
        if (filename == "")
        {
            std::cout << "File not selected\n";
            return -1;
        }
    }

    cout << "Input file: " << filename;
    cout << "\nPopulation size: " << population_size;
    cout << "\nMaximal number of rectangles: " << gene_count << std::endl;

    EvolAlg p(filename, population_size, gene_count, max_generations, render_window);

    StatsObserver o;
    o.setObservedEvolAlg(&p);
    srand(time(NULL));
    p.run();
    return 0;
}
