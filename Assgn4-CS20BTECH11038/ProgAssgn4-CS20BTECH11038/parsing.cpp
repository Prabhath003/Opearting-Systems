#include <iostream>
#include <fstream>

int main()
{
    std::ifstream input;

    std::string hit = "Bounded-CAS-ME";
    std::string file;

    for(unsigned i = 1; i <= 5; i++)
    {
    file = hit + std::to_string(i) + ".txt";
    std::cout << file << " : " << std::endl;
    input.open(file, std::ios::in);

    std::string filename;

    double sum = 0;
    double max = 0;
    int count = 0;

    while(getline(input, filename))
    {
        if(filename[0] == 'w')
        {
            std::string sub = filename.substr(14);
            sum += stoi(sub);
            if(max < stoi(sub))
                max = stoi(sub);
            count++;
        }
        else continue;
    }

    std::cout << sum/count << std::endl;
    std::cout << max << std::endl;
    input.close();
    }

    return 0;
}