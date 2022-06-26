#include <fstream>
#include <iostream>

int main()
{
    std::fstream output;
    output.open("inp-params.txt", std::ios::out);

    int num = 3;

    output << num << std::endl;
    for(unsigned i = 0; i < num; i++)
    {
        output << i + 1 << " 20 " << "50 " << "10" << std::endl;
    }
}

10
1 20 90 10
2 30 250 10
3 70 370 10
4 50 330 10
5 125 2000 10
6 35 80 10
7 25 50 10
8 20 100 10
9 25 40 10
10 30 75 10